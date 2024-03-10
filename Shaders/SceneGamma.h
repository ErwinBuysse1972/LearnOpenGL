#ifndef SCENE_GAMMA_H
#define SCENE_GAMMA_H

#include <memory>
#include <Scene.h>
#include <glslprogram.h>
#include <Plane.h>
#include <Torus.h>
#include <objMesh.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <cfunctracer.h>

class SceneGamma : public Scene
{
private:
    GLSLProgram                 m_prog;
    Plane                       m_plane;
    Torus                       m_torus;
    std::unique_ptr<ObjMesh>    m_ogre;

    float                       m_angle;
    float                       m_tPrev;
    float                       m_rotSpeed;

    void setMatrices(void);
    void compileAndLinkShader(void);

public:
    SceneGamma(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;
};
#endif //SCENE_GAMMA_H

