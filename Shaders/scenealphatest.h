#ifndef SCENE_ALPHATEST_H
#define SCENE_ALPHATEST_H

#include "Scene.h"
#include "teapot.h"
#include "glslprogram.h"

#include <glm/glm.hpp>

#include <cfunctracer.h>

class SceneAlphaTest : public Scene
{
private:
    GLSLProgram m_prog;
    Teapot      m_teapot;

    float m_angle;
    float m_tPrev;
    float m_rotSpeed;

    void SetMatrices();
    void CompileAndLinkShader();

public:
    SceneAlphaTest(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void resize(int w, int h) override;
};
#endif //SCENE_ALPHATEST_H
