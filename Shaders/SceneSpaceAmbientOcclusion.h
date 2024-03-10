#ifndef SCENE_SPACE_AMBIENT_OCCLUSION_H
#define SCENE_SPACE_AMBIENT_OCCLUSION_H

#include <Scene.h>
#include <glslprogram.h>
#include <Plane.h>
#include <objMesh.h>
#include <random.h>

#include <cfunctracer.h>
#include <memory>

class SceneSpaceAmbientOcclusion : public Scene
{
private:
    GLSLProgram         m_prog;
    GLuint              m_aoTex[2];
    GLuint              m_woodTex;
    GLuint              m_brickTex;
    GLuint              m_deferredFBO;
    GLuint              m_ssaoFBO;
    GLuint              m_blurFBO;
    GLuint              m_quad;
    Random              m_rand;

    Plane               m_plane;
    std::unique_ptr<ObjMesh> m_bunny;
    glm::mat4            m_sceneProj;

    float                m_angle;
    float                m_tPrev;
    float                m_rotSpeed;

    GLuint buildRandRotationTex(void);
    void setMatrices(void);
    void compileAndLinkShader(void);
    void setupFrameBufferObjects(void);
    void createGBufTex(GLenum, GLenum, GLuint&);
    void buildKernel(void);
    void pass1(void);
    void pass2(void);
    void pass3(void);
    void pass4(void);
    void drawScreenFillingQuad(void);
    void drawScene(void);
public:
    SceneSpaceAmbientOcclusion(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;
};

#endif //SCENE_SPACE_AMBIENT_OCCLUSION_H

