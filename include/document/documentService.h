#pragma once
#include <map>
#include <string>
#include <iostream>
#include <poll.h>
#include <nlohmann/json.hpp>
#include "document/document.h"

using json = nlohmann::json;

/**
 * DocumentService enables interprocess communication with a live Document
 * External clients send actions via stdin, and receive responses via stdout. Errors and logs are sent to stderr.
 * Implementation notes:
 * DocumentService is stateless - it does not own the Document, it just provides an interface to it. The Document holds all the state.
 * The run() method reads actions from stdin, dispatches them to the specified ocument, and writes responses to stdout.
 * The action format is JSON.
 * Other IPC mechanisms could be supported in future, e.g. files, sockets, shared memory etc.
 * TODO: add a logging stream, and maybe a journal stream, separate from stderr.
 */

namespace e2 {
    namespace DocumentService {

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

        bool dispatchAction(Document* document, const Document::ActionSpec& action) {
            if (!document->dispatchAction(action)) {
                //std::cerr << "Failed to dispatch action. Is \"" << action.type << "\" action registered?" << std::endl;  //--- IGNORE ---
                return false;
            }
            return true;
        }

        void runOnce(Document* document, bool blocking = true, std::istream& input = std::cin, std::ostream& output = std::cout) {
            // Reads from stdin if there is anything there to read, writes to stdout.
            // NOTE: this method uses stdin directly, rather than the input and output streams passed as parameters, because of the use of poll() to check for input availability.
            // For testing purposes only, clients can pass in stringstreams for input and output, so long as blocking is true. Sorry!


            bool inputIsAvailable = false;
            if (!blocking){
                // To avoid blocking, we only attempt read the line if it is available. See http://www.coldestgame.com/site/blog/cybertron/non-blocking-reading-stdin-c
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

        void run(Document* document, bool blocking = true, std::istream& input = std::cin, std::ostream& output = std::cout) {
            // implementation note: the assumption is that this is used in case of a dedicated service process, so we can block waiting for input
            // alternatively, could have a non-blocking mode and a sleep interval, or similar.
            while (true) {
                runOnce(document, blocking, input, output);
            }
        }
    };
};  