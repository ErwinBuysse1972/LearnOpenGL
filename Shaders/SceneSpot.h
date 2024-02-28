#ifndef SCENE_SPOT_H
#define SCENE_SPOT_H

#include <glad/glad.h>
#include "Helper/Scene.h"
#include "glslprogram.h"
#include <glm/glm.hpp>
#include <cfunctracer.h>
#include "Torus.h"
#include "Plane.h"
#include "teapot.h"

#include <cfunctracer.h>

class SceneSpot : public Scene
{
private:
    GLSLProgram m_prog;
    float       m_tPrev;
    Plane       m_plane;
    Teapot      m_teapot;
    Torus       m_torus;

    float       m_angle;

    void SetMatrices();
    void CompileAndLinkShader();

public:
    SceneSpot(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void resize(int w, int h) override;
};

#endif // SCENE_SPOT_H
