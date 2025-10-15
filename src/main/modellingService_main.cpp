#include "document/documentService.h"
#include "document/document.h"
#include "document/store.h"
#include "brep/brepModel.h"
#include "brep/body.actions.h"

using namespace e2;

/**
 * This is the main program for the edge2 modelling service. (e2_modellingService)
 * It is intended to provide services for creating and editing 2D and 3D models to clients such as a GUI, a web server, a scripting environment etc.
 * Clients interact with the document service via stdin and stdout.
 * The document service reads actions from stdin, dispatches them to the document, and writes responses to stdout. Errors are written to stderr.
 * The action format is JSON.
 */


int main(int argc, char* argv[]) {

    // Create the initial document and its stores
    Model* brepModel = new BRepModel();
    Store* brepStore = new Store(brepModel);                      // store takes ownership of the model
    Document* document = new Document({{"brep", brepStore}});     // document takes ownership of the store

    // Register action functions with the document
    document->registerActionFunction(e2::BodyActions::addEmptyBodyDef);

    // Run the DocumentService, which by default reads from stdin and writes to stdout and stderr
    DocumentService::run(document);

    // TODO: error handling
    delete document;
    return 0;
}