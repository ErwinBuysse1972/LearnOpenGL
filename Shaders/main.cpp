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
        {"parallax",            "Parallax mapping"} };

int main_CH02(int argc, char** argv)
{
	m_Maintrace = std::make_shared<CFileTracer>("", "ColoredTriangle.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
	CFuncTracer trace("main_CH02", m_Maintrace);

	std::string recipe = SceneRunner::ParseCLArgs(argc, argv, sceneData);
    SceneRunner runner("Chapter 2 - " + recipe, 500, 500);
    std::unique_ptr<Scene> scene = nullptr;
	if (recipe == "separable") {
        scene = std::unique_ptr<Scene>(new SceneSeparable(m_Maintrace));
	}
	else if (recipe == "basic-attrib") {
		scene = std::unique_ptr<Scene>(new SceneBasic_Attrib(m_Maintrace));
	}
	else if (recipe == "basic-uniform") {
		scene = std::unique_ptr <Scene>(new SceneBasic_Uniform(m_Maintrace));
	}
	else if (recipe == "basic-uniform-block") {
		scene = std::unique_ptr<Scene>(new SceneBasic_UniformBlock(m_Maintrace));
	}
    if (scene)
    {
        runner.ShowWindow();
        return runner.Run(std::move(scene));
    }
    return -1;
}
int main_CH03(int argc, char** argv)
{
    m_Maintrace = std::make_shared<CFileTracer>("", "ColoredTriangle.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_CH03", m_Maintrace);

    std::string recipe = SceneRunner::ParseCLArgs(argc, argv, sceneData);

    SceneRunner runner("Chapter 3 - " + recipe, 500, 500);
    std::unique_ptr<Scene> scene = nullptr;
    if (recipe == "diffuse")
        scene = std::unique_ptr<SceneDiffuse>(new SceneDiffuse(m_Maintrace));
    if (recipe == "phong")
        scene = std::unique_ptr<ScenePhong>(new ScenePhong(m_Maintrace));
    if (recipe == "two-side")
        scene = std::unique_ptr<SceneTwoSite>(new SceneTwoSite(m_Maintrace));
    if (recipe == "flat")
        scene = std::unique_ptr<SceneFlat>(new SceneFlat(m_Maintrace));
    if (recipe == "subroutine")
        scene = std::unique_ptr<SceneSubroutine>(new SceneSubroutine(m_Maintrace));
    if (recipe == "discard")
        scene = std::unique_ptr<SceneDiscard>(new SceneDiscard(m_Maintrace));

    if (scene)
    {
        runner.ShowWindow();
        return runner.Run(std::move(scene));
    }
    return -1;
}
int main_CH04(int argc, char** argv)
{
    m_Maintrace = std::make_shared<CFileTracer>("", "ColoredTriangle.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_CH04", m_Maintrace);

    std::string recipe = SceneRunner::ParseCLArgs(argc, argv, sceneData);
    SceneRunner runner("Chapter 4 - " + recipe);
    std::unique_ptr<Scene> scene = nullptr;
    if (recipe == "multi-light")
        scene = std::unique_ptr<Scene>(new SceneMultiLight(m_Maintrace));
    if (recipe == "directional")
        scene = std::unique_ptr<Scene>(new SceneDirectional(m_Maintrace));
    if (recipe == "per-frag")
        scene = std::unique_ptr<Scene>(new ScenePerFragment(m_Maintrace));
    if (recipe == "spot")
        scene = std::unique_ptr<Scene>(new SceneSpot(m_Maintrace));
    if (recipe == "toon")
        scene = std::unique_ptr<Scene>(new SceneCartoon(m_Maintrace));
    if (recipe == "fog")
        scene = std::unique_ptr<Scene>(new SceneFog(m_Maintrace));
    if (recipe == "pbr")
        scene = std::unique_ptr<Scene>(new SceneBPR(m_Maintrace));

    if (scene)
    {
        runner.ShowWindow();
        return runner.Run(std::move(scene));
    }
    return -1;
}
int main_CH05(int argc, char** argv)
{
    m_Maintrace = std::make_shared<CFileTracer>("", "ColoredTriangle.log", TracerLevel::TRACER_DEBUG_LEVEL, true, true, true, true);
    CFuncTracer trace("main_CH04", m_Maintrace);

    std::string recipe = SceneRunner::ParseCLArgs(argc, argv, sceneData);
    
    SceneRunner runner("Chapter 5 - " + recipe, 800,600,0);
    std::unique_ptr<Scene> scene = nullptr;

    if (recipe == "texture")
        scene = std::unique_ptr<Scene>(new SceneTexture(m_Maintrace));
    else if (recipe == "multi-tex")
        scene = std::unique_ptr<Scene>(new SceneMultiTex(m_Maintrace));
    else if (recipe == "alpha-test")
        scene = std::unique_ptr<Scene>(new SceneAlphaTest(m_Maintrace));
    else if (recipe == "normal-map")
        scene = std::unique_ptr<Scene>(new SceneNormalMap(m_Maintrace));
    else if (recipe == "parallax")
        scene = std::unique_ptr<Scene>(new SceneParallax(m_Maintrace));
    else if (recipe == "reflect-cube")
        scene = std::unique_ptr<Scene>(new SceneReflectCube(m_Maintrace));
    else if (recipe == "refract-cube")
        scene = std::unique_ptr<Scene>(new SceneRefractCube(m_Maintrace));
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

    if (iret < 0)
    {
        // Basics of GLSL shaders
        iret = main_CH03(argc, argv);
    }

    if (iret < 0)
    {
        // Light and shading
        iret = main_CH04(argc, argv);
    }

    if (iret < 0)
    {
        // using textures
        iret = main_CH05(argc, argv);
    }
	return iret;
}
