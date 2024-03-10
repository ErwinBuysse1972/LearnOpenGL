#ifndef SCENE_PCF_FILTER_H
#define SCENE_PCF_FILTER_H

#include <Scene.h>
#include <glslprogram.h>
#include <Plane.h>
#include <objMesh.h>
#include <frustum.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cfunctracer.h>
#include <memory>

class ScenePcfFilter : public Scene
{
private:
    GLSLProgram     m_prog;
    GLuint          m_shadowFBO;
    GLuint          m_pass1Index;
    GLuint          m_pass2Index;

    int             m_shadowMapWidth;
    int             m_shadowMapHeight;
    float           m_tPrev;
    float           m_angle;

    Plane                    m_plane;
    std::unique_ptr<ObjMesh> m_mesh;
    Frustum                  m_lightFrustum;

    glm::mat4       m_lightPV;
    glm::mat4       m_shadowScale;
    glm::vec3       m_lightPos;

    void setMatrices(void);
    void compileAndLinkShader(void);
    void setupFrameBufferObjects(void);
    void drawBuildingScene(void);

public:
    ScenePcfFilter(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;

};

#endif //SCENE_PCF_FILTER_H

