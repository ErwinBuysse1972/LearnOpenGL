#ifndef SCENE_MULTI_TEXTURE_H
#define SCENE_MULTI_TEXTURE_H

#include "scene.h"

#include <glad/glad.h>
#include "glslprogram.h"
#include <glm/glm.hpp>

#include <Cube.h>

#include <cfunctracer.h>
class SceneMultiTex : public Scene
{
private:
    GLSLProgram m_prog;
    GLuint      m_brick;
    GLuint      m_moss;
    Cube        m_cube;

    void SetMatrices();
    void CompileAndLinkShader();
public:
    SceneMultiTex(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override {};
    void renderOtherGuiWindow(ImGuiIO* io)override {};

    void resize(int w, int h) override;
};
#endif //SCENE_MULTI_TEXTURE_H
