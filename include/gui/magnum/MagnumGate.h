#pragma once
#include <Magnum/SceneGraph/Scene.h>
#include "gui/guiModel.h"

namespace Magnum { namespace Examples { namespace MagnumGate {
    void updateSceneFromGuiModel(
        e2::GuiModel* guiModel, 
        SceneGraph::Scene<SceneGraph::MatrixTransformation3D>& scene, 
        SceneGraph::DrawableGroup3D& drawables, 
        Shaders::PhongGL& shader);   
}}}
