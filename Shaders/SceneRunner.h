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


class SceneRunner
{
private:
	GLFWwindow* m_window;
	int m_fbw;
	int m_fbh;
	bool m_debug;

	static void PrintHelpInfo(const char* exefile, std::map<std::string, std::string>& sceneData);
	void Mainloop(GLFWwindow* window, std::unique_ptr<Scene> scene);

public:
	SceneRunner(const std::string& title, int width = WIN_WIDTH, int height = WIN_HEIGHT, int samples = 0);

    void ShowWindow(void);
	int Run(std::unique_ptr<Scene> scene);
	static std::string ParseCLArgs(int argc, char** argv, std::map<std::string, std::string>& sceneData);

};

#endif// SCENERUNNER_H
