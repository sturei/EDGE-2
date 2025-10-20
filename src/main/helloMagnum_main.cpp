#include <Corrade/Containers/StringView.h>

#include <Magnum/DebugTools/ColorMap.h>

#include <Magnum/GL/Context.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>


#include <Magnum/ImageView.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>

#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Math/Angle.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Time.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/Shaders/VertexColorGL.h>
#include <Magnum/Shaders/MeshVisualizerGL.h>
#include <Magnum/Trade/MeshData.h>

#include "gui/helloMagnum/ArcBall.h"
#include "gui/helloMagnum/ArcBallCamera.h"

using namespace Magnum;
using Object3D = SceneGraph::Object<SceneGraph::MatrixTransformation3D>;
using Scene3D = SceneGraph::Scene<SceneGraph::MatrixTransformation3D>;
using namespace Math::Literals;


// Implementation notes:
// Except for Magnum's user-defined literals, chose to be explicit about Magnum namespaces to avoid confusion.
// Probably a better idea is to put the Magnum Application class in its own file, and then use 'using namespace Magnum' inside that file.  
// Switched Intellisense to use the tag parser. The new parser doesn't work very well with namespaces.
// TODO: remove using declarations from header files (once headers and code are more stable and separated)

class MyApplication: public Magnum::Platform::Sdl2Application {
    public:
        explicit MyApplication(const Arguments& arguments);

    private:
        void drawEvent() override;
        void viewportEvent(ViewportEvent& event) override;
        void keyPressEvent(KeyEvent& event) override;
        void pointerPressEvent(PointerEvent& event) override;
        void pointerReleaseEvent(PointerEvent& event) override;
        void pointerMoveEvent(PointerMoveEvent& event) override;
        void scrollEvent(ScrollEvent& event) override;

        Magnum::GL::Mesh _triangleMesh;

        Magnum::Shaders::VertexColorGL2D _flatShader;
        Magnum::Shaders::PhongGL _phongShader;

        Magnum::GL::Mesh _cubeMesh;
        Magnum::Matrix4 _cubeTransformation, _cubeProjection;
        Magnum::Color3 _cubeColor;

        Scene3D _scene;
        Magnum::SceneGraph::DrawableGroup3D _drawables;
        Containers::Optional<Magnum::Examples::ArcBallCamera> _arcballCamera;
        GL::Mesh _arcballMesh{NoCreate};

        /* Stuff for visualizing the cube in the arcball example*/
        Shaders::MeshVisualizerGL3D _arcballShader{NoCreate};
        GL::Texture2D _arcballColormap{NoCreate};

};

class VisualizationDrawable: public SceneGraph::Drawable3D {
    public:
        explicit VisualizationDrawable(Object3D& object,
            Shaders::MeshVisualizerGL3D& shader, GL::Mesh& mesh,
            SceneGraph::DrawableGroup3D& drawables):
                SceneGraph::Drawable3D{object, &drawables}, _shader(shader),
                _mesh(mesh) {}

        void draw(const Matrix4& transformation, SceneGraph::Camera3D& camera) {
            _shader
                .setTransformationMatrix(transformation)
                .setProjectionMatrix(camera.projectionMatrix())
                .draw(_mesh);
        }

    private:
        Shaders::MeshVisualizerGL3D& _shader;
        GL::Mesh& _mesh;
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

    /* Setup a cube with vertex ID and wireframe visualized */
    {
        const Trade::MeshData cube = Primitives::cubeSolid();
        _arcballMesh = MeshTools::compile(cube);

        const auto map = DebugTools::ColorMap::turbo();
        const Vector2i size{Int(map.size()), 1};
        _arcballColormap = GL::Texture2D{};
        _arcballColormap
            .setMinificationFilter(SamplerFilter::Linear)
            .setMagnificationFilter(SamplerFilter::Linear)
            .setWrapping(SamplerWrapping::ClampToEdge)
            .setStorage(1, GL::TextureFormat::RGB8, size)
            .setSubImage(0, {}, ImageView2D{PixelFormat::RGB8Unorm, size, map});

        _arcballShader = Shaders::MeshVisualizerGL3D{Shaders::MeshVisualizerGL3D::Configuration{}
            .setFlags(Shaders::MeshVisualizerGL3D::Flag::Wireframe|
                      Shaders::MeshVisualizerGL3D::Flag::VertexId)};
        _arcballShader
            .setViewportSize(Vector2{framebufferSize()})
            .setColor(0xffffff_rgbf)
            .setWireframeColor(0xffffff_rgbf)
            .setWireframeWidth(2.0f)
            .setColorMapTransformation(0.0f, 1.0f/cube.vertexCount())
            .bindColorMapTexture(_arcballColormap);

        auto object = new Object3D{&_scene};
        (*object)
            .rotateY(40.0_degf)
            .rotateX(-30.0_degf)
            ;
        new VisualizationDrawable{*object, _arcballShader, _arcballMesh, _drawables};
    }

