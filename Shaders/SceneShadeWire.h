#ifndef SCENE_SHADE_WIRE_H
#define SCENE_SHADE_WIRE_H

#include <Scene.h>
#include <glslprogram.h>
#include <objMesh.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

class SceneShadeWire : public Scene
{
private:
    GLSLProgram                 m_prog;
    std::unique_ptr<ObjMesh>    m_ogre;

    glm::mat4                   m_viewport;

    void setMatrices(void);
    void compileAndLinkShader(void);
public:
    SceneShadeWire(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;
};

#endif // SCENE_SHADE_WIRE_H

