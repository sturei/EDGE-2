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

        /** This method converts a JSON string to a json object representing an action.
         *  The format for an action is: {"type":<string>, "payload":<any valid JSON>}
         *  Returns true if successful, false otherwise.
        */
        bool parseAction(const std::string& line, Document::ActionSpec& action) {
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
                return false;
            }
            return true;
        }

        /** This method dispatches the specified action to the document. TODO: catch errors, log them and continue. */
        bool dispatchAction(Document* document, const Document::ActionSpec& action) {
            if (!document->dispatchAction(action)) {
                //std::cerr << "Failed to dispatch action. Is \"" << action.type << "\" action registered?" << std::endl;  //--- IGNORE ---
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
            // Always acknowledge the input on stdout, even if it is invalid. Clients may be waiting for a response.
            std::string line;
            if (!std::getline(input, line)) {
                output << "ACK: invalid stream or EOF" << std::endl;
                return;
            }
            if (line.empty()) {
                output << "ACK: empty line" << std::endl;
                return;
            }
            // std::cerr << "Received input: [" << line << "]" << std::endl;        //--- IGNORE ---
            Document::ActionSpec action;
            if (!parseAction(line, action)) {
                output << "ACK: parse error" << std::endl;
                return; 
            }
            if (!dispatchAction(document, action)) {
                output << "ACK: dispatch error" << std::endl;
                return;
            }
            output << "ACK: success" << std::endl;
                return;
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
         * Responses are written to the specified output stream (stdout by default). Errors and logs are written to stderr.
         */
        void run(Document* document, std::istream& input, std::ostream& output) {
            while (true) {
                runOnce(document, input, output);
            }
        }

    };
};  