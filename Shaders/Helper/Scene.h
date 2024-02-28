#ifndef SCENE_H
#define SCENE_H

#include <cfunctracer.h>
#include <glm/glm.hpp>

class Scene
{
protected:
    glm::mat4 m_model;
    glm::mat4 m_view;
    glm::mat4 m_projection;
	bool m_animate;
	std::shared_ptr<CTracer> m_trace;
public:
	int m_width;
	int m_height;

	Scene(std::shared_ptr<CTracer> tracer)
		:m_animate(true)
		, m_width(800)
		, m_height(600)
		, m_trace(tracer)
	{
	}

	virtual ~Scene() {}

	virtual void initScene() = 0;
	virtual void update(float t) = 0;
	virtual void render() = 0;
	virtual void resize(int w, int h) = 0;

	void setDimensions(int w, int h)
	{
		m_width = w;
		m_height = h;
	}
	void animate(bool value) { m_animate = value; }
	bool animating(void) { return m_animate; }
};

#endif // SCENE_H
