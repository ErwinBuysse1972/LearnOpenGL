#ifndef SCENE_SUBROUTINE_H
#define SCENE_SUBROUTINE_H

#include "scene.h"

#include <glad/glad.h>
#include "glslprogram.h"
#include <glm/glm.hpp>
#include <cfunctracer.h>
#include "teapot.h"

class SceneSubroutine : public Scene
{
private:
    GLSLProgram m_prog;
    Teapot      m_teapot;

    void SetMatrices(void);
    void CompileAndLinkShader(void);

public:
    SceneSubroutine(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void resize(int w, int h) override;
};
#endif // SCENE_SUBROUTINE_H
