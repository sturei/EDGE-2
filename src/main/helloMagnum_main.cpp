#include <Corrade/Containers/StringView.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Angle.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/Shaders/VertexColorGL.h>
#include <Magnum/Trade/MeshData.h>

using namespace Magnum::Math::Literals;

// Implementation notes:
// Except for Magnum's user-defined literals, chose to be explicit about Magnum namespaces to avoid confusion.
// Switched Intellisense to use the tag parser. The new parser doesn't work very well with namespaces.
// TODO: remove using declarations from header files (once headers and code are more stable and separated)

class MyApplication: public Magnum::Platform::Sdl2Application {
    public:
        explicit MyApplication(const Arguments& arguments);

    private:
        void drawEvent() override;
        void pointerReleaseEvent(PointerEvent& event) override;
        void pointerMoveEvent(PointerMoveEvent& event) override;

        Magnum::GL::Mesh _triangleMesh;

        Magnum::Shaders::VertexColorGL2D _flatShader;
        Magnum::Shaders::PhongGL _phongShader;

        Magnum::GL::Mesh _cubeMesh;
        Magnum::Matrix4 _cubeTransformation, _cubeProjection;
        Magnum::Color3 _cubeColor;
};

MyApplication::MyApplication(const Arguments& arguments): Sdl2Application(arguments, 
    Configuration{}.setTitle("Hello Magnum!")) {

    // "Getting started" code - combination of various snippets from Magnum examples: https://doc.magnum.graphics/magnum/getting-started.html#getting-started-run

    // Set clear color to light blue
    using namespace Magnum::Math::Literals;
    Magnum::GL::Renderer::setClearColor(0xa5c9ea_rgbf);

    // Enable depth test and backface culling
    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);

    // Print out some debug info
    Magnum::Debug{} << "Hello! This application is running on"
            << Magnum::GL::Context::current().version() << "using"
            << Magnum::GL::Context::current().rendererString();

    // Set up a simple triangle mesh with vertex colors
    struct TriangleVertex {
        Magnum::Vector2 position;
        Magnum::Color3 color;
    };
    const TriangleVertex vertices[]{
        {{-0.05f, -0.05f}, 0xff0000_rgbf},    /* Left vertex, red color */
        {{ 0.05f, -0.05f}, 0x00ff00_rgbf},    /* Right vertex, green color */
        {{ 0.0f,  0.05f}, 0x0000ff_rgbf}     /* Top vertex, blue color */
    };

    _triangleMesh.setCount(Magnum::Containers::arraySize(vertices))
         .addVertexBuffer(Magnum::GL::Buffer{vertices}, 0,
            Magnum::Shaders::VertexColorGL2D::Position{},
            Magnum::Shaders::VertexColorGL2D::Color3{});    // Add vertex buffer to mesh

    // Set up a simple cube mesh and some initial properties for displaying it
    _cubeMesh = Magnum::MeshTools::compile(Magnum::Primitives::cubeSolid());
    _cubeTransformation =
        Magnum::Matrix4::rotationX(30.0_degf)*Magnum::Matrix4::rotationY(40.0_degf);
    _cubeProjection =
        Magnum::Matrix4::perspectiveProjection(
            35.0_degf, Magnum::Vector2{Magnum::Platform::Sdl2Application::windowSize()}.aspectRatio(), 0.01f, 100.0f)*
        Magnum::Matrix4::translation(Magnum::Vector3::zAxis(-10.0f));
    _cubeColor = Magnum::Color3::fromHsv({35.0_degf, 1.0f, 1.0f});

}

void MyApplication::drawEvent() {
    Magnum::GL::defaultFramebuffer.clear(Magnum::GL::FramebufferClear::Color|Magnum::GL::FramebufferClear::Depth);

     _flatShader.draw(_triangleMesh);

     _phongShader.setLightPositions({{1.4f, 1.0f, 0.75f, 0.0f}})
        .setDiffuseColor(_cubeColor)
        .setAmbientColor(Magnum::Color3::fromHsv({_cubeColor.hue(), 1.0f, 0.3f}))
        .setTransformationMatrix(_cubeTransformation)
        .setNormalMatrix(_cubeTransformation.normalMatrix())
        .setProjectionMatrix(_cubeProjection)
        .draw(_cubeMesh);

    swapBuffers();
}

void MyApplication::pointerReleaseEvent(Magnum::Platform::Sdl2Application::PointerEvent& event) {
    if(!event.isPrimary() ||
       !(event.pointer() & (Magnum::Platform::Sdl2Application::Pointer::MouseLeft|Magnum::Platform::Sdl2Application::Pointer::Finger)))
        return;

    _cubeColor = Magnum::Color3::fromHsv({_cubeColor.hue() + 50.0_degf, 1.0f, 1.0f});

    event.setAccepted();
    redraw();
}

void MyApplication::pointerMoveEvent(Magnum::Platform::Sdl2Application::PointerMoveEvent& event) {
    if(!event.isPrimary() ||
       !(event.pointers() & (Magnum::Platform::Sdl2Application::Pointer::MouseLeft|Magnum::Platform::Sdl2Application::Pointer::Finger)))
        return;

    Magnum::Vector2 delta = 3.0f*Magnum::Vector2{event.relativePosition()}/Magnum::Vector2{windowSize()};

    _cubeTransformation =
        Magnum::Matrix4::rotationX(Magnum::Rad{delta.y()})*
        _cubeTransformation*
        Magnum::Matrix4::rotationY(Magnum::Rad{delta.x()});

    event.setAccepted();
    redraw();
}

//MAGNUM_SDL2APPLICATION_MAIN(MyApplication) // Using my own main function instead of this macro for transparency
int main(int argc, char** argv) {

    MyApplication app({argc, argv});   // ToDo : pass configuration options here

    // exec calls mainLoopIteration (which does one iteration) repeatedly until the application is closed. 
    // (Mostly the iterations do nothing because the redraw flag is not set.)
    // mainLoopIteration can also be called manually for more control. So I guess you could do redraw(), followed by mainLoopIteration() in an action function. 
    // See https://github.com/mosra/magnum-bindings/issues/6

    int ret = app.exec();
    return ret;

}
