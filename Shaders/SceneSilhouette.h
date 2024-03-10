#ifndef SCENE_SILHOUETTE_H
#define SCENE_SILHOUETTE_H

#include <Scene.h>
#include <glslprogram.h>
#include <objMesh.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cfunctracer.h>
#include <memory>

class SceneSilhouette : public Scene
{
private:
    GLSLProgram                 m_prog;
    std::unique_ptr<ObjMesh>    m_ogre;
    
    float                       m_angle;
    float                       m_rotSpeed;
    float                       m_tPrev;

    void setMatrices(void);
    void compileAndLinkShader(void);

public:
    SceneSilhouette(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;

};

#endif// SCENE_SILHOUETTE_H;

