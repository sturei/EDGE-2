/**
 * This is the main program for the edge2 desktop app.
 * It uses the Magnum framework for the GUI, and the DocumentService for handling actions input from the console
 */

#include "document/documentService.h"
#include "document/document.h"
#include "document/store.h"

#include "gui/magnum/MyApplication.h"
#include "gui/guiModel.h"
#include "gui/gui.actions.h"

using namespace e2;
using namespace Magnum::Examples;

class GuiApplication : public MyApplication {
    public:
        explicit GuiApplication(const Arguments& arguments, const Corrade::Containers::StringView& title) :
            MyApplication{arguments, title} {
            // The GUI model will contain (at least) the drawlist of graphical objects to be rendered.
            Model* guiModel = new GuiModel();

            // The GUI store takes ownership of the model. It has a post-change callback that updates the scenegraph whenever the GUI model changes.
            Store* guiStore = new Store(guiModel, [this, guiModel]() {
                // this->updateSceneFromGuiModel(guiModel);   // TODO
            });

            // The document has just the one store.
            m_document = new Document({{"gui", guiStore}});     // document takes ownership of the store

            // Register action functions with the document
            m_document->registerActionFunction(e2::GuiActions::pingDef);
            }
        ~GuiApplication() {
            delete m_document;
        }
    private:
        void doOnTickEvent() override {
            // run one loop of the DocumentService to read, process and dispatch any actions supplied on stdin
            DocumentService::runOnce(m_document);
        }
        Document* m_document = nullptr;
};

int main(int argc, char* argv[]) {

    // Create and run the application
    GuiApplication app({argc, argv}, "e2 Desktop Application");
    return app.exec();

}