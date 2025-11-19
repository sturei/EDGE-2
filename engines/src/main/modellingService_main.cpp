#include "document/documentService.h"
#include "document/document.h"
#include "document/document.actions.h"
#include "document/store.h"
#include "brep/brepModel.h"
#include "frep/frepModel.h"
#include "brep/brep.actions.h"
#include "frep/frep.actions.h"
#include "shape/shapeModel.h"

using namespace e2;

/**
 * This is the main program for the edge2 modelling service
 * It is intended to provide services for creating and editing 2D and 3D models to clients such as a GUI, a web server, a scripting environment etc.
 * Clients interact with the document service via stdin and stdout.
 * The document service reads actions from stdin, dispatches them to the document, and writes responses to stdout.
 * Errors and debug information are written to stderr.
 * The action format is JSON.
 */


int main(int argc, char* argv[]) {

    // Note on resource management:
    // Document takes ownership of its stores; stores take ownership of their models; models own all of their data. 
    // It's a 'total ownership' resource model, with the document at the top of the hierarchy.
    // std::unique_ptr could be used instead of raw pointers to enforce ownership, but it's a faff.

    // Initialize the models
    BRepModel* brepModel = new BRepModel();                  // an initially empty collection of bodies
    FRepModel* frepModel = new FRepModel();                // an initially empty collection of FObjects
    ShapeModel* shapeModel = new ShapeModel(brepModel, frepModel);

    // Initialise the stores and the document.
    Document* document = new Document({{"shape", new Store(shapeModel)}});    // document takes ownership of the store

    // Register action functions
    document->registerActionFunction(e2::DocumentActions::pingDef);

    document->registerActionFunction(e2::BRepActions::addEmptyBodyDef);
    document->registerActionFunction(e2::BRepActions::addAcornBodyDef);
    document->registerActionFunction(e2::BRepActions::addWireRectangleDef);
    document->registerActionFunction(e2::BRepActions::addSheetRectangleDef);
    document->registerActionFunction(e2::BRepActions::addWireRectangleDef);
    document->registerActionFunction(e2::BRepActions::addSheetRectangleDef);
    document->registerActionFunction(e2::BRepActions::addWireRoundRectDef);
    document->registerActionFunction(e2::BRepActions::addSheetRoundRectDef);

    document->registerActionFunction(e2::FRepActions::addRectangleDef);

    // Run the DocumentService loop forever. This communicates with other processes via stdin and stdout. 
    DocumentService::run(document);

    delete document;   // this deletes the stores and models too (if in future the run() loop ever exits)
    return 0;
}
    