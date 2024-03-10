#ifndef DIFFUSE_IMAGE_BASED_LIGHTING_H
#define DIFFUSE_IMAGE_BASED_LIGHTING_H

#include <Scene.h>
#include <glslprogram.h>

#include <objMesh.h>
#include <skybox.h>

#include <glm/glm.hpp>
#include <cfunctracer.h>

class SceneDiffIbl : public Scene
{
private:
    GLSLProgram                 m_prog;
    GLSLProgram                 m_sbProg;
    GLuint                      m_diffCube;
    GLuint                      m_colorTex;
    GLuint                      m_cube;

    std::unique_ptr<ObjMesh>    m_spot;
    SkyBox                      m_skyBox;

    glm::vec3                   m_camPos;
    float                       m_camAngle;
    float                       m_tPrev;
    float                       m_rotSpeed;

    void setMatrices(GLSLProgram& prog);
    void compileAndLinkShader(void);

public:
    SceneDiffIbl(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;

};

#endif //DIFFUSE_IMAGE_BASED_LIGHTING_H

