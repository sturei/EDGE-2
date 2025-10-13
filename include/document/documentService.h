#pragma once
#include <map>
#include <string>
#include <iostream>
#include "document/document.h"

/**
 * The DocumentService class enables interprocess communication with a live Document
 * External clients send actions to the DocumentService via stdin, and receive responses via stdout. Errors and logs are sent to stderr.
 * For testing, actions can be sent from a string, and responses received in a string.
 * Implementation notes:
 * There is expected to be a main() program that creates a DocumentService instance, passing it a Document instance, and then calls run() on the service instance.
 * The run() method reads actions from stdin, dispatches them to the Document, and writes responses to stdout.
 * The action format is JSON.
 * Other IPC mechanisms could be supported in future, e.g. files, sockets, shared memory etc.
 * The DocumentService class takes ownership of the Document it is given.
 */

namespace e2 {
    class DocumentService {
        public:
            DocumentService(Document* document) : m_document(document) {}
            ~DocumentService() {
                delete m_document;
            }
            void run(std::ostream& outputStream = std::cout, std::istream& inputStream = std::cin, std::ostream& errorStream = std::cerr) {
                // reads from m_inputStream, writes to m_outputStream and m_errorStream
                std::string line;
                while (std::getline(inputStream, line)) {
                    // For now, just echo the input line to output stream
                    outputStream << "Received action: " << line << std::endl;
                    // In future, parse the action, dispatch it to the document, and write the response
                }
            }
        private:
            Document* m_document;
    };
};  