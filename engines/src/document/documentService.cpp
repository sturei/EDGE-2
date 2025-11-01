#include <map>
#include <string>
#include <iostream>
#include <poll.h>
#include <nlohmann/json.hpp>
#include "document/document.h"
#include "document/documentService.h"

using json = nlohmann::json;

/**
 * DocumentService enables interprocess communication with a live Document
 * External clients send actions via stdin, and receive responses via stdout. Errors and logs are sent to stderr.
 * This is quick-and-dirty IPC. In future, websockets or messaging services such as ZeroMQ could be used instead, or as well.
 */

namespace e2 {
    namespace DocumentService {

        // The response sent to the output is a JSON string representing this structure, i.e. a string like: {"status":<string>, "reason":<string>}
        struct ActionResponse
        {
            std::string status;       // "OK" or "ERROR"
            std::string reason;       // human-readable reason for failure
        };

        void to_json(json& j, const ActionResponse& ar) {
            j = json{
                {"status", ar.status}, 
                {"reason", ar.reason}
            };
        }

        /** This method converts a JSON string to a json object representing an action.
         *  The format for an action is: {"type":<string>, "payload":<any valid JSON>}
         *  Returns true if successful, false otherwise. In case of failure, response contains the details.
        */
        bool parseAction(const std::string& line, ActionSpec& action, ActionResponse& response) {
            try
            {
                json jsonAction = json::parse(line);
                //std::cerr << "Parsed input to json: " << jsonAction.dump() << std::endl;    //--- DEBUG ---
                action.type = jsonAction.at("type");
                action.payload = jsonAction.at("payload");
            }
            catch (const json::exception& e)
            {
                std::cerr << "Error parsing input: " << e.what() << std::endl;
                response = {"ERROR", "Invalid JSON: " + std::string(e.what())};
                return false;
            }
            return true;
        }

        /** This method dispatches the specified action to the document. 
         * Returns true if successful, false otherwise. In case of failure, response contains the details.
        */
        bool dispatchAction(Document* document, const ActionSpec& action, ActionResponse& response) {
            ActionResult result = document->dispatchAction(action);
            if (result == ActionResult::SUCCESS) {
                response = {"OK", ""};
            }
            else if (result == ActionResult::UNKNOWN_ACTION) {
                std::cerr << "Unknown action type: " << action.type << std::endl;
                response = {"ERROR", "Unknown action type: " + action.type};
                return false;
            }
            else if (result == ActionResult::INVALID_PAYLOAD) {
                response = {"ERROR", "Invalid payload for action type: " + action.type};
                return false;
            }
            else  {// INTERNAL_ERROR
                response = {"ERROR", "Internal error. Failed to execute action: " + action.type};
                return false;
            }
            return true;
        }

        /** Reads one line of text from the input stream, parses it and dispatches it.
         * Responses are written to the output stream. TODO: structured responses as JSON.
         * It blocks on input, waiting for a line to be entered.
        */
        void runOnce(Document* document, std::istream& input, std::ostream& output) {
            // Read input and process it. getLine will block and wait for input if there is no input available.
            // We always acknowledge the input on the output stream because clients may be blocking, awaiting a response.

            ActionSpec action;
            ActionResponse response;

            std::string line;
            if (!std::getline(input, line)) {
                response = {"ERROR", "Invalid stream or EOF"};
            }
            else if (line.empty()) {
                // empty line is not an error, just acknowledge it
                response = {"OK", "Empty line"};
            }
            else if (!parseAction(line, action, response)) {
                // parseAction sets the response in case of error
            }
            else if (!dispatchAction(document, action, response)) {
                // dispatchAction sets the response in case of error
            }
            else {
                // success
                response = {"OK", ""};
            }

            // package up the response as JSON and stream it to the output stream
            json jsonResponse = response;
            output << jsonResponse;
        }   

        /** Similar to runOnce, but it first polls the input stream, and only reads from it if something is available.
         * It was used in the now-defunct desktop viewer to read input from stdin in the main application loop, without 
         * blocking the UI. It only works for stdin and stout, and probably not at all on Windows. 
        */
        void runOnceWithoutBlocking(Document* document) {
            // Unfortunately this code uses a System call (poll) instead of language-provided methods. 
            // I tried cin.peek() and cin.rdbuf()->in_avail() but to no avail.
            const size_t len = 1;
            const int timeoutMillis = 0;    // 0 millis => non-blocking
            pollfd cinfd[len];
            cinfd[0].fd = fileno(stdin);
            cinfd[0].events = POLLIN;
            if (poll(cinfd, len, timeoutMillis)) {
                runOnce(document);
            }
        }   

        /** This runs the document service in a loop.
         * It reads one line at a time from the specified input stream (stdin by default) and processes it. 
         * The line is expected to consist of a JSON string representing an action.
         * The format for an action is: {"type":<string>, "payload":<any valid JSON>}
         * Responses are written to the specified output stream (stdout by default). 
         * Errors and logs are written to stderr.
         */
        void run(Document* document, std::istream& input, std::ostream& output) {
            while (true) {
                runOnce(document, input, output);
            }
        }

    };
};  