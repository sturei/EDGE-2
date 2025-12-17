#include "document/document.h"
#include "document/documentService.h"
#include "document/document.actions.h"
#include "shape/shapeModel.h"
#include "shape/shape.actions.h"
#include "scene/sceneActions.h"
#include <fstream>

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
    // std::unique_ptr could be used instead of raw pointers to enforce ownership. Maybe later.

    // parse arguments
    std::istream* input = &std::cin;
    std::ostream* output = &std::cout;

    if (argc > 1) {
        const char* inputFile = argv[1];
        input = new std::ifstream(inputFile);
    }
    if (argc > 2) {
        const char* outputFile = argv[2];
        output = new std::ofstream(outputFile);
    }

    // Initialize the models
    BRepModel* sketches = new BRepModel();
    BRepModel* profiles = new BRepModel();
    FeatureModel* features = new FeatureModel();
    ShapeModel* shapeModel = new ShapeModel(sketches, profiles, features);    // model takes ownership of its sub-models

    // Initialize the stores
    Store* shapeStore = new Store(shapeModel);                     // store takes ownership of the model

    // Initialise the document
    Document* document = new Document({{"shape", shapeStore}});    // document takes ownership of the store

    // Register action functions
    for(auto& action: e2::DocumentActions::allDocumentActions) {
        document->registerActionFunction(action);
    }
    for (auto& action : e2::ShapeActions::allShapeActions) {
        document->registerActionFunction(action);
    }

    // Register post-state-change callbacks
    shapeStore->setPostStateChangeCallback([document]() {
        // This callback is called whenever the shape store's model is modified.
        // It ensures that the graphics scene in the client is updated to reflect the changes in the model.
        dispatchGraphicsActionsForModifiedScene(*document);
    });

    // Run the DocumentService loop forever. This communicates with other processes via stdin and stdout (or the specified input/output streams).
    DocumentService::run(*document, *input, *output);

    // Clean up and exit
    delete document;
    return 0;
}
    