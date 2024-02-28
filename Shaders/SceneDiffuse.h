#ifndef SCENEDIFFUSE_H
#define SCENEDIFFUSE_H
#include "scene.h"

#include <glad/glad.h>
#include "glslprogram.h"
#include <glm/glm.hpp>
#include "Helper/Torus.h"

#include <cfunctracer.h>

class SceneDiffuse : public Scene
{
private:
    GLSLProgram m_prog;
    Torus m_torus;

    void SetMatrices(void);
    void CompileAndLinkShader();

public:
    SceneDiffuse(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void resize(int w, int h) override;
};
#endif // SCENEDIFFUSE_H
