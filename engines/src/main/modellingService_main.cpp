#include "document/document.h"
#include "document/documentService.h"
#include "document/document.actions.h"
#include "frep/frepModel.h"
#include "brep/brepModel.h"
#include "shape/shapeModel.h"
#include "shape/shape.actions.h"

using namespace e2;

/**
 * This is the main program for the EDGE-2 modelling service
 * It provides services for creating and modifying 2D and 3D models, to clients such as a GUI, a web server, a scripting environment etc.
 * Clients send so-called "actions" to stdin of the service; the service responds by updating its internal state and sending a response to the client on stdout.
 * Errors and debug information are written to stderr.
 */


int main(int argc, char* argv[]) {

    // Note on resource management:
    // Document takes ownership of its stores; stores take ownership of their models; models own all of their data. 
    // It's a 'total ownership' resource model, with the document at the top of the hierarchy.
    // std::unique_ptr could be used instead of raw pointers to enforce ownership, but it's a faff. Maybe later.

    // Initialize the models
    BRepModel* sketches = new BRepModel();
    BRepModel* profiles = new BRepModel();
    FRepModel* objects = new FRepModel();
    ShapeModel* shapeModel = new ShapeModel(sketches, profiles, objects);

    // Initialise the stores and the document.
    Document* document = new Document({{"shape", new Store(shapeModel)}});    // document takes ownership of the store

    // Register action functions
    for(auto& action: e2::DocumentActions::allDocumentActions) {
        document->registerActionFunction(action);
    }
    for (auto& action : e2::ShapeActions::allShapeActions) {
        document->registerActionFunction(action);
    }

    // Run the DocumentService loop forever. This communicates with other processes via stdin and stdout. 
    DocumentService::run(document);

    // Clean up and exit
    delete document;
    return 0;
}
    