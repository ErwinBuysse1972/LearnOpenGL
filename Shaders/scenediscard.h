#ifndef SCENE_DISCARD_H
#define SCENE_DISCARD_H

#include "Scene.h"
#include "glslprogram.h"
#include "teapot.h"
#include <glad/glad.h>

#include <cfunctracer.h>

class SceneDiscard : public Scene
{
private:
    GLSLProgram m_prog;
    Teapot      m_teapot;

    void SetMatrices();
    void CompileAndLinkShader();

public:
    SceneDiscard(std::shared_ptr<CTracer> tracer);


    void initScene() override;
    void update(float t) override;
    void render() override;
    void resize(int w, int h) override;
};
#endif // SCENE_DISCARD_H
