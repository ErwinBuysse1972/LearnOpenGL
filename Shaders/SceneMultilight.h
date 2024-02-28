#ifndef SCENE_MULTILIGHT_H
#define SCENE_MULTILIGHT_H

#include "Scene.h"
#include "glslprogram.h"
#include "Plane.h"
#include "objMesh.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <cfunctracer.h>

class SceneMultiLight : public Scene
{
private:
    GLSLProgram m_prog;
    Plane       m_plane;
    std::unique_ptr<ObjMesh> m_mesh;

    void SetMatrices();
    void CompileAndLinkShader();

public:
    SceneMultiLight(std::shared_ptr<CTracer> tracer);
    void initScene() override;
    void update(float t) override;
    void render() override;
    void resize(int w, int h) override;

};

#endif // SCENE_MULTILIGHT_H
