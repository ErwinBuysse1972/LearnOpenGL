#ifndef SCENE_TEAPOT_TESS_DEPTH_H
#define SCENE_TEAPOT_TESS_DEPTH_H
#include <Scene.h>
#include <glslprogram.h>

#include <teapotpatch.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cfunctracer.h>
#include <memory>
class SceneTessTeapotDepth : public Scene
{
private:
    GLSLProgram         m_prog;
    GLuint              m_vaoHandle;
    TeapotPatch         m_teapot;
    glm::mat4           m_viewport;

    float               m_angle;
    float               m_tPrev;
    float               m_rotSpeed;

    void setMatrices(void);
    void compileAndLinkShader(void);
public:
    SceneTessTeapotDepth(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;
};

#endif //SCENE_TEAPOT_TESS_DEPTH_H

