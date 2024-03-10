#ifndef SCENE_REFLECT_CUBE_H
#define SCENE_REFLECT_CUBE_H

#include <Scene.h>
#include <glslprogram.h>
#include <teapot.h>
#include <skybox.h>

#include <cfunctracer.h>

class SceneReflectCube : public Scene
{
private:
    GLSLProgram m_prog;
    GLSLProgram m_skyProg;
    Teapot      m_teapot;
    SkyBox      m_sky;

    float       m_angle;
    float       m_tPrev;
    float       m_rotSpeed;
    float       m_clearColor;

    unsigned long m_ulCountClicks;

    void SetMatrices(GLSLProgram& p);
    void CompileAndLinkShader();

public:
    SceneReflectCube(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io)override {};

    void resize(int w, int h) override;

};
#endif //SCENE_REFLECT_CUBE_H
