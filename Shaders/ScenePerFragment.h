#ifndef SCENE_PER_FRAGMENT_H
#define SCENE_PER_FRAGMENT_H

#include "scene.h"

#include <glad/glad.h>
#include "glslprogram.h"
#include <glm/glm.hpp>
#include "Plane.h"
#include "teapot.h"

#include <cfunctracer.h>

class ScenePerFragment : public Scene
{
private:
    GLSLProgram m_prog;

    float       m_tPrev;
    Plane       m_plane;
    Teapot      m_teapot;
    float       m_angle;

    void SetMatrices();
    void CompileAndLinkShader();

public:
    ScenePerFragment(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override {};
    void renderOtherGuiWindow(ImGuiIO* io)override {};

    void resize(int w, int h) override;
};

#endif //SCENE_PER_FRAGMENT_H
