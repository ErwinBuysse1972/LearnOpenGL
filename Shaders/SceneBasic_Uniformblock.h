#ifndef SCENEBASIC_UNIFORMBLOCK_H
#define SCENEBASIC_UNIFORMBLOCK_H

#include "scene.h"

#include <glad/glad.h>
#include "glslprogram.h"
#include <glm/glm.hpp>

#include <cfunctracer.h>

class SceneBasic_UniformBlock : public Scene
{
private:
    GLuint m_vaoHandle;
    GLSLProgram m_prog;
    float m_angle;

    void compile();
    void initUniformBlockBuffer();

public:
    SceneBasic_UniformBlock(std::shared_ptr<CTracer> tracer);

    void initScene();
    void update(float t);
    void render();
    void renderGuiWindow(ImGuiIO* io) override {};
    void renderOtherGuiWindow(ImGuiIO* io)override {};

    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORMBLOCK_H