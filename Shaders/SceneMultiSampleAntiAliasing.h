#ifndef SCENE_MULTI_SAMPLE_ANTI_ALLIASING_H
#define SCENE_MULTI_SAMPLE_ANTI_ALLIASING_H

#include <Scene.h>
#include <glslprogram.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cfunctracer.h>

class SceneMultiSampleAntiAliasing : public Scene
{
private:
    GLSLProgram         m_prog;
    GLuint              m_quad;

    float               m_angle;
    float               m_tPrev;
    float               m_rotSpeed;

    void setMatrices(void);
    void compileAndLinkShader(void);

public:
    SceneMultiSampleAntiAliasing(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;

};

#endif // SCENE_MULTI_SAMPLE_ANTI_ALLIASING_H
