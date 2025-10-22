#include "document/documentService.h"
#include "document/document.h"
#include "document/store.h"
#include "brep/brepModel.h"
#include "brep/body.actions.h"

using namespace e2;

/**
 * This is the main program for the edge2 modelling service
 * It is intended to provide services for creating and editing 2D and 3D models to clients such as a GUI, a web server, a scripting environment etc.
 * Clients interact with the document service via stdin and stdout.
 * The document service reads actions from stdin, dispatches them to the document, and writes responses to stdout. Errors and debug information are written to stderr.
 * The action format is JSON.
 */


int main(int argc, char* argv[]) {

    // Initialize the document
    Model* brepModel = new BRepModel();                           // owns an initially empty collection of bodies
    Store* brepStore = new Store(brepModel);                      // store takes ownership of the model
    Document* document = new Document({{"brep", brepStore}});     // document takes ownership of the store

    // Register action functions with the document
    document->registerActionFunction(e2::BodyActions::addEmptyBodyDef);

    // Run the DocumentService, which communicates with clients via stdin and stdout
    DocumentService::run(document);

    delete document;
    return 0;
}