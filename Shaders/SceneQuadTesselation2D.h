#ifndef SCENE_QUAD_TESSELATION_2D_H
#define SCENE_QUAD_TESSELATION_2D_H

#include <Scene.h>
#include <glslprogram.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <cfunctracer.h>
#include <memory>

class SceneQuadTesselation2D : public Scene
{
private:
    GLSLProgram             m_prog;
    GLuint                  m_vaoHandle;
    glm::mat4               m_viewport;

    int                     m_inner;
    int                     m_outer;
    float                   m_lineWidth;

    void setMatrices(void);
    void compileAndLinkShader(void);

public:
    SceneQuadTesselation2D(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;
};

#endif //SCENE_QUAD_TESSELATION_2D_H

