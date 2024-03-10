#ifndef SCENE_SHADOW_MAP_H
#define SCENE_SHADOW_MAP_H

#include <Scene.h>
#include <glslprogram.h>

#include <Plane.h>
#include <Torus.h>
#include <teapot.h>
#include <frustum.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "cfunctracer.h"
#include <memory>


class SceneShadowMap : public Scene
{
private:
    GLSLProgram     m_prog;
    GLSLProgram     m_solidProg;
    GLuint          m_shadowFBO;
    GLuint          m_pass1Index;
    GLuint          m_pass2Index;

    Teapot          m_teapot;
    Plane           m_plane;
    Torus           m_torus;
    Frustum         m_lightFrustum;

    int             m_shadowMapWidth;
    int             m_shadowMapHeight;

    float           m_angle;
    float           m_tPrev;

    glm::mat4       m_lightPV;
    glm::mat4       m_shadowBias;

    void setMatrices(void);
    void compileAndLinkShader(void);
    void setupFrameBufferObjects(void);
    void drawScene(void);
    void splitOutDepthBuffer(void);

public:
    SceneShadowMap(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;
};

#endif //SCENE_SHADOW_MAP_H

