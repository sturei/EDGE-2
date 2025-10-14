#pragma once
#include <map>
#include <string>
#include <iostream>
#include "document/document.h"

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
        void run(Document* document, std::istream& inputStream = std::cin, std::ostream& outputStream = std::cout,std::ostream& errorStream = std::cerr) {
            // reads from m_inputStream, writes to m_outputStream and m_errorStream
            std::string line;
            while (std::getline(inputStream, line)) {

                // errorStream << "Received input: " << line << std::endl;

                if (line.empty()) {
                    continue; // skip empty lines
                }

                // TODO: add error handling, logging etc

                // parse the input line. Each line is expected to be a complete JSON action.
                auto jsonAction = json::parse(line);
                ActionSpec action { jsonAction.at("type"), jsonAction.at("payload")};

                // dispatch the action to the document
                document->dispatchAction(action);

                // For now, output the document to the output stream. Later, output a more structured response.
                outputStream << *document << std::endl;
            }
        }
    };
};  