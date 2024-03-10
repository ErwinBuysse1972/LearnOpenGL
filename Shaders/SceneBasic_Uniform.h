#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include <glad/glad.h>
#include "Helper/Scene.h"
#include "glslprogram.h"
#include <glm/glm.hpp>
#include <cfunctracer.h>

class SceneBasic_Uniform : public Scene
{
private:
    GLuint m_vaoHandle;
    GLSLProgram m_prog;
    float m_angle;

    glm::mat4 m_rotationMatrix;

    void compile();

public:
    SceneBasic_Uniform(std::shared_ptr<CTracer> tracer);

    void initScene();
    void update(float t);
    void render();
    void renderGuiWindow(ImGuiIO* io) override {};
    void renderOtherGuiWindow(ImGuiIO* io)override {};

    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H