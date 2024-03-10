#ifndef SCENE_POINT_SPRITES_H
#define SCENE_POINT_SPRITES_H

#include <Scene.h>
#include <glslprogram.h>

#include <glad/glad.h>
#include <memory>
#include <cfunctracer.h>

class ScenePointSprites: public Scene
{
private:
    GLSLProgram         m_prog;

    GLuint              m_sprites;
    int                 m_numSprites;
    float*              m_locations;

    void setMatrices(void);
    void compileAndLinkShader(void);

public:
    ScenePointSprites(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io) override;
    void resize(int w, int h) override;

};

#endif //SCENE_POINT_SPRITES_H

