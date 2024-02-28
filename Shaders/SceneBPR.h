#ifndef SCENE_BPR_H
#define SCENE_BPR_H
#include <glad/glad.h>
#include "Helper/Scene.h"
#include "glslprogram.h"
#include <glm/glm.hpp>
#include <cfunctracer.h>
#include <objMesh.h>
#include "Torus.h"
#include "Plane.h"
#include "teapot.h"

#include <cfunctracer.h>
class SceneBPR : public Scene
{
private:
    GLSLProgram m_prog;
    Plane       m_plane;
    std::unique_ptr<ObjMesh> m_mesh;
    Teapot      m_teapot;

    float m_tPrev;
    float m_lightAngle;
    float m_lightRotationSpeed;

    glm::vec4 m_lightPos;

    void SetMatrices();
    void CompileAndLinkShader();
    void DrawScene();
    void DrawFloor();
    void DrawSpot(const glm::vec3& pos, float rough, int metal, const glm::vec3& color);

public:
    SceneBPR(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void resize(int w, int h) override;
};
#endif // SCENE_BPR_H
