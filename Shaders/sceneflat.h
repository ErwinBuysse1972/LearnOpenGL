#ifndef SCENE_FLAT_H
#define SCENE_FLAT_H
#include "scene.h"

#include <glad/glad.h>
#include "glslprogram.h"
#include <glm/glm.hpp>
#include "Helper/objMesh.h"

#include <cfunctracer.h>

class SceneFlat : public Scene
{
private:
    GLSLProgram                 m_prog;
    std::unique_ptr<ObjMesh>    m_ogre;
    float                       m_angle;

    void SetMatrices(void);
    void CompileAndLinkShader();

public:
    SceneFlat(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override {};
    void renderOtherGuiWindow(ImGuiIO* io) override {};

    void resize(int w, int h) override;
};


#endif //SCENE_FLAT_H
