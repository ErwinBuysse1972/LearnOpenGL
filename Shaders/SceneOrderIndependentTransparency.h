#ifndef SCENE_ORDER_INDEPENDENT_TRANSPARENCY_H
#define SCENE_ORDER_INDEPENDENT_TRANSPARENCY_H

#include <Scene.h>
#include <glslprogram.h>
#include <Cube.h>
#include <Sphere.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

enum Buffernames
{
    COUNTER_BUFFER = 0,
    LINKED_LIST_BUFFER
};

struct ListNode
{
    glm::vec4 color;
    GLfloat depth;
    GLuint  next;
};

class SceneOrderIndependentTransparency : public Scene
{
private:
    GLSLProgram         m_prog;
    GLuint              m_buffers[2];
    GLuint              m_fsQuad;
    GLuint              m_headPtrTex;
    GLuint              m_pass1Index;
    GLuint              m_pass2Index;
    GLuint              m_clearBuf;

    Cube                m_cube;
    Sphere              m_sphere;

    float               m_angle;
    float               m_tPrev;
    float               m_rotSpeed;

    void setMatrices(void);
    void compileAndLinkShader(void);
    void drawScene(void);
    void initShaderStorage(void);
    void pass1(void);
    void pass2(void);
    void clearBuffers(void);
public:
    SceneOrderIndependentTransparency(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;

};

#endif //SCENE_ORDER_INDEPENDENT_TRANSPARENCY_H

