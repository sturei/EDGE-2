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

        /** This method takes a JSON string as input, and converts it a json object representing an action. 
         *  The format for an action is: {"type":<string>, "payload":<any valid JSON>}
         *  Returns true if successful, false otherwise.
        */
        bool parseAction(const std::string& line, Document::ActionSpec& action) {
            try
            {
                json jsonAction = json::parse(line);
                //std::cerr << "Parsed input to json: " << jsonAction.dump() << std::endl;    //--- IGNORE ---
                action.type = jsonAction.at("type");
                action.payload = jsonAction.at("payload");
            }
            catch (const json::parse_error& e)
            {
                //std::cerr << "Error parsing json at byte " << e.byte << std::endl;    //--- IGNORE ---
                return false;
            }
            catch (const json::out_of_range& e)
            {
                //std::cerr << "Error unpacking json" << std::endl;  //--- IGNORE ---
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

        /** In blocking mode, this method waits for input on stdin, processes it and dispatches it to the document as an action.
        *  In non-blocking mode, it only processes input it it is available, Otherwise it does nothing.
        */
        void runOnce(Document* document, bool blocking, std::istream& input, std::ostream& output) {
            // NOTE: this method uses stdin directly, rather than the input and output streams passed as parameters, 
            // because it uses poll() to check for input availability.
            // For testing purposes only, clients may pass in stringstreams for input and output, but that only works in blocking mode.

            bool inputIsAvailable = false;
            if (!blocking){
                // To avoid blocking, we only read the line if it is available. See http://www.coldestgame.com/site/blog/cybertron/non-blocking-reading-stdin-c
                // Unfortunately this code mixes C and C++ style I/O. I tried cin.peek() and cin.rdbuf()->in_avail() but they didn't work for this purpose.
                const size_t len = 1;
                const int timeoutMillis = 0;    // 0 = non-blocking
                pollfd cinfd[len];
                cinfd[0].fd = fileno(stdin);
                cinfd[0].events = POLLIN;
                if (poll(cinfd, len, timeoutMillis)) {
                    inputIsAvailable = true;
                }
            }

            if (blocking || inputIsAvailable)
            {
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
            // std::cerr << "No input available" << std::endl;        //--- IGNORE ---
        }   

        /** This method runs the document service in a loop, processing input and dispatching actions. 
         * Right now it really works in blocking mode, otherwise the loop will just whizz round and round. Later, perhaps, a sleep interval 
         * could be added to make non-blockin mode work too. 
         */
        void run(Document* document, bool blocking, std::istream& input, std::ostream& output) {
            // implementation note: the assumption is that this is used in case of a dedicated service process, so we can block waiting for input
            // alternatively, could have a non-blocking mode and a sleep interval, or similar.
            while (true) {
                runOnce(document, blocking, input, output);
            }
        }
    };
};  