#ifndef SCENE_H
#define SCENE_H

#include <Camera.h>
#include <cfunctracer.h>
#include <imgui.h>
#include <functional>
#include <glm/glm.hpp>
#include <map>
using glm::mat4;
using glm::vec3;
using glm::vec4;


struct LightInfo {
    vec4 Position; // Light position in eye coords.
    vec3 La;       // Ambient light intensity
    vec3 Ld;       // Diffuse light intensity
    vec3 Ls;       // Specular light intensity
};

struct LightInfoS {
    vec4 Position;  // Light position in eye coords.
    vec3 La;        // Ambient light intesity
    vec3 L;         // Diffuse and specular light intensity
};


struct MaterialInfo {
    vec3 Ka;            // Ambient reflectivity
    vec3 Kd;            // Diffuse reflectivity
    vec3 Ks;            // Specular reflectivity
    float Shininess;    // Specular shininess factor
};

enum class MouseButtonStatus
{
    nopress = 0,
    rightPressed = 1,
    leftPressed = 2,
    wheelPressed = 4
};


class onKeyPress
{
public:
    bool SubscribeKeypress(std::function<void(const std::string& id, unsigned long keypress)> evt, std::vector<unsigned long> keys, const std::string& eventId);
    bool UnSubscribeKeypress(const std::string& eventId);
    bool UnSubscribeAllKeypress(void);
    bool OnKeyPress(const std::string& id, unsigned long keypress);
    std::vector<unsigned long> GetKeysOfInterest(void) { return m_keysToCheck; }
    bool KeysProcessNeeded(void) { return m_onKeysPressed.size() > 0; }
private:
    std::vector<unsigned long> m_keysToCheck;
    std::map<std::string, std::vector<unsigned long>> m_idVsKeys;
    std::map<std::string, std::function<void(const std::string& id, unsigned long keypress)>> m_onKeysPressed;

    void updateKeysOfInterest(void);
};

class onMouseMove
{
public:
    onMouseMove()
        : m_current_x(-1.0)
        , m_current_y(-1.0)
        , m_bfirstMove(true)
    {}
    bool SubscribeMouse(std::function<void(const std::string& id, float dx, float dy, float startX, float startY, MouseButtonStatus button)> evt, const std::string& eventId);
    bool UnSubscribeMouse(const std::string& eventId);
    bool UnSubscribeAllMouse(void);
    bool OnMouseMove(const std::string& id, float xpos, float ypos, MouseButtonStatus btn);
    bool MouseProcessNeeded(void) { return m_evOnMouseMove.size() > 0; }
private:
    float m_current_x;
    float m_current_y;
    bool   m_bfirstMove;
    std::map<std::string, std::function<void(const std::string& id, float dx, float dy, float startX, float startY, MouseButtonStatus button)>> m_evOnMouseMove;
};

class Scene : public onMouseMove, public onKeyPress
{
protected:
    glm::mat4 m_model;
    glm::mat4 m_view;
    glm::mat4 m_projection;
	bool m_animate;
    bool m_show_demo_window;
    bool m_show_other_window;
	std::shared_ptr<CTracer> m_trace;
public:
	int m_width;
	int m_height;

	Scene(std::shared_ptr<CTracer> tracer)
		: m_animate(true)
		, m_width(800)
		, m_height(600)
        , m_show_demo_window(true)
        , m_show_other_window(false)
		, m_trace(tracer)
	{
	}

	virtual ~Scene() {}

	virtual void initScene() = 0;
	virtual void update(float t) = 0;
	virtual void render() = 0;
    virtual void renderGuiWindow(ImGuiIO* io) = 0;
    virtual void renderOtherGuiWindow(ImGuiIO* io) = 0;
	virtual void resize(int w, int h) = 0;

	void setDimensions(int w, int h)
	{
		m_width = w;
		m_height = h;
	}
	void animate(bool value) { m_animate = value; }
	bool animating(void) { return m_animate; }
    void showingDemoWindow(bool value) { m_show_demo_window = value; }
    bool ShowDemoWindwow(void) { return m_show_demo_window; }
    bool ShowOtherWindow(void) { return m_show_other_window; }
};

#endif // SCENE_H
