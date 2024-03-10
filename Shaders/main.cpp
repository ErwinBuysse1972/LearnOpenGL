#include "Scene.h"
#include "SceneRunner.h"
#include "SceneBasic_Attributes.h"
#include "SceneBasic_Uniform.h"
#include "SceneBasic_Uniformblock.h"
#include "SceneSeparable.h"
#include "SceneDiffuse.h"
#include "ScenePhong.h"
#include "scenetwoside.h"
#include "sceneflat.h"
#include "scenesubroutine.h"
#include "scenediscard.h"
#include "SceneMultilight.h"
#include "SceneDirectional.h"
#include "ScenePerFragment.h"
#include "SceneSpot.h"
#include "SceneCartoon.h"
#include "SceneFog.h"
#include "SceneBPR.h"
#include "SceneTexture.h"
#include "SceneMultiTex.h"
#include "scenealphatest.h"
#include "SceneNormal.h"
#include "SceneParallax.h"
#include "SceneReflectCube.h"
#include "SceneRefractCube.h"
#include "SceneRenderToTex.h"
#include "SceneMOW.h"
#include "SceneSamplerObj.h"
#include "SceneDiffIbl.h"
#include "SceneEdgeDetection.h"
#include "SceneToneMap.h"
#include "SceneGausianBlur.h"
#include "SceneHDRBloom.h"
#include "SceneGamma.h"
#include "SceneMultiSampleAntiAliasing.h"
#include "SceneDeferredShading.h"
#include "SceneSpaceAmbientOcclusion.h"
#include "SceneOrderIndependentTransparency.h"
#include "ScenePointSprites.h"
#include "SceneShadeWire.h"
#include "SceneSilhouette.h"
#include "SceneBezierCurve.h"
#include "SceneQuadTesselation2D.h"
#include "SceneTessTeapot.h"
#include "SceneTessTeapotDepth.h"
#include "SceneShadowMap.h"
#include "ScenePcfFilter.h"
#include <cfunctracer.h>
#include <memory>

std::shared_ptr<CTracer> m_Maintrace(nullptr);

static std::map< std::string, std::string > sceneData = {
        {"basic-attrib",		"Prints active attributes."},
        {"basic-uniform",		"Basic scene with a uniform variable."},
        {"basic-uniform-block", "Scene with a uniform block variable."},
        {"separable",			"Scene using separable shaders and program pipelines."},
        {"phong",				"Phong reflection mdoel (per-vertex)."},
        {"diffuse",				"Diffuse shading only"},
        {"discard",				"Example of discarding fragments"},
        {"flat",				"Flat shading"},
        {"subroutine",			"Using a shader subroutine"},
        {"two-side",			"Two-sided lighting"},
        {"directional",         "Directional light source" },
        {"fog",                 "Fog" },
        {"multi-light",         "Multiple light sources" },
        {"per-frag",            "Per-fragment shading" },
        {"spot",                "Spot light" },
        {"toon",                "Toon shading" },
        {"pbr",                 "Physically based rendering (PBR) shader"},
        {"alpha-test",          "Discard fragments based on an alpha test." },
        {"multi-tex",           "Multiple textures" },
        {"normal-map",          "Normal map" },
        {"proj-tex",            "Projected texture" },
        {"reflect-cube",        "Reflection with a cube map" },
        {"refract-cube",        "Refraction with a cube map" },
        {"render-to-tex",       "Render to a texture using framebuffer objects" },
        {"sampler-obj",         "Sampler objects " },
        {"texture",             "Basic texture mapping" },
        {"diff-ibl",            "Diffuse image based lighting"},
        {"parallax",            "Parallax mapping"},
        {"blur",                "Gaussian blur"},
        {"deferred",            "deferred rendering"},
        {"edge",                "edge detection filter"},
        {"gamma",               "gamma correction"},
        {"msaa",                "multisample anti-aliasing"},
        {"tone-map",            "tone mapping example."},
        {"hdr-bloom",           "bloom example with HDR tone mapping."},
        {"oit",                 "order independent transparency (requires OpenGL 4.3)"},
        {"ssao",                "Screen space ambieng occlusion example"},
        { "bez-curve",          "2D Bezier curve with tessellation shader" },
        { "point-sprite",       "Point sprites with the geometry shader" },
        { "quad-tess",          "Demonstrates how quad tessellation works" },
        { "shade-wire",         "Uses the geometry shader to draw a mesh over a shaded object" },
        { "silhouette",         "Uses the geometry shader to draw silhouette edges" },
        { "tess-teapot",        "Uses tessellation to draw a teapot" },
        { "tess-teapot-depth",  "Varies the amount of tessellation with depth" },
        { "ao",                 "Ambient occlusion from a texture" },
        { "jitter",             "Blur shadow map edges using a random jitter" },
        { "pcf",                "Blur shadow map edges using percentage-closer-filtering" },
        { "shadow-map",         "Simple shadow map" },
        { "shadow-volume",      "Shadow Volumes using geometry shader." },
        { "noise",              "just display the raw noise texture" },
        { "decay",              "decay of a teapot" },
        { "night-vision",       "night vision goggles" },
        { "paint",              "paint spatters on a teapot" },
        { "sky",                "clouds and sky" },
        { "wood",               "wood " },
        { "rust",               "rust"},
        {"mow",                 "Scene MOW"}
};

