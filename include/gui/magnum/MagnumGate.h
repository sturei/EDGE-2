#pragma once
#include <Magnum/SceneGraph/Scene.h>
#include "gui/guiModel.h"

namespace Magnum { namespace Examples { namespace MagnumGate {

    struct AppShaders{
        Shaders::PhongGL& phongShader;
        Shaders::FlatGL3D& flatShader;
    };

    void updateSceneFromGuiModel(
        e2::GuiModel* guiModel, 
        SceneGraph::Scene<SceneGraph::MatrixTransformation3D>& scene, 
        SceneGraph::DrawableGroup3D& drawables, 
        AppShaders& shaders);
}}}
