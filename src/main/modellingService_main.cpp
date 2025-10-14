#include "document/documentService.h"
#include "document/document.h"
#include "document/store.h"
#include "document/model.h"

using namespace e2;

/**
 * This is the main program for the edge2 modelling service. (e2_modellingService)
 * It is intended to provide services for creating and editing 2D and 3D models to clients such as a GUI, a web server, a scripting environment etc.
 * Clients interact with the document service via stdin and stdout.
 * The document service reads actions from stdin, dispatches them to the document, and writes responses to stdout.
 * Errors and logs are written to stderr.
 * The action format is JSON.
 */


int main(int argc, char* argv[]) {

    Model* brepModel = new BRepModel();
    Store* brepStore = new Store(brepModel);            // store takes ownership of the model
    Document* document = new Document({{"brep", brepStore}});     // document takes ownership of the store

    // TODO: document->registerActionFunction("addAnimal", DocumentServiceTestActions::addAnimal);

    DocumentService docService(document);     // documentService takes ownership of the document
    
    // TODO: error handling

    // Run the DocumentService, reading from stdin and writing to stdout and stderr
    docService.run(std::cout, std::cin, std::cerr);

    return 0;
}