#ifndef SCENE_SEPARABLE_H
#define SCENE_SEPARABLE_H

#include "scene.h"

#include <glad/glad.h>
#include "glslprogram.h"
#include <glm/glm.hpp>

#include <cfunctracer.h>

class SceneSeparable : public Scene
{
private:
    GLuint m_vaoHandle;
    GLuint m_programs[3];
    GLuint m_pipelines[2];
    GLuint m_uniLocation;

    void Link(GLuint program);
    void CheckLinkStatus(GLuint program);
    GLuint CompileShader(const std::string& file, GLenum shaderType);
    std::string GetShaderInfoLog(GLuint shader);
    std::string GetProgramInfoLog(GLuint program);

public:
    SceneSeparable(std::shared_ptr<CTracer> tracer);

    void CreateShaderPrograms1();
    void CreateShaderPrograms2();

    void SetupVao();
    std::string LoadShaderCode(const std::string& file);
    void initScene() override;
    void update(float t) override;
    void render() override ;
    void renderGuiWindow(ImGuiIO* io) override {};
    void renderOtherGuiWindow(ImGuiIO* io)override {};

    void resize(int w, int h) override;
};

#endif //SCENE_SEPARABLE_H
