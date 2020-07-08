/**
* @file TitleScene.h
*/
#ifndef TITLESCENE_H_INCLUDED
#define TITLESCENE_H_INCLUDED
#include "GLFWEW.h"
#include "Texture.h"
#include "Shader.h"
#include "MeshList.h"

/**
* ÉÅÉCÉìÉQÅ[ÉÄâÊñ 
*/
class TitleScene
{
public:
	TitleScene() = default;
	~TitleScene() = default;

	bool Initialize();
	void ProcessInput();
	void Update();
	void Render();
	void Finalize();

	bool IsFinish() const;

private:
	MeshList meshList;

	Texture::Image2D texLogo;
	Texture::Image2D texBackGround;

	Shader::Program progSimple;

	float timer;
	bool isFinish;
};
#endif //TITLESCENE_H_INCLUDED
