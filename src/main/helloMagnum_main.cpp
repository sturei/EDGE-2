#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>

////
// For the "add your initialization code here" part
#include <Corrade/Containers/StringView.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Color.h>
//
/////

using namespace Magnum;

// Implementation notes:
// I had to mess around a bit with explicit namespacing to get avoid wiggly lines in VSCode.
// Actually this was causing errors often, so I switched Intellisense to use the tag parser only.


class MyApplication: public Magnum::Platform::Sdl2Application {
    public:
        explicit MyApplication(const Arguments& arguments);

    private:
        void drawEvent() override;
};

MyApplication::MyApplication(const Arguments& arguments): Sdl2Application(arguments) {
    /* TODO: Add your initialization code here */

    /////
    // Here it is. It makes the background a nice light blue color.

    using namespace Magnum::Math::Literals;

    Magnum::GL::Renderer::setClearColor(0xa5c9ea_rgbf);

    Debug{} << "Hello! This application is running on"
            << GL::Context::current().version() << "using"
            << GL::Context::current().rendererString();

    //
    /////
}

void MyApplication::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

    /* TODO: Add your drawing code here */

    swapBuffers();
}

//MAGNUM_SDL2APPLICATION_MAIN(MyApplication) // Using my own main function instead of this macro for transparency
int main(int argc, char** argv) {

    MyApplication app({argc, argv});
    int ret = app.exec();
    return ret;

}
