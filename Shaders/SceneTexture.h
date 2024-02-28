#ifndef SCENE_TEXTURE_H
#define SCENE_TEXTURE_H
#include "scene.h"

#include <glad/glad.h>
#include "glslprogram.h"
#include <glm/glm.hpp>

#include <Cube.h>

#include <cfunctracer.h>

class SceneTexture : public Scene
{
private:
    GLSLProgram m_prog;
    Cube        m_cube;
    float       m_angle;

    void SetMatrices();
    void CompileAndLinkShader();

public:
    SceneTexture(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void resize(int w, int h) override;

};
#endif //SCENE_TEXTURE_H
