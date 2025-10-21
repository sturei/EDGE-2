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

        void runOnce(Document* document) {

            // reads from stdin, writes to stdout and stderr

            const size_t len = 1;
            const int timeoutMillis = 0;    // 0 = non-blocking
            pollfd cinfd[len];
            cinfd[0].fd = fileno(stdin);
            cinfd[0].events = POLLIN;
            // to avoid blocking, only read the line if it is available. See http://www.coldestgame.com/site/blog/cybertron/non-blocking-reading-stdin-c
            if (poll(cinfd, len, timeoutMillis))
            {
                // Input is available. Read and process it.
                // Always acknowledge the input on stdout, even if it is invalid. Clients may be waiting for a response.
                std::string line;
                if (!std::getline(std::cin, line)) {
                    std::cout << "ACK: invalid stream or EOF" << std::endl;
                    return;
                }
                if (line.empty()) {
                    std::cout << "ACK: empty line" << std::endl;
                    return;
                }
                // std::cerr << "Received input: [" << line << "]" << std::endl;        //--- IGNORE ---
                Document::ActionSpec action;
                if (!parseAction(line, action)) {
                    std::cout << "ACK: parse error" << std::endl;
                    return; 
                }
                if (!dispatchAction(document, action)) {
                    std::cout << "ACK: dispatch error" << std::endl;
                    return;
                }
                std::cout << "ACK: success" << std::endl;
            }
        }   

        void run(Document* document, std::istream& inputStream = std::cin, std::ostream& outputStream = std::cout,std::ostream& errorStream = std::cerr) {
            // reads from m_inputStream, writes to m_outputStream and m_errorStream
            std::string line;
            while (std::getline(inputStream, line)) {                   // TODO: some non-blocking way to check for input?

                //errorStream << "Received input: " << line << std::endl;

                if (line.empty()) {
                    continue; // skip empty lines
                }

                // TODO: add error handling, logging etc

                // parse the input line. Each line is expected to be a complete JSON action.
                auto jsonAction = json::parse(line);
                Document::ActionSpec action { jsonAction.at("type"), jsonAction.at("payload")};

                // dispatch the action to the document
                if (!document->dispatchAction(action)) {
                    //errorStream << "Unknown action type: " << action.type << std::endl;
                }

                //errorStream << "***" << *document << std::endl;

                // Give a single-line response acknowledging that the action has been processed. 
                // Later this may becomes more sophisticated, e.g. return a success/failure with maybe an action to getLastError or similar.
                outputStream << "Action [" << action.type << "] processed" << std::endl;
            }
        }
    };
};  