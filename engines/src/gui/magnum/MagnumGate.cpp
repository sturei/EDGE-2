

#include <Corrade/Containers/Optional.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Primitives/Line.h>
#include <Magnum/Primitives/Plane.h>
#include <Magnum/Primitives/UVSphere.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/Shaders/FlatGL.h>
#include <Magnum/Trade/MeshData.h>

#include "gui/magnum/MagnumGate.h"
#include "gui/guiModel.h"
#include "gui/gitem.h"

namespace Magnum { namespace Examples { namespace MagnumGate {

    using Object3D = SceneGraph::Object<SceneGraph::MatrixTransformation3D>;
    using Scene3D = SceneGraph::Scene<SceneGraph::MatrixTransformation3D>;
    using namespace Math::Literals;

    class PhongColoredDrawable: public SceneGraph::Drawable3D {
        public:
            explicit PhongColoredDrawable(
                Object3D& object, Shaders::PhongGL& shader, GL::Mesh& mesh, 
                const Color4& color, SceneGraph::DrawableGroup3D& group): 
                SceneGraph::Drawable3D{object, &group}, _shader(shader), _mesh(mesh), _color{color} {}

        private:
            void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override {
                _shader
                    .setDiffuseColor(_color)
                    .setTransformationMatrix(transformationMatrix)
                    .setNormalMatrix(transformationMatrix.normalMatrix())
                    .setProjectionMatrix(camera.projectionMatrix())
                    .draw(_mesh);
            }   

            Shaders::PhongGL& _shader;    // the application owns the shader
            GL::Mesh& _mesh;              // the handle owns the mesh  
            Color4 _color;                // color is just a value type
    };

    class FlatColoredDrawable: public SceneGraph::Drawable3D {
        public:
            explicit FlatColoredDrawable(
                Object3D& object, Shaders::FlatGL3D& shader, GL::Mesh& mesh, 
                const Color4& color, SceneGraph::DrawableGroup3D& group): 
                SceneGraph::Drawable3D{object, &group}, _shader(shader), _mesh(mesh), _color{color} {}

        private:
            void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override {
                GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);

                _shader
                    .setColor(_color)
                    .setTransformationProjectionMatrix(camera.projectionMatrix()*transformationMatrix)
                    .draw(_mesh);

                GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
            }   

            Shaders::FlatGL3D& _shader;    // the application owns the shader
            GL::Mesh& _mesh;              // the handle owns the mesh  
            Color4 _color;                // color is just a value type
    };

    struct MagnumHandle : public e2::GItemHandle {
        GL::Mesh mesh{NoCreate};   // the handle owns the mesh. The mesh owns its buffers. 
        Object3D* object = nullptr;  // the scene owns the object. The object owns its features/drawables.
    };

    void updateGPoint(e2::GPoint* gpoint, Scene3D& scene, SceneGraph::DrawableGroup3D& drawables, AppShaders& shaders){
        MagnumHandle* handle = dynamic_cast<MagnumHandle*>(gpoint->handle());
        if (!handle->object) {
            // First time creation of the object and mesh
            // create a sphere radius 1 centered at the origin
            const Trade::MeshData sphere = Primitives::uvSphereSolid(60, 60);
            handle->mesh = MeshTools::compile(sphere);
            handle->object = new Object3D(&scene);
            new PhongColoredDrawable(*handle->object, shaders.phongShader, handle->mesh, 0x0000ffff_rgbaf, drawables);
        }
        // scale the sphere to the dimension of the GPoint
        Vector3 scaleVec = Vector3(gpoint->size(), gpoint->size(), gpoint->size());
        handle->object->scale(scaleVec);
        handle->setNeedsUpdate(false);
    }

