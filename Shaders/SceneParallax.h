#ifndef SCENE_PARALLAX_H
#define SCENE_PARALLAX_H

#include <Scene.h>
#include <Plane.h>
#include <glslprogram.h>

class SceneParallax : public Scene
{
private:
    GLSLProgram m_prog;
    Plane       m_plane;

    float       m_tPrev;
    float       m_angle;

    GLuint      m_normalMap;
    GLuint      m_heightMap;
    GLuint      m_colorMap;

    void SetMatrices(GLSLProgram& p);
    void CompileAndLinkShader();

public:
    SceneParallax(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override {};
    void renderOtherGuiWindow(ImGuiIO* io) override {};

    void resize(int w, int h) override;

};
#endif //SCENE_PARALLAX_H
