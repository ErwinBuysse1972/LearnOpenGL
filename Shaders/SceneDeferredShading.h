#ifndef SCENE_DEFERRED_SHADING_H
#define SCENE_DEFERRED_SHADING_H

#include <Scene.h>
#include <glslprogram.h>
#include <Plane.h>
#include <Torus.h>
#include <teapot.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cfunctracer.h>
#include <memory>

class SceneDeferredShading : public Scene
{
private:
    GLSLProgram         m_prog;
    GLuint              m_deferredFBO;
    GLuint              m_quad;

    Plane               m_plane;
    Torus               m_torus;
    Teapot              m_teapot;

    float               m_angle;
    float               m_tPrev;
    float               m_rotSpeed;

    void setMatrices(void);
    void compileAndLinkShader(void);
    void setupFrameBufferObjects(void);
    void createGBufTex(GLenum, GLenum, GLuint&);
    void pass1(void);
    void pass2(void);

public:
    SceneDeferredShading(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;

};

#endif //SCENE_DEFERRED_SHADING_H

