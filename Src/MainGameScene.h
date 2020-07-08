/**
* @file MainGameScene.h
*/
#ifndef MAINGAMESCENE_H_INCLUDED
#define MAINGAMESCENE_H_INCLUDED
#include "GLFWEW.h"
#include "Texture.h"
#include "Shader.h"
#include "MeshList.h"
#include "Actor.h"

/**
* プレイヤーが操作するアクター
*/
class PlayerActor : public Actor
{
public:
	virtual ~PlayerActor() = default;
	virtual void Update(float deltaTIme) override;
};

/**
* メインゲーム画面
*/
class MainGameScene
{
public:
	MainGameScene() = default;
	~MainGameScene() = default;

	bool Initialize();
	void ProcessInput();
	void Update();
	void Render();
	void Finalize();

private:
	MeshList meshList;

	Texture::Image2D texGround;
	Texture::Image2D texHuman;
	Texture::Image2D texWallWidth;
	Texture::Image2D texWallHeight;

	Shader::Program progSimple;
	Shader::Program progLighting;
	Shader::LightList lights;

	float angleY = 0;
	glm::vec3 viewPos;

	PlayerActor player;

};
#endif //MAINGAMESCENE_H_INCLUDED