int main_CH02(int argc, char** argv)
{
	m_Maintrace = std::make_shared<CFileTracer>("", "ColoredTriangle.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
	CFuncTracer trace("main_CH02", m_Maintrace);

	std::string recipe = SceneRunner::ParseCLArgs(argc, argv, sceneData);
    SceneRunner runner("Chapter 2 - " + recipe, 500, 500);
    runner.Create();
    std::unique_ptr<Scene> scene = nullptr;
	if (recipe == "separable")                  scene = std::unique_ptr<Scene>(new SceneSeparable(m_Maintrace));
	else if (recipe == "basic-attrib")          scene = std::unique_ptr<Scene>(new SceneBasic_Attrib(m_Maintrace));
	else if (recipe == "basic-uniform")         scene = std::unique_ptr <Scene>(new SceneBasic_Uniform(m_Maintrace));
	else if (recipe == "basic-uniform-block")   scene = std::unique_ptr<Scene>(new SceneBasic_UniformBlock(m_Maintrace));
    if (scene)
    {
        runner.ShowWindow();
        return runner.Run(std::move(scene));
    }
    return -1;
}
int main_CH03(int argc, char** argv)
{
    m_Maintrace = std::make_shared<CFileTracer>("", "BasicsOfGLSL.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_CH03", m_Maintrace);

    std::string recipe = SceneRunner::ParseCLArgs(argc, argv, sceneData);

    SceneRunner runner("Chapter 3 - " + recipe, 1200, 725);
    runner.Create();
    std::unique_ptr<Scene> scene = nullptr;
    if (recipe == "diffuse")        scene = std::unique_ptr<SceneDiffuse>(new SceneDiffuse(m_Maintrace));
    if (recipe == "phong")          scene = std::unique_ptr<ScenePhong>(new ScenePhong(m_Maintrace));
    if (recipe == "two-side")       scene = std::unique_ptr<SceneTwoSite>(new SceneTwoSite(m_Maintrace));
    if (recipe == "flat")           scene = std::unique_ptr<SceneFlat>(new SceneFlat(m_Maintrace));
    if (recipe == "subroutine")     scene = std::unique_ptr<SceneSubroutine>(new SceneSubroutine(m_Maintrace));
    if (recipe == "discard")        scene = std::unique_ptr<SceneDiscard>(new SceneDiscard(m_Maintrace));

    if (scene)
    {
        runner.ShowWindow();
        return runner.Run(std::move(scene));
    }
    return -1;
}
int main_CH04(int argc, char** argv)
{
    m_Maintrace = std::make_shared<CFileTracer>("", "LightingAndShading.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_CH04", m_Maintrace);

    std::string recipe = SceneRunner::ParseCLArgs(argc, argv, sceneData);
    SceneRunner runner("Chapter 4 - " + recipe);
    runner.Create();
    std::unique_ptr<Scene> scene = nullptr;
    if (recipe == "multi-light")    scene = std::unique_ptr<Scene>(new SceneMultiLight(m_Maintrace));
    if (recipe == "directional")    scene = std::unique_ptr<Scene>(new SceneDirectional(m_Maintrace));
    if (recipe == "per-frag")       scene = std::unique_ptr<Scene>(new ScenePerFragment(m_Maintrace));
    if (recipe == "spot")           scene = std::unique_ptr<Scene>(new SceneSpot(m_Maintrace));
    if (recipe == "toon")           scene = std::unique_ptr<Scene>(new SceneCartoon(m_Maintrace));
    if (recipe == "fog")            scene = std::unique_ptr<Scene>(new SceneFog(m_Maintrace));
    if (recipe == "pbr")            scene = std::unique_ptr<Scene>(new SceneBPR(m_Maintrace));

    if (scene)
    {
        runner.ShowWindow();
        return runner.Run(std::move(scene));
    }
    return -1;
}
int main_CH05(int argc, char** argv)
{
    m_Maintrace = std::make_shared<CFileTracer>("", "UsingTextures.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_CH05", m_Maintrace);

    std::string recipe = SceneRunner::ParseCLArgs(argc, argv, sceneData);
    
    SceneRunner runner("Chapter 5 - " + recipe, 3000,1500,4);
    runner.Create();
    std::unique_ptr<Scene> scene = nullptr;

    if (recipe == "texture")            scene = std::unique_ptr<Scene>(new SceneTexture(m_Maintrace));
    else if (recipe == "multi-tex")     scene = std::unique_ptr<Scene>(new SceneMultiTex(m_Maintrace));
    else if (recipe == "alpha-test")    scene = std::unique_ptr<Scene>(new SceneAlphaTest(m_Maintrace));
    else if (recipe == "normal-map")    scene = std::unique_ptr<Scene>(new SceneNormalMap(m_Maintrace));
    else if (recipe == "parallax")      scene = std::unique_ptr<Scene>(new SceneParallax(m_Maintrace));
    else if (recipe == "reflect-cube")  scene = std::unique_ptr<Scene>(new SceneReflectCube(m_Maintrace));
    else if (recipe == "refract-cube")  scene = std::unique_ptr<Scene>(new SceneRefractCube(m_Maintrace));
    else if (recipe == "render-to-tex") scene = std::unique_ptr<Scene>(new SceneRenderToTex(m_Maintrace));
    else if (recipe == "sampler-obj")   scene = std::unique_ptr<Scene>(new SceneSamplerObj(m_Maintrace));
    else if (recipe == "diff-ibl")      scene = std::unique_ptr<Scene>(new SceneDiffIbl(m_Maintrace));
    if (scene)
    {
        runner.ShowWindow();
        return runner.Run(std::move(scene));
    }
    return -1;
}
int main_CH06(int argc, char** argv)
{
    m_Maintrace = std::make_shared<CFileTracer>("", "ImageProcessing.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_CH06", m_Maintrace);

    std::string recipe = SceneRunner::ParseCLArgs(argc, argv, sceneData);

    SceneRunner runner("Chapter 6 - " + recipe, 3000, 1500, 4);
    runner.Create();
    std::unique_ptr<Scene> scene = nullptr;

    if (recipe == "edge")           scene = std::unique_ptr<Scene>(new SceneEdgeDetection(m_Maintrace));
    else if (recipe == "tone-map")  scene = std::unique_ptr<Scene>(new SceneHDRLightToneMap(m_Maintrace));
    else if (recipe == "blur")      scene = std::unique_ptr<Scene>(new SceneGausianBlur(m_Maintrace));
    else if (recipe == "hdr-bloom") scene = std::unique_ptr<Scene>(new SceneHDRBloom(m_Maintrace));
    else if (recipe == "gamma")     scene = std::unique_ptr<Scene>(new SceneGamma(m_Maintrace));
    else if (recipe == "msaa")      scene = std::unique_ptr<Scene>(new SceneMultiSampleAntiAliasing(m_Maintrace));
    else if (recipe == "deferred")  scene = std::unique_ptr<Scene>(new SceneDeferredShading(m_Maintrace));
    else if (recipe == "ssao")      scene = std::unique_ptr<Scene>(new SceneSpaceAmbientOcclusion(m_Maintrace));
    else if (recipe == "oit")       scene = std::unique_ptr<Scene>(new SceneOrderIndependentTransparency(m_Maintrace));
    if (scene)
    {
        runner.ShowWindow();
        return runner.Run(std::move(scene));
    }
    return -1;
}
int main_CH07(int argc, char** argv)
{
    m_Maintrace = std::make_shared<CFileTracer>("", "GeometryAndTesselationShaders.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_CH07", m_Maintrace);

    std::string recipe = SceneRunner::ParseCLArgs(argc, argv, sceneData);

    SceneRunner runner("Chapter 7 - " + recipe, 3000, 1500, 4);
    runner.Create();
    std::unique_ptr<Scene> scene = nullptr;

    if (recipe == "point-sprite")           scene = std::unique_ptr<Scene>(new ScenePointSprites(m_Maintrace));
    else if (recipe == "shade-wire")        scene = std::unique_ptr<Scene>(new SceneShadeWire(m_Maintrace));
    else if (recipe == "silhouette")        scene = std::unique_ptr<Scene>(new SceneSilhouette(m_Maintrace));
    else if (recipe == "bez-curve")         scene = std::unique_ptr<Scene>(new SceneBezierCurve(m_Maintrace));
    else if (recipe == "quad-tess")         scene = std::unique_ptr<Scene>(new SceneQuadTesselation2D(m_Maintrace));
    else if (recipe == "tess-teapot")       scene = std::unique_ptr<Scene>(new SceneTessTeapot(m_Maintrace));
    else if (recipe == "tess-teapot-depth") scene = std::unique_ptr<Scene>(new SceneTessTeapotDepth(m_Maintrace));
    if (scene)
    {
        runner.ShowWindow();
        return runner.Run(std::move(scene));
    }
    return -1;

}
int main_CH08(int argc, char** argv)
{
    m_Maintrace = std::make_shared<CFileTracer>("", "Shadows.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_CH08", m_Maintrace);

    std::string recipe = SceneRunner::ParseCLArgs(argc, argv, sceneData);

    SceneRunner runner("Chapter 8 - " + recipe, 3000, 1500, 4);
    runner.Create();
    std::unique_ptr<Scene> scene = nullptr;

    if (recipe == "shadow-map") scene = std::unique_ptr<Scene>(new SceneShadowMap(m_Maintrace));
    if (recipe == "pcf") scene = std::unique_ptr<Scene>(new ScenePcfFilter(m_Maintrace));
    if (scene)
    {
        runner.ShowWindow();
        return runner.Run(std::move(scene));
    }
    return -1;
}
int main_CH09(int argc, char** argv)
{
    m_Maintrace = std::make_shared<CFileTracer>("", "NoiseInShadows.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_CH09", m_Maintrace);

    std::string recipe = SceneRunner::ParseCLArgs(argc, argv, sceneData);

    SceneRunner runner("Chapter 9 - " + recipe, 3000, 1500, 4);
    runner.Create();
    std::unique_ptr<Scene> scene = nullptr;

    if (scene)
    {
        runner.ShowWindow();
        return runner.Run(std::move(scene));
    }
    return -1;
}
int main_CH10(int argc, char** argv)
{
    m_Maintrace = std::make_shared<CFileTracer>("", "Animations.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_CH10", m_Maintrace);

    std::string recipe = SceneRunner::ParseCLArgs(argc, argv, sceneData);

    SceneRunner runner("Chapter 10 - " + recipe, 3000, 1500, 4);
    runner.Create();
    std::unique_ptr<Scene> scene = nullptr;

    if (scene)
    {
        runner.ShowWindow();
        return runner.Run(std::move(scene));
    }
    return -1;
}
int main_CH11(int argc, char** argv)
{
    m_Maintrace = std::make_shared<CFileTracer>("", "ComputeShaders.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_CH11", m_Maintrace);

    std::string recipe = SceneRunner::ParseCLArgs(argc, argv, sceneData);

    SceneRunner runner("Chapter 11 - " + recipe, 3000, 1500, 4);
    runner.Create();
    std::unique_ptr<Scene> scene = nullptr;

    if (scene)
    {
        runner.ShowWindow();
        return runner.Run(std::move(scene));
    }
    return -1;
}

int main_MOW(int argc, char** argv)

{
    m_Maintrace = std::make_shared<CFileTracer>("", "MOWScene.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_MOW", m_Maintrace);

    std::string recipe = SceneRunner::ParseCLArgs(argc, argv, sceneData);

    SceneRunner runner("MOW - " + recipe, 1200, 725, 0);
    runner.Create();
    std::unique_ptr<Scene> scene = nullptr;

    if (recipe == "mow")
        scene = std::unique_ptr<Scene>(new SceneMOW(m_Maintrace));
    if (scene)
    {
        runner.ShowWindow();
        return runner.Run(std::move(scene));
    }
    return -1;
}

int main(int argc, char* argv[])
{
	int iret = 0;
    // Working with GLSL programs
	iret = main_CH02(argc, argv);

    if (iret < 0) iret = main_CH03(argc, argv);     // Basics of GLSL shaders
    if (iret < 0) iret = main_CH04(argc, argv);     // Light and shading
    if (iret < 0) iret = main_CH05(argc, argv);     // using textures
    if (iret < 0) iret = main_CH06(argc, argv);     // Image processing and screen space techniques
    if (iret < 0) iret = main_CH07(argc, argv);     // Geometry and tesselation shaders
    if (iret < 0) iret = main_CH08(argc, argv);     // Shadows 
    if (iret < 0) iret = main_CH09(argc, argv);     // Noise in shadders
    if (iret < 0) iret = main_CH10(argc, argv);     // Paricle systems and animation
    if (iret < 0) iret = main_CH11(argc, argv);     // compute shaders
    if (iret < 0) iret = main_MOW(argc, argv);      // MOW Testings
	return iret;
}
 