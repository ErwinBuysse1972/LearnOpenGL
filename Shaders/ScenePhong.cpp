#include "ScenePhong.h"
#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;
#include <GLFW/glfw3.h>

#include <imgui.h>


ScenePhong::ScenePhong(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_torus(0.7f, 0.3f, 50, 50)
    , m_camera(tracer, 
               glm::vec3(0.0f, 0.0f, 2.0f), // cam position
               glm::vec3(0.0f, 0.0f, 0.0f)) // target position
    , m_bFirstFrame(true)
{
    CFuncTracer trace("ScenePhong::ScenePhong", m_trace);
    try
    {

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
ScenePhong::~ScenePhong()
{
    CFuncTracer trace("ScenePhong::~ScenePhong", m_trace);
    try
    {
       bool bok = UnSubscribeAllKeypress();
       if (!bok)
           trace.Error("Failed to unsubscribe the keypress events");

       bok = UnSubscribeAllMouse();
       if (!bok)
           trace.Error("Failed to unsubscribe the mouse events");
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void ScenePhong::initScene()
{
    CFuncTracer trace("ScenePhong::initScene", m_trace);
    try
    {
        CompileAndLinkShader();

        glEnable(GL_DEPTH_TEST);

        m_camera.setYaw(0.0f);
        m_camera.setPitch(0.0f);

        m_projection = mat4(1.0f);

        // Create the viewMatrix
        m_view = m_camera.getViewMatrix();

        trace.Info("View Matrix :");
        trace.Info("    %.8f  %.8f  %.8f  %.8f", m_view[0][0], m_view[0][1], m_view[0][2], m_view[0][3]);
        trace.Info("    %.8f  %.8f  %.8f  %.8f", m_view[1][0], m_view[1][1], m_view[1][2], m_view[1][3]);
        trace.Info("    %.8f  %.8f  %.8f  %.8f", m_view[2][0], m_view[2][1], m_view[2][2], m_view[2][3]);
        trace.Info("    %.8f  %.8f  %.8f  %.8f", m_view[3][0], m_view[3][1], m_view[3][2], m_view[3][3]);
        trace.Info("Projection Matrix :");
        trace.Info("    %.8f  %.8f  %.8f  %.8f", m_projection[0][0], m_projection[0][1], m_projection[0][2], m_projection[0][3]);
        trace.Info("    %.8f  %.8f  %.8f  %.8f", m_projection[1][0], m_projection[1][1], m_projection[1][2], m_projection[1][3]);
        trace.Info("    %.8f  %.8f  %.8f  %.8f", m_projection[2][0], m_projection[2][1], m_projection[2][2], m_projection[2][3]);
        trace.Info("    %.8f  %.8f  %.8f  %.8f", m_projection[3][0], m_projection[3][1], m_projection[3][2], m_projection[3][3]);


        m_material.Ka = vec3(0.9f, 0.5f, 0.3f);
        m_material.Kd = vec3(0.9f, 0.5f, 0.3f);
        m_material.Ks = vec3(0.8f, 0.8f, 0.8f);
        m_material.Shininess = 100.0f;

        m_light.La = vec3(0.4f, 0.4f, 0.4f);
        m_light.Ld = vec3(1.0f, 1.0f, 1.0f);
        m_light.Ls = vec3(1.0f, 1.0f, 1.0f);
        m_light.Position = vec4(5.0f, 5.0f, 2.0f, 1.0f);

        m_angle_x = -35.0f;
        m_angle_y = 0.0f;

        std::vector<unsigned long> keys = {GLFW_KEY_RIGHT, GLFW_KEY_LEFT, GLFW_KEY_DOWN, GLFW_KEY_UP, GLFW_KEY_M };
        bool bOk = SubscribeKeypress([=](const std::string id, unsigned long keypress) {
            if (m_bEnableKeyboard == true)
                processKeypress(keypress);
            }, keys, "ScenePhong");
        if (!bOk)
            trace.Error("Failed to subscribe key events");
            
        bOk = SubscribeMouse([=](const std::string& id, float dx, float dy, float start_x, float start_y, MouseButtonStatus button) {
            if (m_bEnableMouse == true)
                processMouse(dx, dy, start_x, start_y, button);
            }, "ScenePhong");
        if (!bOk)
            trace.Error("Failed to subscribe mouse events");
            
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void ScenePhong::update(float t)
{
    CFuncTracer trace("ScenePhong::update", m_trace);
    try
    {
        if (m_bFirstFrame)
        {
            m_lastTime = t;
            m_bFirstFrame = false;
        }
        m_dt = t - m_lastTime;
        m_lastTime = t;


    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void ScenePhong::processKeypress(unsigned long key)
{
    CFuncTracer trace("ScenePhong::processKeypress", m_trace, false);
    try
    {
        trace.Trace("Key pressed : %ld", key);
        if (key == GLFW_KEY_UP) m_camera.processKeyboard(CameraMovement::forward, m_dt);
        if (key == GLFW_KEY_DOWN) m_camera.processKeyboard(CameraMovement::backward, m_dt);
        if (key == GLFW_KEY_LEFT) m_camera.processKeyboard(CameraMovement::left, m_dt);
        if (key == GLFW_KEY_RIGHT) m_camera.processKeyboard(CameraMovement::right, m_dt);
        if (key == GLFW_KEY_M)
        {
            KeyboardMode eMode = m_camera.getKbdMode();
            if (eMode == KeyboardMode::eRotation) m_camera.setKbdMode(KeyboardMode::eMovement);
            else if (eMode == KeyboardMode::eMovement) m_camera.setKbdMode(KeyboardMode::eRotation);
        }
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void ScenePhong::processMouse(float dx, float dy, float start_x, float start_y, MouseButtonStatus button)
{
    CFuncTracer trace("ScenePhong::processMouse", m_trace, false);
    try
    {
        trace.Info("dx: %.8f, dy: %.8f, startx: %.8f, starty: %.8f button: %02x", dx, dy, start_x, start_y, static_cast<unsigned long>(button));
      // if (static_cast<unsigned long>(button) & static_cast<unsigned long>(MouseButtonStatus::rightPressed)) m_camera.processMouseRotation(dx, dy);
      //  if (static_cast<unsigned long>(button) & static_cast<unsigned long>(MouseButtonStatus::leftPressed)) m_camera.processMouseMove(dx, dy);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occcured : %s", ex.what());
    }
}
void ScenePhong::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("ScenePhong::renderGuiWindow", m_trace);
    try
    {
        ImGui::Begin("Settings");
        if (ImGui::CollapsingHeader("Light settings"))
        {
            ImGui::InputFloat3("Ambient light", &m_light.La.x, "%.8f");
            ImGui::InputFloat3("Diffuse light", &m_light.Ld.x, "%.8f");
            ImGui::InputFloat3("Specular light", &m_light.Ls.x, "%.8f");
            ImGui::InputFloat4("Position", &m_light.Position.x, "%.8f");
        }
        if (ImGui::CollapsingHeader("Material settings"))
        {
            ImGui::InputFloat3("Ambient reflection", &m_material.Ka.x, "%.8f");
            ImGui::InputFloat3("Diffuse reflection", &m_material.Kd.x, "%.8f");
            ImGui::InputFloat3("Specular reflection", &m_material.Ks.x, "%.8f");
            ImGui::InputFloat("Shininess", &m_material.Shininess, 0.0f, 0.0f, "%.8f");
        }
        if (ImGui::CollapsingHeader("Model settings"))
        {
            ImGui::SliderFloat("angle_x", &m_angle_x, 0.0f, 360.0f);
            ImGui::SliderFloat("angle_y", &m_angle_y, 0.0f, 360.0f);
        }
        if (ImGui::CollapsingHeader("Camera settings"))
        {
            glm::vec3 position = m_camera.getPosition();
            glm::vec3 target = m_camera.getTarget();
            float yaw = m_camera.getYaw();
            float pitch = m_camera.getPitch();
            float kbdSpeed = m_camera.getMovementSpeed();
            KeyboardMode eMode = m_camera.getKbdMode();

            ImGui::SliderFloat("camPosition X", &position.x, -100.0f, 100.0f);
            ImGui::SliderFloat("camPosition Y", &position.y, -100.0f, 100.0);
            ImGui::SliderFloat("camPosition Z", &position.z, -100.0f, 100.0f);
            ImGui::SliderFloat("cam target X", &target.x, -100.0f, 100.0f);
            ImGui::SliderFloat("cam target Y", &target.y, -100.0f, 100.0f);
            ImGui::SliderFloat("cam target Z", &target.z, -100.0f, 100.0f);
            ImGui::SliderFloat("Yaw", &yaw, -180.0f, 180.0f);
            ImGui::SliderFloat("Pitch", &pitch, -180.0f, 180.0);
            ImGui::SliderFloat("KbdSpeed", &kbdSpeed, 1.0f, 50.0f);
            ImGui::Checkbox("Enable keyboard control", &m_bEnableKeyboard);
            ImGui::Checkbox("Enable mouse control", &m_bEnableMouse);
            if (eMode == KeyboardMode::eRotation) ImGui::Text("current keyboard Mode : Rotation (can be switched pressing 'M'");
            if (eMode == KeyboardMode::eMovement) ImGui::Text("current keyboard Mode : Movement (can be switched pressing 'M");

            if (   (position != m_camera.getPosition()) 
                || (target != m_camera.getTarget())
                || (yaw != m_camera.getYaw())
                || (pitch != m_camera.getPitch())
               )
            {
                m_camera.setPosition(position);
                m_camera.setTarget(target);
                m_camera.rotate(yaw, pitch);
            }

            if (kbdSpeed != m_camera.getMovementSpeed()) m_camera.setMovementSpeed(kbdSpeed);
        }
        ImGui::End();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void ScenePhong::render()
{
    CFuncTracer trace("ScenePhong::render", m_trace);
    try
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_model = mat4(1.0f);
        m_model = glm::rotate(m_model, glm::radians(m_angle_x), vec3(1.0f, 0.0f, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(m_angle_y), vec3(0.0f, 1.0f, 0.0f));
     //   m_model = glm::rotate(m_model, glm::radians(m_angle_z), vec3(0.0f, 0.0f, 1.0f));

        trace.Info("Model Matrix :");
        trace.Info("    %.8f  %.8f  %.8f  %.8f", m_model[0][0], m_model[0][1], m_model[0][2], m_model[0][3]);
        trace.Info("    %.8f  %.8f  %.8f  %.8f", m_model[1][0], m_model[1][1], m_model[1][2], m_model[1][3]);
        trace.Info("    %.8f  %.8f  %.8f  %.8f", m_model[2][0], m_model[2][1], m_model[2][2], m_model[2][3]);
        trace.Info("    %.8f  %.8f  %.8f  %.8f", m_model[3][0], m_model[3][1], m_model[3][2], m_model[3][3]);

        // Create the viewMatrix
        m_view = m_camera.getViewMatrix();


        m_prog.setUniform("Material.Kd", m_material.Kd);
        m_prog.setUniform("Material.Ka", m_material.Ka);
        m_prog.setUniform("Material.Ks", m_material.Ks);
        m_prog.setUniform("Material.Shininess", m_material.Shininess);

        m_prog.setUniform("Light.Ld", m_light.Ld);
        m_prog.setUniform("Light.Position", m_light.Position * m_view);
        m_prog.setUniform("Light.La", m_light.La);
        m_prog.setUniform("Light.Ls", m_light.Ls);


        SetMatrices();
        m_torus.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void ScenePhong::resize(int w, int h)
{
    CFuncTracer trace("ScenePhong::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
        m_projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void ScenePhong::SetMatrices()
{
    CFuncTracer trace("ScenePhong::SetMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
        trace.Info("ModelViewMatrix :");
        trace.Info("    %.8f  %.8f  %.8f  %.8f", mv[0][0], mv[0][1], mv[0][2], mv[0][3]);
        trace.Info("    %.8f  %.8f  %.8f  %.8f", mv[1][0], mv[1][1], mv[1][2], mv[1][3]);
        trace.Info("    %.8f  %.8f  %.8f  %.8f", mv[2][0], mv[2][1], mv[2][2], mv[2][3]);
        trace.Info("    %.8f  %.8f  %.8f  %.8f", mv[3][0], mv[3][1], mv[3][2], mv[3][3]);

        m_prog.setUniform("ModelViewMatrix", mv);
        m_prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        m_prog.setUniform("MVP", m_projection * mv);


    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void ScenePhong::CompileAndLinkShader()
{
    CFuncTracer trace("ScenePhong::CompileAndLinkShader", m_trace);
    try 
    {
        m_prog.compileShader("shaders/phong.vert.glsl");
        m_prog.compileShader("shaders/phong.frag.glsl");
        //m_prog.compileShader("shaders/function.vert.glsl");
        //m_prog.compileShader("shaders/function.frag.glsl");
        m_prog.link();
        m_prog.use();
    }
    catch (GLSLProgramException& e) 
    {
        cerr << e.what() << endl;
        trace.Error("GLSL program exception occurred : %s", e.what());
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
        exit(EXIT_FAILURE);
    }
}
