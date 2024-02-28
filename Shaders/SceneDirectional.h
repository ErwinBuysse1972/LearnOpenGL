#ifndef SCENE_DIRECTIONAL_H
#define SCENE_DIRECTIONAL_H
#include "Scene.h"
#include "glslprogram.h"
#include "Torus.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <cfunctracer.h>

class SceneDirectional : public Scene
{
private:
    GLSLProgram     m_prog;
    Torus           m_torus;

    void SetMatrices();
    void CompileAndLinkShader();
public:
    SceneDirectional(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void resize(int w, int h) override;
};
#endif //SCENE_DIRECTIONAL_H