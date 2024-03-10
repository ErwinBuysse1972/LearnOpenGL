#ifndef SCENE_TWO_SIDE_H
#define SCENE_TWO_SIDE_H

#include "scene.h"

#include <glad/glad.h>
#include "glslprogram.h"
#include <glm/glm.hpp>
#include "Helper/teapot.h"

#include <cfunctracer.h>

class SceneTwoSite : public Scene
{
private:
    GLSLProgram m_prog;
    Teapot m_teapot;
    float m_angle;

    void SetMatrices(void);
    void CompileAndLinkShader(void);

public:
    SceneTwoSite(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override {};
    void renderOtherGuiWindow(ImGuiIO* io)override {};

    void resize(int w, int h) override;
};

#endif //SCENE_TWO_SIDE_H
