/**
 * This is the main program for the edge2 desktop app.
 * It uses the Magnum framework for the GUI, and the DocumentService for handling actions input from the console
 */

#include "document/documentService.h"
#include "document/document.h"
#include "document/store.h"

#include "gui/magnum/MyApplication.h"
#include "gui/magnum/MagnumGate.h"
#include "gui/guiModel.h"
#include "gui/gui.actions.h"

using namespace e2;
using namespace Magnum::Examples;

class GuiApplication : public MyApplication {
    public:
        explicit GuiApplication(const Arguments& arguments, const Corrade::Containers::StringView& title) :
            MyApplication{arguments, title} {

            // Initialize the document
            GuiModel* guiModel = new GuiModel();       // owns an initially empty collection of GItems
            Store* guiStore = new Store(guiModel,   // store takes ownership of the model
                [this, guiModel]() {
                // this callback is called after every state change to the GUI model
                this->updateScene(guiModel);
            });

            m_document = new Document({{"gui", guiStore}});     // document takes ownership of the store

            // Register action functions with the document
            m_document->registerActionFunction(e2::GuiActions::pingDef);
            m_document->registerActionFunction(e2::GuiActions::addGBlockDef);
            }
        ~GuiApplication() {
            delete m_document;
        }
    private:
        void doOnTickEvent() override {
            // The main application loop of the Magnum framework calls this method after processing input events 
            // and before drawEvent().
            // We run one loop of the DocumentService to read, process and dispatch any actions received on stdin
            // The call is non-blocking, so if there are no actions to read, it returns immediately.
            DocumentService::runOnce(m_document, false);
        }
        void updateScene(GuiModel* guiModel) {
            // Updates the scene from the GUI model
            auto& scene = getScene();
            auto& drawables = getDrawables();
            auto& shader = getColoredShader();
            MagnumGate::updateSceneFromGuiModel(guiModel, scene, drawables, shader);
            redraw();
        }
    private:
        Document* m_document = nullptr;
};

int main(int argc, char* argv[]) {

    // Create and run the application
    GuiApplication app({argc, argv}, "e2 Desktop Application");
    return app.exec();

}