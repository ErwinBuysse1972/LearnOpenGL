#ifndef SCENE_SAMPLER_OBJ_H
#define SCENE_SAMPLER_OBJ_H

#include <Scene.h>
#include <glslprogram.h>
#include <Plane.h>

#include <glm/glm.hpp>

class SceneSamplerObj : public Scene
{
private:
    GLSLProgram m_prog;
    Plane       m_plane;

    float       m_angle;
    GLuint      m_linearSampler;
    GLuint      m_nearSampler;

    void setMatrices(void);
    void compileAndLinkShader(void);

public:
    SceneSamplerObj(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;
};

#endif //SCENE_SAMPLER_OBJ_H
