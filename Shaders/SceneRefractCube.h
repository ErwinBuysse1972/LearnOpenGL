#ifndef SCENE_REFRACT_CUBE_H
#define SCENE_REFRACT_CUBE_H

#include <Scene.h>
#include <glslprogram.h>
#include <teapot.h>
#include <skybox.h>

#include <glm/glm.hpp>
#include <cfunctracer.h>

class SceneRefractCube : public Scene
{
private:
    GLSLProgram m_prog;
    GLSLProgram m_skyProg;
    Teapot      m_teapot;
    SkyBox      m_sky;

    float m_angle;
    float m_rotSpeed;
    float m_tPrev;

    void SetMatrices(GLSLProgram& p);
    void CompileAndLinkShader();

public:
    SceneRefractCube(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void resize(int w, int h) override;
};
#endif //SCENE_REFRACT_CUBE_H