    /* Set up the camera */
    /* Setup the arcball after the camera objects */
    const Vector3 eye = Vector3::zAxis(-10.0f);
    const Vector3 center{};
    const Vector3 up = Vector3::yAxis();
    _arcballCamera.emplace(_scene, eye, center, up, 45.0_degf,
        windowSize(), framebufferSize());

    /* Loop at 60 Hz max */
    setSwapInterval(1);
    setMinimalLoopPeriod(16.0_msec);


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

    /* Call arcball update in every frame. This will do nothing if the camera
       has not been changed. Otherwise, camera transformation will be
       propagated into the camera objects. */
    bool camChanged = _arcballCamera->update();
    _arcballCamera->draw(_drawables);
    swapBuffers();

    if(camChanged) redraw();
}

void MyApplication::viewportEvent(ViewportEvent& event) {
    Magnum::GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});

    _arcballCamera->reshape(event.windowSize(), event.framebufferSize());
}

void MyApplication::keyPressEvent(KeyEvent& event) {
    switch(event.key()) {
        case Key::L:
            if(_arcballCamera->lagging() > 0.0f) {
                Debug{} << "Lagging disabled";
                _arcballCamera->setLagging(0.0f);
            } else {
                Debug{} << "Lagging enabled";
                _arcballCamera->setLagging(0.85f);
            }
            break;
        case Key::R:
            _arcballCamera->reset();
            break;

        default: return;
    }

    event.setAccepted();
    redraw(); /* camera has changed, redraw! */
}

void MyApplication::pointerPressEvent(PointerEvent& event) {
    if(!event.isPrimary() ||
       !(event.pointer() & (Pointer::MouseLeft|Pointer::Finger)))
        return;

    /* Enable mouse capture so the mouse can drag outside of the window */
    /** @todo replace once https://github.com/mosra/magnum/pull/419 is in */
    SDL_CaptureMouse(SDL_TRUE);

    _arcballCamera->initTransformation(event.position());

    event.setAccepted();
    redraw(); /* camera has changed, redraw! */
}

void MyApplication::pointerReleaseEvent(Magnum::Platform::Sdl2Application::PointerEvent& event) {
    if(!event.isPrimary() ||
       !(event.pointer() & (Pointer::MouseLeft|Pointer::Finger)))
        return;

    /* Disable mouse capture again */
    /** @todo replace once https://github.com/mosra/magnum/pull/419 is in */
    SDL_CaptureMouse(SDL_FALSE);


    // From anpother example.
    //if(!event.isPrimary() ||
    //   !(event.pointer() & (Magnum::Platform::Sdl2Application::Pointer::MouseLeft|Magnum::Platform::Sdl2Application::Pointer::Finger)))
    //    return;
    //
    //_cubeColor = Magnum::Color3::fromHsv({_cubeColor.hue() + 50.0_degf, 1.0f, 1.0f});

    //event.setAccepted();
    //redraw();
}

void MyApplication::pointerMoveEvent(Magnum::Platform::Sdl2Application::PointerMoveEvent& event) {
    if(!event.isPrimary() ||
       !(event.pointers() & (Pointer::MouseLeft|Pointer::Finger)))
        return;

    if(event.modifiers() & Modifier::Shift)
        _arcballCamera->translate(event.position());
    else _arcballCamera->rotate(event.position());

    event.setAccepted();
    redraw(); /* camera has changed, redraw! */

    // from another example
    //if(!event.isPrimary() ||
    //   !(event.pointers() & (Magnum::Platform::Sdl2Application::Pointer::MouseLeft|Magnum::Platform::Sdl2Application::Pointer::Finger)))
    //    return;
    //
    //Magnum::Vector2 delta = 3.0f*Magnum::Vector2{event.relativePosition()}/Magnum::Vector2{windowSize()};

    //_cubeTransformation =
    //    Magnum::Matrix4::rotationX(Magnum::Rad{delta.y()})*
    //    _cubeTransformation*
    //    Magnum::Matrix4::rotationY(Magnum::Rad{delta.x()});

    //event.setAccepted();
    //redraw();
}

void MyApplication::scrollEvent(ScrollEvent& event) {
    const Float delta = event.offset().y();
    if(Math::abs(delta) < 1.0e-2f) return;

    _arcballCamera->zoom(delta);

    event.setAccepted();
    redraw(); /* camera has changed, redraw! */
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