    void updateGLine(e2::GLine* gline, Scene3D& scene, SceneGraph::DrawableGroup3D& drawables, AppShaders& shaders){
        MagnumHandle* handle = dynamic_cast<MagnumHandle*>(gline->handle());
        if (!handle->object) {
            // First time creation of the object and mesh
            // create a line segment length 2 centered at origin and aligned with x axis
            const Trade::MeshData line = Primitives::line3D(Vector3(-1.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
            handle->mesh = MeshTools::compile(line);
            handle->object = new Object3D(&scene);
            new FlatColoredDrawable(*handle->object, shaders.flatShader, handle->mesh, 0xffffffff_rgbaf, drawables);
        }
        // scale the line to the dimension of the GLine
        Vector3 scaleVec = Vector3(gline->length()/2, 1.0f, 1.0f);
        handle->object->scale(scaleVec);
        handle->setNeedsUpdate(false);
    }

    void updateGPlane(e2::GPlane* gplane, Scene3D& scene, SceneGraph::DrawableGroup3D& drawables, AppShaders& shaders){
        MagnumHandle* handle = dynamic_cast<MagnumHandle*>(gplane->handle());
        if (!handle->object) {
            // First time creation of the object and mesh
            // create a 2x2 plane centered at origin
            const Trade::MeshData plane = Primitives::planeSolid();
            handle->mesh = MeshTools::compile(plane);
            handle->object = new Object3D(&scene);
            new FlatColoredDrawable(*handle->object, shaders.flatShader, handle->mesh, 0x0000ffff_rgbaf, drawables);
        }
        // scale the plane to the dimension of the GPlane
        Vector3 scaleVec = Vector3(gplane->width()/2, gplane->height()/2, 1.0f);
        handle->object->scale(scaleVec);
        handle->setNeedsUpdate(false);
    }

    void updateGSphere(e2::GSphere* gsphere, Scene3D& scene, SceneGraph::DrawableGroup3D& drawables, AppShaders& shaders){
        MagnumHandle* handle = dynamic_cast<MagnumHandle*>(gsphere->handle());
        if (!handle->object) {
            // First time creation of the object and mesh
            // create a sphere radius 1 centered at origin
            const Trade::MeshData sphere = Primitives::uvSphereSolid(60, 60);
            handle->mesh = MeshTools::compile(sphere);
            handle->object = new Object3D(&scene);
            new PhongColoredDrawable(*handle->object, shaders.phongShader, handle->mesh, 0x0000ffff_rgbaf, drawables);
        }
        // scale the sphere to the dimension of the GSphere
        Vector3 scaleVec = Vector3(gsphere->radius(), gsphere->radius(), gsphere->radius());
        handle->object->scale(scaleVec);
        handle->setNeedsUpdate(false);
    }   


    void updateGBlock(e2::GBlock* gblock, Scene3D& scene, SceneGraph::DrawableGroup3D& drawables, AppShaders& shaders){
        MagnumHandle* handle = dynamic_cast<MagnumHandle*>(gblock->handle());
        if (!handle->object) {
            // First time creation of the object and mesh
            // create a 2x2x2 cube centered at origin
            const Trade::MeshData cube = Primitives::cubeSolid();
            handle->mesh = MeshTools::compile(cube);
            handle->object = new Object3D(&scene);
            new PhongColoredDrawable(*handle->object, shaders.phongShader, handle->mesh, 0x0000ffff_rgbaf, drawables);
        }
        // scale the cube to the dimension of the GBlock
        Vector3 scaleVec = Vector3(gblock->width()/2, gblock->height()/2, gblock->depth()/2);
        handle->object->scale(scaleVec);
        handle->setNeedsUpdate(false);
    }   

    void updateGItem(e2::GItem* item, Scene3D& scene, SceneGraph::DrawableGroup3D& drawables, AppShaders& shaders) { 

        // Check if it needs updating. Handle will be marked as needing update if the GItem has changed since the last update
        if (!item->handle()) {
            // first time creation of the handle
            MagnumHandle* handle = new MagnumHandle();
            item->setHandle(handle);
        }
        else if (!item->handle()->needsUpdate()) {
            // no need to update
            return;
        }

        // Update the scene based on the type of GItem
        if (auto gpoint = dynamic_cast<e2::GPoint*>(item)) {
            std::cerr << "  Updating from " << *gpoint << std::endl;
            updateGPoint(gpoint, scene, drawables, shaders);
        }
        else if (auto gline = dynamic_cast<e2::GLine*>(item)) {
            std::cerr << "  Updating from " << *gline << std::endl;
            updateGLine(gline, scene, drawables, shaders);
        }
        else if (auto gplane = dynamic_cast<e2::GPlane*>(item)) {
            std::cerr << "  Updating from " << *gplane << std::endl;
            updateGPlane(gplane, scene, drawables, shaders);
        }
        else if (auto gsphere = dynamic_cast<e2::GSphere*>(item)) {
            std::cerr << "  Updating from " << *gsphere << std::endl;
            updateGSphere(gsphere, scene, drawables, shaders);
        }
        else if (auto gblock = dynamic_cast<e2::GBlock*>(item)) {
            std::cerr << "  Updating from " << *gblock << std::endl;
            updateGBlock(gblock, scene, drawables, shaders);
        }

    }

    void updateSceneFromGuiModel(e2::GuiModel* guiModel, Scene3D& scene, SceneGraph::DrawableGroup3D& drawables, AppShaders& shaders) {
        std::cerr << "Updating scene from GuiModel with " << guiModel->numGItems() << " GItems." << std::endl;
        for (size_t i = 0; i < guiModel->numGItems(); ++i) {
            updateGItem(guiModel->gItem(i), scene, drawables, shaders);
        }
    }
}}}
