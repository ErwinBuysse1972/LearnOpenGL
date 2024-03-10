#include "GLUtils.h"
#include "SceneRunner.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_opengl3.h"

SceneRunner::SceneRunner(const std::string& title, int width, int height, int samples)
    : m_debug(true)
    , m_io(nullptr)
    , m_samples(samples)
    , m_height(height)
    , m_width(width)
    , m_title(title)
{
}

SceneRunner::~SceneRunner()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool SceneRunner::Create(void)
{
    if (!glfwInit())
        exit(EXIT_FAILURE);

    const char* glsl_version = "#version 130";
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    ::glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    ::glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    ::glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    if (m_debug)
        ::glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    if (m_samples > 0)
        ::glfwWindowHint(GLFW_SAMPLES, m_samples);

    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
    if (m_window == nullptr)
    {
        std::cerr << "Unable to create OPENGL context." << std::endl;
        ::glfwTerminate();
        exit(EXIT_FAILURE);
    }

    ::glfwMakeContextCurrent(m_window);
    ::glfwSwapInterval(0);
    ::glfwSetWindowUserPointer(m_window, this);
/*
    ::glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        SceneRunner* runner = static_cast<SceneRunner*>(glfwGetWindowUserPointer(window));
        if (runner)
            runner->GetScene()->resize(width, height);
        });
        */
    ::glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y) {
            SceneRunner* runner = static_cast<SceneRunner*>(glfwGetWindowUserPointer(window));
            if (runner)
                runner->ProcessMouse(runner->GetScene(), x, y);
            });
      

    // setup Dear imgui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)m_io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable the keyboard controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable the Gamepad controls

    SetGuiIo(&io);

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup platform/renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
  //IM_ASSERT(font != nullptr);

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
    m_Scene = scene.get();

	Mainloop(m_window, std::move(scene));

	if (m_debug)
		::glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 1, GL_DEBUG_SEVERITY_NOTIFICATION, -1, "End debug");

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


	while (!::glfwWindowShouldClose(m_window) 
        && !::glfwGetKey(m_window, GLFW_KEY_ESCAPE))
	{
        ProcessKeys(scene.get());
        
        GLUtils::checkForOpenGLError(__FILE__, __LINE__);

		scene->update(float(::glfwGetTime()));
		scene->render();

        ::glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        bool show_demo_window = scene->ShowDemoWindwow();
        if (show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);
            scene->showingDemoWindow(show_demo_window);
        }

        scene->renderGuiWindow(m_io);

        if (scene->ShowOtherWindow())
            scene->renderOtherGuiWindow(m_io);
/*
		int state = ::glfwGetKey(m_window, GLFW_KEY_SPACE);
		if (state == GLFW_PRESS)
			scene->animate(!scene->animating());
  */  
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
    }
}
void SceneRunner::ProcessKeys(Scene* pScene)
{
    try
    {
        if (pScene->KeysProcessNeeded() == true)
        {
            std::vector<unsigned long> keys = pScene->GetKeysOfInterest();
            for (unsigned long key : keys)
            {
                if (glfwGetKey(m_window, key) == GLFW_PRESS)
                    pScene->OnKeyPress("SceneRunner", key);
            }
        }
    }
    catch (const std::exception&) { }
}

void SceneRunner::ProcessMouse(Scene* pScene, double xpos, double ypos)
{
    try
    {
        if (pScene->MouseProcessNeeded() == true)
        {
            unsigned long eBtnStatus = static_cast<unsigned long>(MouseButtonStatus::nopress);
            if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) eBtnStatus |= static_cast<unsigned long>(MouseButtonStatus::rightPressed);
            if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) eBtnStatus |= static_cast<unsigned long>(MouseButtonStatus::leftPressed);
            if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) eBtnStatus |= static_cast<unsigned long>(MouseButtonStatus::wheelPressed);

            if (eBtnStatus != static_cast<unsigned long>(MouseButtonStatus::nopress))
                pScene->OnMouseMove("SceneRunner", (float)xpos, (float)ypos, static_cast<MouseButtonStatus>(eBtnStatus));
        }
    }
    catch (const std::exception&) {  }
}

