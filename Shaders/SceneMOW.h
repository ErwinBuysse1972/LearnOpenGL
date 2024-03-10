#ifndef SCENE_MOW_H
#define SCENE_MOW_H

#include "Scene.h"
#include "glslprogram.h"
#include "Plane.h"
#include "teapot.h"
#include "Torus.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;
using glm::vec4;

#include <cfunctracer.h>


struct VegaPrime
{
    vec4 m_vpEnv_ambientColor_0;
    vec4 m_vpEnv_blueHorizon_0;
    vec4 m_vpEnv_hazeHorizon_0;
    vec4 m_vpEnv_sunlight_0;
    vec4 m_vpEnv_blueWeight_0;
    vec4 m_vpEnv_hazeWeight_0;
    vec4 m_vpEnv_temp1_0;
    vec4 m_vpEnv_glow_0;
    vec4 m_vpEnv_cameraPos_0;
    vec4 m_vpEnv_illumSunlightColor_0;
    vec4 m_vpEnv_lightNormal_0;
    vec4 m_vpMarine_v4Params0VS;
    vec4 m_vrLight_Position_0;
    vec4 m_vrMaterial_Diffuse_Front;
    vec3 m_vsChannel_AxisZEyeSpace;
    float m_vpEnv_cloudShadow_0;
    float m_vpEnv_densityMultiplier_0;
    float m_vpEnv_distanceMultiplier_0;
    float m_vpEnv_maxY_0;
    float m_vpEnv_cameraAltitude_0;
    float m_vpEnv_minColor_0;
    //    uint m_unLastWrite;
    //    mat4 m_vrTransform_ModelViewMatrixInverseTranspose;
    //    mat4 m_vrTransform_ModelViewProjectionMatrix;
};


class SceneMOW : public Scene
{
private:
    GLSLProgram m_prog;
    float       m_tPrev;
    Plane       m_plane;
    Teapot      m_teapot;

    VegaPrime       m_VP;
    
    MaterialInfo    m_TeapotMaterial;
    MaterialInfo    m_PlaneMaterial;

    LightInfoS       m_light;

    float       m_angle;

    void SetMatrices();
    void CompileAndLinkShader();
    void InitVegaPrimeValues();
    void InitMaterials();
    void InitLight();
    int LogActiveUniformVars(GLuint shaderProgram);

public:
    SceneMOW(std::shared_ptr<CTracer> tracer);

    void initScene() override;
    void update(float t) override;
    void render() override;
    void renderGuiWindow(ImGuiIO* io) override;
    void renderOtherGuiWindow(ImGuiIO* io)override;

    void resize(int w, int h) override;
};
#endif // SCENE_FOG_H
