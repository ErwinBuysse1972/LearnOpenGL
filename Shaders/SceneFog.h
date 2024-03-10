#ifndef SCENE_FOG_H
#define SCENE_FOG_H

#include "Scene.h"
#include "glslprogram.h"
#include "Plane.h"
#include "teapot.h"
#include "Torus.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <cfunctracer.h>

class SceneFog : public Scene
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
    SceneFog(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override {};
    void renderOtherGuiWindow(ImGuiIO* io)override {};

    void resize(int w, int h) override;

};
#endif // SCENE_FOG_H
