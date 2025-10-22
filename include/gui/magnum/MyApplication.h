/**
 * This code began as a modified ArcBall camera implementation from the Magnum examples. 
 * See below for the original license and authorship information.
 */

/*
    ArcBallExample is part of Magnum.

    Original authors — credit is appreciated but not required:

        2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019,
        2020, 2021, 2022, 2023, 2024, 2025
             — Vladimír Vondruš <mosra@centrum.cz>
        2020 — Nghia Truong <nghiatruong.vn@gmail.com>

    This is free and unencumbered software released into the public domain.

    Anyone is free to copy, modify, publish, use, compile, sell, or distribute
    this software, either in source code form or as a compiled binary, for any
    purpose, commercial or non-commercial, and by any means.

    In jurisdictions that recognize copyright laws, the author or authors of
    this software dedicate any and all copyright interest in the software to
    the public domain. We make this dedication for the benefit of the public
    at large and to the detriment of our heirs and successors. We intend this
    dedication to be an overt act of relinquishment in perpetuity of all
    present and future rights to this software under copyright law.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
    IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <Corrade/Containers/Optional.h>
#include <Corrade/Containers/String.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Shaders/MeshVisualizerGL.h>
#include <Magnum/Shaders/PhongGL.h>

#include "gui/magnum/ArcBallCamera.h"

namespace Magnum { namespace Examples {

class MyApplication: public Platform::Application {
    public:
        explicit MyApplication(const Arguments& arguments, const Containers::StringView& title = Containers::StringView("My Application"));

    protected:

        SceneGraph::Scene<SceneGraph::MatrixTransformation3D>& getScene() { return _scene; }
        SceneGraph::DrawableGroup3D& getDrawables() { return _drawables; }
        Shaders::PhongGL& getColoredShader() { return _coloredShader; }

    private:
        void tickEvent() override;
        void drawEvent() override;
        void viewportEvent(ViewportEvent& event) override;
        void keyPressEvent(KeyEvent& event) override;
        void pointerPressEvent(PointerEvent& event) override;
        void pointerReleaseEvent(PointerEvent& event) override;
        void pointerMoveEvent(PointerMoveEvent& event) override;
        void scrollEvent(ScrollEvent& event) override;

        virtual void doOnTickEvent();

        SceneGraph::Scene<SceneGraph::MatrixTransformation3D> _scene;
        SceneGraph::DrawableGroup3D _drawables;
        GL::Mesh _mesh{NoCreate};
        Containers::Optional<ArcBallCamera> _arcballCamera;

        Shaders::MeshVisualizerGL3D _shader{NoCreate};
        Shaders::PhongGL _coloredShader{NoCreate};

        GL::Texture2D _colormap{NoCreate};
};

}} // namespace Magnum::Examples    