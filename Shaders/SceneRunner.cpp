#include "GLUtils.h"
#include "SceneRunner.h"

SceneRunner::SceneRunner(const std::string& title, int width, int height, int samples)
	: m_debug(true)
{
	if (!glfwInit())
		exit(EXIT_FAILURE);

	::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	::glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	::glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    ::glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	if (m_debug)
		::glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	if (samples > 0)
		::glfwWindowHint(GLFW_SAMPLES, samples);

	m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (m_window == nullptr)
	{
		std::cerr << "Unable to create OPENGL context." << std::endl;
		::glfwTerminate();
		exit(EXIT_FAILURE);
	}

	::glfwMakeContextCurrent(m_window);

	// Get framebuffer size
	::glfwGetFramebufferSize(m_window, &m_fbw, &m_fbh);

	// Load the openfl functions
	if (!gladLoadGL()) { exit(-1); }

	GLUtils::dumpGLInfo();

	// Initialization
	::glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	if (m_debug)
	{
		::glDebugMessageCallback(GLUtils::debugCallback, nullptr);
		::glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
		::glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0, GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Start debugging");
	}
}
void SceneRunner::ShowWindow(void)
{
    glfwShowWindow(m_window);
}
int SceneRunner::Run(std::unique_ptr<Scene> scene)
{
	Mainloop(m_window, std::move(scene));

	if (m_debug)
		::glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 1, GL_DEBUG_SEVERITY_NOTIFICATION, -1, "End debug");

	::glfwTerminate();
	return EXIT_SUCCESS;
}
std::string SceneRunner::ParseCLArgs(int argc, char** argv, std::map<std::string, std::string>& sceneData)
{
	if (argc < 2)
	{
		PrintHelpInfo(argv[0], sceneData);
		exit(EXIT_FAILURE);
	}

	std::string recipeName = argv[1];
	auto it = sceneData.find(recipeName);
	if (it == sceneData.end())
	{
		printf("Unknown recipe: %s \n\n", recipeName.c_str());
		PrintHelpInfo(argv[0], sceneData);
		exit(EXIT_FAILURE);
	}

	return recipeName;
}
void SceneRunner::PrintHelpInfo(const char* exefile, std::map<std::string, std::string>& sceneData)
{
	printf("Usage : %s recipe-name\n\n", exefile);
	printf("Recipe names :");
	for (auto& it : sceneData)
		printf("  %11s : %s \n", it.first.c_str(), it.second.c_str());
}
void SceneRunner::Mainloop(GLFWwindow* window, std::unique_ptr<Scene> scene)
{
	scene->setDimensions(m_fbw, m_fbh);
	scene->initScene();
	scene->resize(m_fbw, m_fbh);

	while (!::glfwWindowShouldClose(m_window) && !::glfwGetKey(m_window, GLFW_KEY_ESCAPE))
	{
		GLUtils::checkForOpenGLError(__FILE__, __LINE__);

		scene->update(float(::glfwGetTime()));
		scene->render();
		::glfwSwapBuffers(m_window);

		::glfwPollEvents();
		int state = ::glfwGetKey(m_window, GLFW_KEY_SPACE);
		if (state == GLFW_PRESS)
			scene->animate(!scene->animating());
	}
}

