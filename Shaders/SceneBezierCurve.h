#ifndef SCENE_BEZIER_CURVE_H
#define SCENE_BEZIER_CURVE_H

#include <Scene.h>
#include <glslprogram.h>

#include <glad/glad.h>
#include <glm/glm.hpp>


class SceneBezierCurve : public Scene
{
private:
    GLSLProgram         m_prog;
    GLSLProgram         m_solidProg;

    GLuint              m_vaoHandle;

    int                 m_numStrips;
    int                 m_numSegments;

    void setMatrices(void);
    void compileAndLinkShader();
public:
    SceneBezierCurve(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;
};


#endif //SCENE_BEZIER_CURVE_H

