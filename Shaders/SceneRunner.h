#ifndef SCENERUNNER_H
#define SCENERUNNER_H

#include <glad/glad.h>
#include "Scene.h"
#include <GLFW/glfw3.h>
#include "GLUtils.h"

#define WIN_WIDTH		800
#define WIN_HEIGHT		600

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <imgui.h>



class SceneRunner
{
private:
	GLFWwindow* m_window;
    GLFWwindow* m_guiWindow;
	int m_fbw;
	int m_fbh;
	bool m_debug;
    ImGuiIO *m_io;
    Scene* m_Scene;

    /*creation parameters*/
    int m_samples;
    int m_height;
    int m_width;
    std::string m_title;

	static void PrintHelpInfo(const char* exefile, std::map<std::string, std::string>& sceneData);
	void Mainloop(GLFWwindow* window, std::unique_ptr<Scene> scene);
    void SetGuiIo(ImGuiIO* io) { m_io = io; }
    void ProcessKeys(Scene* pScene);
public:
	SceneRunner(const std::string& title, int width = WIN_WIDTH, int height = WIN_HEIGHT, int samples = 0);
    virtual ~SceneRunner();

    bool Create(void);

    void ShowWindow(void);
	int Run(std::unique_ptr<Scene> scene);
    void ProcessMouse(Scene* pScene, double xpos, double ypos);
    Scene* GetScene(void) { return m_Scene; }
    static std::string ParseCLArgs(int argc, char** argv, std::map<std::string, std::string>& sceneData);

};

#endif// SCENERUNNER_H
