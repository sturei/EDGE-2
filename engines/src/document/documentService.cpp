#include <map>
#include <string>
#include <iostream>
#include <poll.h>
#include <nlohmann/json.hpp>
#include "document/document.h"
#include "document/documentService.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

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
            std::string status = "OK";                              // "OK" or "ERROR"
            std::string reason = "";                                // human-readable reason for failure, or empty string
            ordered_json clientActions = ordered_json::array();           // a JSON array of actions to be performed by the client, or empty array
        };

        void to_json(ordered_json& j, const ActionResponse& ar) {
            j["status"] = ar.status;
            j["reason"] = ar.reason;
            j["clientActions"] = ar.clientActions;
        }   

        /** This method converts a JSON string to a json object representing an action.
         *  The format for an action is: {"type":<string>, "payload":<any valid JSON>}
         *  Returns true if successful, false otherwise. In case of failure, response contains the details.
        */
        bool parseAction(const std::string& line, ActionSpec& action, ActionResponse& response) {
            try
            {
                json jsonAction = json::parse(line);
                action.type = jsonAction.at("type");
                action.payload = jsonAction.at("payload");
            }
            catch (const json::exception& e)
            {
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

                // collect any pending client actions and return them in the response.
                // Implementation note: in future, we might want to stream client actions directly to the client through a websocket or similar, instead of buffering them up
                // and retrieving them here and bundling them into the response.
                ordered_json clientActionsArray = ordered_json::array();
                for (const auto& clientAction : document->getClientActions()) {
                    ordered_json clientActionJson;
                    clientActionJson["type"] = clientAction.type;
                    clientActionJson["payload"] = clientAction.payload;
                    clientActionsArray.push_back(clientActionJson.dump());
                }
                response = {"OK", "", clientActionsArray};
                document->clearClientActions();
            }
            else if (result == ActionResult::UNKNOWN_ACTION) {
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
        bool runOnce(Document* document, std::istream& input, std::ostream& output) {
            // Read input and process it. getLine will block and wait for input if there is no input available.
            // We always acknowledge the input on the output stream because clients may be blocking, awaiting a response.
            // Returns false if EOF is reached, true otherwise.

            ActionSpec action;
            ActionResponse response = {"OK", ""};
            bool result = true;

            std::string line;
            if (!std::getline(input, line)) {
                response = {"OK", "EOF"};
                result = false;
            }
            else if (line.empty() || std::all_of(line.begin(), line.end(), ::isspace))  {
                // blank line is not an error, we just acknowledge it
                response = {"OK", "Blank line"};
            }
            else if (!parseAction(line, action, response)) {
                // Bad input. parseAction sets the response
            }
            else if (!dispatchAction(document, action, response)) {
                // Bad input or internal error. dispatchAction sets the response.
            }
            else {
                // Success! dispatchAction sets the response
            }

            // package up the response as JSON and stream it to the output stream
            ordered_json jsonResponse = response;
            output << jsonResponse << std::endl;
            return result;
        }   

        /** This runs the document service in a loop until EOF is reached on the input stream.
         * It reads one line at a time from the specified input stream (stdin by default) and processes it. 
         * The line is expected to consist of a JSON string representing an action.
         * The format for an action is: {"type":<string>, "payload":<any valid JSON>}
         * Responses are written to the specified output stream (stdout by default). 
         * Errors, logs and debug are written to stderr.
         */
        void run(Document* document, std::istream& input, std::ostream& output) {
            while (true) {
                if (!runOnce(document, input, output)) {
                    break;
                }
            }
        }

    };
};  