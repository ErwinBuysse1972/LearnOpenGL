#include <iostream>
using std::cerr;
using std::endl;
#include <imgui.h>
#include "SceneMOW.h"

SceneMOW::SceneMOW(std::shared_ptr<CTracer> tracer)
    : Scene(tracer)
    , m_tPrev(0.0f)
    , m_plane(50.0f, 50.0f, 1, 1)
    , m_teapot(14, glm::mat4(1.0f))
    
{
    CFuncTracer trace("SceneMOW::SceneMOW", m_trace);
    try
    {
        animate(false);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
        exit(EXIT_FAILURE);
    }
}

void SceneMOW::InitLight()
{
    CFuncTracer trace("SceneMOW::InitLight", m_trace);
    try
    {
        m_light.L = vec3(0.9f);
        m_light.La = vec3(0.2f);
        m_light.Position = vec4(10.0f, 10.0f, 10.0f, 1.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

int SceneMOW::LogActiveUniformVars(GLuint shaderProgram)
{
    CFuncTracer trace("SceneMOW::LogActiveUniformVars", m_trace);
    try
    {
        GLint maxUniformLocations, numOfActiveUniforms;
        glad_glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &maxUniformLocations);
        glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &numOfActiveUniforms);

        printf("Max. uniform locations : %ld\n", maxUniformLocations);
        printf("Active uniforms : %ld\n", numOfActiveUniforms);
        int idx = 0;
        std::unique_ptr<char[]>buff = std::make_unique<char[]>(250);
        for (idx = 0; idx < numOfActiveUniforms; idx++)
        {
            GLsizei length = 0;
            GLint size = 0;
            GLenum type;
            glGetActiveUniform(shaderProgram, idx, 250, &length, &size, &type, buff.get());
            trace.Info("[loc %ld] %s", idx, buff.get());
            printf("[loc %ld] %s\n", idx, buff.get());
        } 
        return idx;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
    return 0;
}
void SceneMOW::InitMaterials()
{
    CFuncTracer trace("SceneMOW::InitMaterials", m_trace);
    try
    {
        m_TeapotMaterial.Ka = vec3(0.9f * 0.3f, 0.5f * 0.3f, 0.3f * 0.3f);
        m_TeapotMaterial.Kd = vec3(0.9f, 0.5f, 0.3f);
        m_TeapotMaterial.Ks = vec3(0.0f, 0.0f, 0.0f);
        m_TeapotMaterial.Shininess = 180.0f;

        
        m_PlaneMaterial.Ka = vec3(0.2f, 0.2f, 0.2f);
        m_PlaneMaterial.Kd = vec3(0.7f, 0.7f, 0.7f);
        m_PlaneMaterial.Ks = vec3(0.0f, 0.0f, 0.0f);
        m_PlaneMaterial.Shininess = 180.0f;

    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}

void SceneMOW::InitVegaPrimeValues()
{
    CFuncTracer trace("SceneMOW::InitVegaPrimeValues", m_trace);
    try
    {
        /*
        m_vpEnv_ambientColor_0       : 0.71642905 0.73346245 0.79995823 0.40499997
        m_vpEnv_blueHorizon_0        : 0.32712182 0.32712182 0.32712182 0.00006440
        m_vpEnv_hazeHorizon_0        : 0.32712182 0.32712182 0.32712182 0.79870933
        m_vpEnv_sunlight_0           : 0.03993562 0.04504257 0.06552000 0.65639627
        m_vpEnv_blueWeight_0         : 0.32804486 0.41262171 0.53333336 0.00006440
        m_vpEnv_hazeWeight_0         : 0.67195511 0.58737832 0.46666667 1.00000000
        m_vpEnv_temp1_0              : 1.04173625 1.19173622 1.50000000 1.00000000
        m_vpEnv_glow_0               : 5.00000000 0.00006504 -0.46000001 25000.00000000
        m_vpEnv_cameraPos_0          : 0.00000000 11228.26757812 0.00000000 1.00000000
        m_vpEnv_illumSunlightColor_0 : 0.62562048 0.62562048 0.62562048 1.00000000
        m_vpEnv_lightNormal_0        : -0.31375551 0.65639627 0.68607688 0.00000000
        vpMarine_v4Params0VS         : 0.00000000 0.00000000 0.00000000 0.00000000
        vrLight_Position_0           : 0.00000000 0.00000000 0.00000000 0.00000000
        vrLight_Ambient_0            : 0.00000000 0.00000000 0.00000000 0.00000000
        vrLight_Diffuse_0            : 0.00000000 0.00000000 0.00000000 0.00000000
        vrMaterial_Diffuse_Front     : 0.80000001 0.80000001 0.80000001 1.00000000
        AxisZEyeSpace                : 0.00000000 1.00000000 -0.00000004
        matMVInvTransp               : 1.00000000 -0.00000000 0.00000000 0.00000000
                                       -0.00000000 1.00000000 0.00000000 0.00000000
                                       0.00000000 0.00000000 1.00000000 0.00000000
                                       0.00000000 0.00000000 0.00000000 1.00000000
        MVP                          : 1.00000000 0.00000000 0.00000000 0.00000000
                                       0.00000000 1.00000000 0.00000000 0.00000000
                                       0.00000000 0.00000000 1.00000000 0.00000000
                                       0.00000000 0.00000000 0.00000000 1.00000000
        m_vpEnv_cloudShadow_0        : 0.00000000
        m_vpEnv_densityMultiplier_0  : 0.00006440
        m_vpEnv_distanceMultiplier_0 : 1.00000000
        m_vpEnv_maxY_0               : 1200.00000000
        m_vpEnv_cameraAltitude_0     : -21.73220253
        m_vpEnv_minColor_0           : 0.01200000
        m_unLastWrite                : 522

        */
        m_VP.m_vpEnv_ambientColor_0             = vec4(0.71642905f, 0.73346245f, 0.79995823f, 0.40499997f);
        m_VP.m_vpEnv_blueHorizon_0              = vec4(0.32712182f, 0.32712182f, 0.32712182f, 0.00006440f);
        m_VP.m_vpEnv_hazeHorizon_0              = vec4(0.32712182f, 0.32712182f, 0.32712182f, 0.79870933f);
        m_VP.m_vpEnv_sunlight_0                 = vec4(0.03993562f, 0.04504257f, 0.06552000f, 0.65639627f);
        m_VP.m_vpEnv_blueWeight_0               = vec4(0.32804486f, 0.41262171f, 0.53333336f, 0.00006440f);
        m_VP.m_vpEnv_hazeWeight_0               = vec4(0.67195511f, 0.58737832f, 0.46666667f, 1.00000000f);
        m_VP.m_vpEnv_temp1_0                    = vec4(1.04173625f, 1.19173622f, 1.50000000f, 1.00000000f);
        m_VP.m_vpEnv_glow_0                     = vec4(5.00000000f, 0.00006504f, -0.46000001f, 25000.00000000f);
        m_VP.m_vpEnv_cameraPos_0                = vec4(0.00000000f, 11228.26757812f, 0.00000000f, 1.00000000f);
        m_VP.m_vpEnv_illumSunlightColor_0       = vec4(0.62562048f, 0.62562048f, 0.62562048f, 1.00000000f);
        m_VP.m_vpEnv_lightNormal_0              = vec4(-0.31375551f, 0.65639627f, 0.68607688f, 0.00000000f);
        m_VP.m_vpMarine_v4Params0VS             = vec4(0.00000000f, 0.00000000f, 0.00000000f, 0.00000000f);
        m_VP.m_vrLight_Position_0               = vec4(0.00000000f, 0.00000000f, 0.00000000f, 0.00000000f);
        m_VP.m_vrMaterial_Diffuse_Front         = vec4(0.80000001f, 0.80000001f, 0.80000001f, 1.00000000f);
        m_VP.m_vsChannel_AxisZEyeSpace          = vec3(0.00000000f, 1.00000000f, -0.00000004f);
        m_VP.m_vpEnv_cloudShadow_0              = 0.00000000f;
        m_VP.m_vpEnv_densityMultiplier_0        = 0.00006440f;
        m_VP.m_vpEnv_distanceMultiplier_0       = 1.00000000f;
        m_VP.m_vpEnv_maxY_0                     = 1200.00000000f;
        m_VP.m_vpEnv_cameraAltitude_0           = -21.73220253f;
        m_VP.m_vpEnv_minColor_0                 = 0.01200000f;
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occured : %s", ex.what());
    }
}
void SceneMOW::initScene()
{
    CFuncTracer trace("SceneMOW::initScene", m_trace);
    try
    {
        CompileAndLinkShader();

        glEnable(GL_DEPTH_TEST);

        m_view = glm::lookAt(vec3(0.0f, 4.0f, 6.0f), vec3(0.0f, 2.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        m_projection = mat4(1.0f);

        m_angle = 0.0;

        InitMaterials();
        InitLight();
        InitVegaPrimeValues();

        m_prog.setUniform("Light.L", m_light.L);
        m_prog.setUniform("Light.La", m_light.La);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneMOW::update(float t)
{
    CFuncTracer trace("SceneMOW::update", m_trace);
    try
    {
        float deltaT = t - m_tPrev;
        if (m_tPrev == 0.0f) deltaT = 0.0f;
        m_tPrev = t;

        m_angle += 1.0f * deltaT;
        if (m_angle > glm::two_pi<float>()) m_angle -= glm::two_pi<float>();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneMOW::render()
{
    CFuncTracer trace("SceneMOW::render", m_trace);
    try
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (animating())
            m_light.Position = vec4(10.0f * cos(m_angle), m_light.Position.y, 10.0f * sin(m_angle), 1.0f);
        m_prog.setUniform("Light.Position", m_view * m_light.Position);
        m_prog.setUniform("Light.L", m_light.L);
        m_prog.setUniform("Light.La", m_light.La);

        m_prog.setUniform("Material.Kd", m_TeapotMaterial.Kd);
        m_prog.setUniform("Material.Ka", m_TeapotMaterial.Ka);
        m_prog.setUniform("Material.Ks", m_TeapotMaterial.Ks);
        m_prog.setUniform("Material.Shininess", m_TeapotMaterial.Shininess);


        m_prog.setUniform("VP.m_vpEnv_ambientColor_0", m_VP.m_vpEnv_ambientColor_0);
        m_prog.setUniform("VP.m_vpEnv_blueHorizon_0", m_VP.m_vpEnv_blueHorizon_0);
        m_prog.setUniform("VP.m_vpEnv_hazeHorizon_0", m_VP.m_vpEnv_hazeHorizon_0);
        m_prog.setUniform("VP.m_vpEnv_sunlight_0", m_VP.m_vpEnv_sunlight_0);
        m_prog.setUniform("VP.m_vpEnv_blueWeight_0", m_VP.m_vpEnv_blueWeight_0);
        m_prog.setUniform("VP.m_vpEnv_hazeWeight_0", m_VP.m_vpEnv_hazeWeight_0);
        m_prog.setUniform("VP.m_vpEnv_temp1_0", m_VP.m_vpEnv_temp1_0);
        m_prog.setUniform("VP.m_vpEnv_glow_0", m_VP.m_vpEnv_glow_0);
        m_prog.setUniform("VP.m_vpEnv_illumSunlightColor_0", m_VP.m_vpEnv_illumSunlightColor_0);
        m_prog.setUniform("VP.m_vpMarine_v4Params0VS", m_VP.m_vpMarine_v4Params0VS);
        m_prog.setUniform("VP.m_vrLight_Position_0", m_VP.m_vrLight_Position_0);
        m_prog.setUniform("VP.m_vsChannel_AxisZEyeSpace", m_VP.m_vsChannel_AxisZEyeSpace);
        m_prog.setUniform("VP.m_vpEnv_cloudShadow_0", m_VP.m_vpEnv_cloudShadow_0);
        m_prog.setUniform("VP.m_vpEnv_densityMultiplier_0", m_VP.m_vpEnv_densityMultiplier_0);
        m_prog.setUniform("VP.m_vpEnv_distanceMultiplier_0", m_VP.m_vpEnv_distanceMultiplier_0);
        m_prog.setUniform("VP.m_vpEnv_maxY_0", m_VP.m_vpEnv_maxY_0);
        m_prog.setUniform("VP.m_vpEnv_cameraAltitude_0", m_VP.m_vpEnv_cameraAltitude_0);


        float dist = 0.0f;
        for (int i = 0; i < 4; i++) {
            m_model = mat4(1.0f);
            m_model = glm::translate(m_model, vec3(dist * 0.6f - 1.0f, 0.0f, -dist));
            m_model = glm::rotate(m_model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
            SetMatrices();
            m_teapot.render();
            dist += 7.0f;
        }

        m_prog.setUniform("Material.Kd",  m_PlaneMaterial.Kd);
        m_prog.setUniform("Material.Ka", m_PlaneMaterial.Ka);
        m_prog.setUniform("Material.Ks", m_PlaneMaterial.Ks);
        m_prog.setUniform("Material.Shininess", m_PlaneMaterial.Shininess);

        m_model = mat4(1.0f);
        SetMatrices();
        m_plane.render();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneMOW::renderGuiWindow(ImGuiIO* io)
{
    CFuncTracer trace("SceneMOW::renderGuiWindow", m_trace);
    bool bAnimate = animating();
    try
    {
        ImGui::Begin("Light parameters");
        ImGui::Checkbox("Light animation", &bAnimate);
        
        animate(bAnimate);
        ImGui::InputFloat3("L", &m_light.L.x);
        ImGui::InputFloat3("La", &m_light.La.x);
        ImGui::InputFloat4("Position", &m_light.Position.x);
        ImGui::End();

        ImGui::Begin("Material parameters");
        ImGui::InputFloat3("Plane.Kd", &m_PlaneMaterial.Kd.x);
        ImGui::InputFloat3("Plane.Ka", &m_PlaneMaterial.Ka.x);
        ImGui::InputFloat3("Plane.Ks", &m_PlaneMaterial.Ks.x);
        ImGui::InputFloat("Plane.Shininess", &m_PlaneMaterial.Shininess);
        ImGui::InputFloat3("Teapot.Kd", &m_TeapotMaterial.Kd.x);
        ImGui::InputFloat3("Teapot.Ka", &m_TeapotMaterial.Ka.x);
        ImGui::InputFloat3("Teapot.Ks", &m_TeapotMaterial.Ks.x);
        ImGui::InputFloat("Teapot.Shininess", &m_TeapotMaterial.Shininess);
        ImGui::End();

        ImGui::Begin("Vega prime parameters");
        ImGui::InputFloat4("vpEnv_AmbientColor_0", &m_VP.m_vpEnv_ambientColor_0.x,"%.8f");
        ImGui::InputFloat4("vpEnv_BlueHorizon_0", &m_VP.m_vpEnv_blueHorizon_0.x, "%.8f");
        ImGui::InputFloat4("vpEnv_hazeHorizon_0", &m_VP.m_vpEnv_hazeHorizon_0.x, "%.8f");
        ImGui::InputFloat4("vpEnv_sunlight_0", &m_VP.m_vpEnv_sunlight_0.x, "%.8f");
        ImGui::InputFloat4("vpEnv_blueWeight_0", &m_VP.m_vpEnv_blueWeight_0.x, "%.8f");
        ImGui::InputFloat4("vpEnv_hazeWeight_0", &m_VP.m_vpEnv_hazeWeight_0.x, "%.8f");
        ImGui::InputFloat4("vpEnv_temp1_0", &m_VP.m_vpEnv_temp1_0.x, "%.8f");
        ImGui::InputFloat4("vpEnv_glow_0", &m_VP.m_vpEnv_glow_0.x, "%.8f");
        ImGui::InputFloat4("vpEnv_illumSunlightColor_0", &m_VP.m_vpEnv_illumSunlightColor_0.x, "%.8f");
        ImGui::InputFloat4("vpMarine_v4Params0VS", &m_VP.m_vpMarine_v4Params0VS.x, "%.8f");
        ImGui::InputFloat4("vrLight_Position_0", &m_VP.m_vrLight_Position_0.x, "%.8f");
        ImGui::InputFloat3("vsChannel_AxisZEyeSpace", &m_VP.m_vsChannel_AxisZEyeSpace.x, "%.8f");
        ImGui::InputFloat("vpEnv_cloudShadow_0", &m_VP.m_vpEnv_cloudShadow_0,0.0f, 0.0f, "%.8f");
        ImGui::InputFloat("vpEnv_densityMultiplier_0", &m_VP.m_vpEnv_densityMultiplier_0, 0.0f, 0.0f, "%.8f");
        ImGui::InputFloat("vpEnv_distanceMultiplier_0", &m_VP.m_vpEnv_distanceMultiplier_0, 0.0f, 0.0f, "%.8f");
        ImGui::InputFloat("vpEnv_maxY_0", &m_VP.m_vpEnv_maxY_0, 0.0f, 0.0f, "%.8f");
        ImGui::InputFloat("vpEnv_cameraAltitude_0", &m_VP.m_vpEnv_cameraAltitude_0, 0.0f, 0.0f, "%.8f");
        ImGui::End();
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred : %s", ex.what());
    }
}
void SceneMOW::renderOtherGuiWindow(ImGuiIO* io)
{
}

void SceneMOW::resize(int w, int h)
{
    CFuncTracer trace("SceneMOW::resize", m_trace);
    try
    {
        glViewport(0, 0, w, h);
        m_width = w;
        m_height = h;
        m_projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
        exit(EXIT_FAILURE);
    }
}
void SceneMOW::SetMatrices()
{
    CFuncTracer trace("SceneMOW::SetMatrices", m_trace);
    try
    {
        mat4 mv = m_view * m_model;
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
void SceneMOW::CompileAndLinkShader()
{
    CFuncTracer trace("SceneMOW::CompileAndLinkShader", m_trace);
    try
    {
        m_prog.compileShader("shaders/MOW_Ocean.vert.glsl");
        m_prog.compileShader("shaders/MOW_Ocean.frag.glsl");

        //m_prog.compileShader("shaders/Ocean.vert.glsl");
        //m_prog.compileShader("shaders/Ocean.frag.glsl");
        m_prog.link();
        m_prog.use();

        LogActiveUniformVars(m_prog.getHandle());
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
    catch (const std::exception& ex)
    {
        trace.Error("Exception occurred :%s", ex.what());
        exit(EXIT_FAILURE);
    }
}
