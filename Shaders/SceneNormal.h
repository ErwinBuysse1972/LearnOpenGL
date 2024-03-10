#ifndef SCENE_NORMAL_H
#define SCENE_NORMAL_H

#include "scene.h"

#include <glad/glad.h>
#include "glslprogram.h"
#include <glm/glm.hpp>

#include <objMesh.h>

#include <cfunctracer.h>

class SceneNormalMap : public Scene
{
private:
    GLSLProgram                 m_prog;
    std::unique_ptr<ObjMesh>    m_ogre;

    float m_angle;
    float m_rotSpeed;
    float m_tPrev;

    void SetMatrices();
    void CompileAndLinkShader();

public:
    SceneNormalMap(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override {};
    void renderOtherGuiWindow(ImGuiIO* io)override {};

    void resize(int w, int h) override;

};

#endif // SCENE_NORMAL_H
