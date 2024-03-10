#ifndef SCENE_EDGE_DETECTION_FILTER_H
#define SCENE_EDGE_DETECTION_FILTER_H

#include <Scene.h>
#include <glslprogram.h>
#include <Plane.h>
#include <Torus.h>
#include <teapot.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <cfunctracer.h>


class SceneEdgeDetection : public Scene
{
private:
    GLSLProgram m_prog;
    GLuint      m_fsQuad;
    GLuint      m_fboHandle;
    GLuint      m_renderTex;

    Plane       m_plane;
    Torus       m_torus;
    Teapot      m_teapot;

    float       m_angle;
    float       m_tPrev;
    float       m_rotSpeed;

    void setMatrices();
    void compileAndLinkShader();
    void setupFrameBufferObjects(void);
    void pass1(void);
    void pass2(void);

public:
    SceneEdgeDetection(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;

};

#endif //SCENE_EDGE_DETECTION_FILTER_H

