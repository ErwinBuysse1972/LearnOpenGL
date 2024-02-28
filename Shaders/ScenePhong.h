#ifndef SCENEPHONG_H
#define SCENEPHONG_H
#include "scene.h"

#include <glad/glad.h>
#include "glslprogram.h"
#include <glm/glm.hpp>
#include "Helper/Torus.h"

#include <cfunctracer.h>

class ScenePhong : public Scene
{
private:
    GLSLProgram m_prog;
    Torus       m_torus;
    float       m_angle;
    
    void SetMatrices();
    void CompileAndLinkShader();

public:
    ScenePhong(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void resize(int w, int h) override;
};
#endif //SCENEPHONG_H
