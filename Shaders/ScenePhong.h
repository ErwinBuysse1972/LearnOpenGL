#ifndef SCENEPHONG_H
#define SCENEPHONG_H
#include "scene.h"

#include <glad/glad.h>
#include <Camera.h>
#include "glslprogram.h"
#include "Helper/Torus.h"

#include <cfunctracer.h>



class ScenePhong : public Scene
{
private:
    GLSLProgram     m_prog;
    Torus           m_torus;
    float           m_angle_x;
    float           m_angle_y;

    CameraArcBall   m_camera;

    LightInfo       m_light;
    MaterialInfo    m_material;
    float           m_dt;
    float           m_lastTime;

    bool            m_bFirstFrame;
    bool            m_bEnableKeyboard;
    bool            m_bEnableMouse;
    
    void SetMatrices();
    void CompileAndLinkShader();
    void processKeypress(unsigned long key);
    void processMouse(float dx, float dy, float start_x, float start_y, MouseButtonStatus button);

public:
    ScenePhong(std::shared_ptr<CTracer> tracer);
    virtual ~ScenePhong();
    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io)override {};

    void resize(int w, int h) override;
};
#endif //SCENEPHONG_H
