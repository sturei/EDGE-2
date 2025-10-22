

#include <Corrade/Containers/Optional.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/Trade/MeshData.h>

#include "gui/magnum/MagnumGate.h"
#include "gui/guiModel.h"
#include "gui/gitem.h"

namespace Magnum { namespace Examples { namespace MagnumGate {

    using Object3D = SceneGraph::Object<SceneGraph::MatrixTransformation3D>;
    using Scene3D = SceneGraph::Scene<SceneGraph::MatrixTransformation3D>;

    // A simple drawable that uses the Phong shader to render a colored object
    class ColoredDrawable: public SceneGraph::Drawable3D {
    public:
        explicit ColoredDrawable(
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

    struct MagnumHandle : public e2::GItemHandle {
        GL::Mesh mesh{NoCreate};   // the handle owns the mesh. The mesh owns its buffers. 
        Object3D* object = nullptr;  // the scene owns the object. The object owns its features/drawables.
    };

    void updateGBlock(e2::GBlock* gblock, Scene3D& scene, SceneGraph::DrawableGroup3D& drawables, Shaders::PhongGL& shader){

        MagnumHandle* handle = dynamic_cast<MagnumHandle*>(gblock->handle());

        if (!handle->object) {
            // first time creation of the object
            const Trade::MeshData cube = Primitives::cubeSolid();
            handle->mesh = MeshTools::compile(cube);
            handle->object = new Object3D(&scene);
            new ColoredDrawable(*handle->object, shader, handle->mesh, Color4{1.0f, 0.0f, 0.0f, 0.5f}, drawables);
        }
        // scale the cube to the size of the GBlock
        Vector3 scaleVec = Vector3(gblock->length()/2, gblock->width()/2, gblock->height()/2);
        handle->object->scale(scaleVec);
        handle->setNeedsUpdate(false);
    }   

    void updateGItem(e2::GItem* item, Scene3D& scene, SceneGraph::DrawableGroup3D& drawables, Shaders::PhongGL& shader) { 

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
        if (auto gblock = dynamic_cast<e2::GBlock*>(item)) {
            std::cerr << "  Updating from " << *gblock << std::endl;
            updateGBlock(gblock, scene, drawables, shader);
        }

    }

    void updateSceneFromGuiModel(e2::GuiModel* guiModel, Scene3D& scene, SceneGraph::DrawableGroup3D& drawables, Shaders::PhongGL& shader) {
        std::cerr << "Updating scene from GuiModel with " << guiModel->numGItems() << " GItems." << std::endl;
        for (size_t i = 0; i < guiModel->numGItems(); ++i) {
            updateGItem(guiModel->gItem(i), scene, drawables, shader);
        }
    }
}}}
