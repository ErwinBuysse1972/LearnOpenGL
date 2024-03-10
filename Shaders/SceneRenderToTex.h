#ifndef SCENE_RENDER_TO_TEX_H
#define SCENE_RENDER_TO_TEX_H

#include <memory>

#include <Scene.h>
#include <glslprogram.h>
#include <Cube.h>
#include <objMesh.h>

#include <glm/glm.hpp>

#include <cfunctracer.h>

class SceneRenderToTex : public Scene
{
private:
    GLSLProgram m_prog;

    GLuint                      m_fboHandle;
    Cube                        m_cube;
    std::unique_ptr<ObjMesh>    m_spot;

    float                       m_angle;
    float                       m_cow_angle;
    float                       m_tPrev;
    float                       m_rotSpeed;

    void setMatrices(void);
    void compileAndLinkShader(void);
    void setupFrameBufferObjects(void);
    void renderToTexture(void);
    void renderScene(void);

public:
    SceneRenderToTex(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;
};

#endif //SCENE_RENDER_TO_TEX_H
