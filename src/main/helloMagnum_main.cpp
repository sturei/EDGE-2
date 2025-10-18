#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>

////
// For the "add your xxx code here" parts
#include <Corrade/Containers/StringView.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Color.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/VertexColorGL.h>
//
/////

// Implementation notes:
// I had to mess around a bit with explicit namespacing to get avoid wiggly lines in VSCode.
// Actually this was causing errors often, so I switched Intellisense to use the tag parser only.
// TODO: remove using declarations from header files (once headers and code are more stable and separated)
// And in the end, don't use Magnum namespace at all - it's clearer to see what is from Magnum and what is fron elsewhere.

class MyApplication: public Magnum::Platform::Sdl2Application {
    public:
        explicit MyApplication(const Arguments& arguments);

    private:
        void drawEvent() override;
        Magnum::GL::Mesh _mesh;
        Magnum::Shaders::VertexColorGL2D _shader;
};

MyApplication::MyApplication(const Arguments& arguments): Sdl2Application(arguments, 
    Configuration{}.setTitle("Hello Magnum!")) {
    /* TODO: Add your initialization code here */

    // "Getting started" code, from Magnum examples: https://doc.magnum.graphics/magnum/getting-started.html#getting-started-run

    // Set clear color to light blue
    using namespace Magnum::Math::Literals;
    Magnum::GL::Renderer::setClearColor(0xa5c9ea_rgbf);

    // Print out some debug info
    Magnum::Debug{} << "Hello! This application is running on"
            << Magnum::GL::Context::current().version() << "using"
            << Magnum::GL::Context::current().rendererString();

    // Set up a simple triangle mesh with vertex colors
    using namespace Magnum::Math::Literals;

    struct TriangleVertex {
        Magnum::Vector2 position;
        Magnum::Color3 color;
    };
    const TriangleVertex vertices[]{
        {{-0.5f, -0.5f}, 0xff0000_rgbf},    /* Left vertex, red color */
        {{ 0.5f, -0.5f}, 0x00ff00_rgbf},    /* Right vertex, green color */
        {{ 0.0f,  0.5f}, 0x0000ff_rgbf}     /* Top vertex, blue color */
    };

    _mesh.setCount(Magnum::Containers::arraySize(vertices))
         .addVertexBuffer(Magnum::GL::Buffer{vertices}, 0,
            Magnum::Shaders::VertexColorGL2D::Position{},
            Magnum::Shaders::VertexColorGL2D::Color3{});    // Add vertex buffer to mesh

}

void MyApplication::drawEvent() {
    Magnum::GL::defaultFramebuffer.clear(Magnum::GL::FramebufferClear::Color);

    /* TODO: Add your drawing code here */

    // "Your first triangle" code, from Magnum examples: https://doc.magnum.graphics/magnum/examples-triangle.html

     _shader.draw(_mesh);

    swapBuffers();
}

//MAGNUM_SDL2APPLICATION_MAIN(MyApplication) // Using my own main function instead of this macro for transparency
int main(int argc, char** argv) {

    MyApplication app({argc, argv});   // ToDo : pass configuration options here
    int ret = app.exec();
    return ret;

}
