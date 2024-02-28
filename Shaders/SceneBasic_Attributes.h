#ifndef SCENE_BASIC_ATTRIBUTES_H
#define SCENE_BASIC_ATTRIBUTES_H
#include "Helper/Scene.h"
#include "glslprogram.h"

class SceneBasic_Attrib : public Scene
{
private:
	GLuint		m_voaHandle;
	GLSLProgram m_prog;

	void compileShaderProgram();
public:
	SceneBasic_Attrib(std::shared_ptr<CTracer> tracer);
	virtual ~SceneBasic_Attrib() {};

	void initScene() override;
	void update(float t) override;
	void render() override;

	void resize(int w, int h) override;
};

#endif //SCENE_BASIC_ATTRIBUTES_H


