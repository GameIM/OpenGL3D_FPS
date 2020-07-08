/**
* @file MainGameScene.cpp
*/
#include "MainGameScene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

/**
* プレイヤーの状態を更新する
*
* @param deltaTime 経過時間
*/
void PlayerActor::Update(float deltaTime)
{
	if (health <= 0)
	{
		rotation.x -= glm::radians(45.0f) * deltaTime;
		if (rotation.x < glm::radians(-90.0f))
		{
			rotation.x = glm::radians(-90.0f);
		}
	}
	Actor::Update(deltaTime);
}

/**
* 初期化
*/
bool MainGameScene::Initialize()
{
	if (!meshList.Allcate())
	{
		return false;
	}
	progSimple.Reset(Shader::BuildFromFile("Res/Simple.vert", "Res/Simple.frag"));
	progLighting.Reset(Shader::BuildFromFile(
		"Res/FragmentLighting.vert", "Res/FragmentLighting.frag"));

	//テクスチャの読み込み
	texGround.Reset(Texture::LoadImage2D("Res/Ground.tga"));
	texHuman.Reset(Texture::LoadImage2D("Res/Human.tga"));
	texWallWidth.Reset(Texture::LoadImage2D("Res/wallWidth.tga"));
	texWallHeight.Reset(Texture::LoadImage2D("Res/wallHeight.tga"));

	//光の設定
	lights.ambient.color = glm::vec3(0.05f, 0.1f, 0.1f) * 14.0f;
	lights.directional.direction = glm::normalize(glm::vec3(-5,-50,-15));
	lights.directional.color = glm::vec3(1, 1, 1);

	player.Initialize(1, texHuman.Get(), 10,
		glm::vec3(2,-3,0), glm::vec3(0,0,0), glm::vec3(1));

	player.colLocal =
	{ glm::vec3(-0.5f,0.0f,-0.5f), glm::vec3(1.0f,1.7f,1.0f) };

	return true;
}

/**
* 入力の反映
*/
void MainGameScene::ProcessInput()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();

	//プレイヤーを移動する
	player.velocity = glm::vec3(0);
	if (window.IsKeyPressed(GLFW_KEY_A))
	{
		const glm::mat4x4 matRotLeft =
			glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));//回転行列
		const glm::vec3 LtFrontVector = matRotLeft * glm::vec4(0, 0, -1, 1);//前方ベクトル
		player.rotation.y += glm::radians(0.03f);//回転の速さ
	}
	else if (window.IsKeyPressed(GLFW_KEY_D))
	{
		const glm::mat4x4 matRotRight =
			glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, -1, 0));
		player.rotation.y -= glm::radians(0.03f);
	}
	if (window.IsKeyPressed(GLFW_KEY_W))
	{

		player.velocity.z = -1;
	}
	else if (window.IsKeyPressed(GLFW_KEY_S))
	{
		player.velocity.z = 1;
	}
	if (player.velocity.x || player.velocity.z)
	{
		player.velocity = glm::normalize(player.velocity);
	}
	/*
	if (!window.IsKeyPressed(GLFW_KEY_SPACE))
	{
		player.rotation.y = std::atan2(-player.velocity.z, player.velocity.x);
		player.rotation.y -= glm::radians(90.0f);
	}
	*/
	
	const float speed = 10.0f;
	player.velocity *= speed;
}

/**
* 状態の更新
*/
void MainGameScene::Update()
{
	const float deltaTime = (float)GLFWEW::Window::Instance().DeltaTime();

	angleY += glm::radians(1.0f);
	if (angleY > glm::radians(360.0f))
	{
		angleY -= glm::radians(360.0f);
	}

	//プレイヤーの状態を更新
	player.Update(deltaTime);

	//視点の位置を更新
	const glm::vec3 viewOffset(0,2,0);
	viewPos = player.position + viewOffset;
}

/**
* 描画
*/
void MainGameScene::Render()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glfwPollEvents();

	progLighting.Use();

	meshList.BindVertexArray();

	//座標変換行列を作成する
	const glm::mat4x4 matProj =
		glm::perspective(glm::radians(45.0f), 1000.0f / 600.0f, 0.1f, 500.0f);
	const glm::mat4x4 matRotY =
		glm::rotate(glm::mat4(1), player.rotation.y, glm::vec3(0, 1, 0));
	const glm::vec3 viewVector = matRotY * glm::vec4(0, 2, -2, 1);
	const glm::mat4x4 matView =
		glm::lookAt(viewPos,player.position + viewVector,glm::vec3(0,1,0));

	progLighting.SetViewProjectionMatrix(matProj * matView);

	//光源を設定する
	progLighting.SetLightList(lights);

	progLighting.BindTexture(0, player.texture);
	progLighting.Draw(meshList.Get(player.mesh),
		player.position, player.rotation, player.scale);

	progLighting.BindTexture(0, texGround.Get());
	//地面を表示
	{
		progLighting.Draw(meshList.Get(0),
			glm::vec3(0,-3,0), glm::vec3(0), glm::vec3(1));
	}


	//壁を表示(横)
	progLighting.BindTexture(0, texWallWidth.Get());
	{
		progLighting.Draw(meshList.Get(3),
			glm::vec3(-10,-5.5,-20), glm::vec3(0), glm::vec3(1));
		progLighting.Draw(meshList.Get(3),
			glm::vec3(10,-5.5,-20), glm::vec3(0), glm::vec3(1));
		progLighting.Draw(meshList.Get(3),
			glm::vec3(-10, -5.5, 20), glm::vec3(0), glm::vec3(1));
		progLighting.Draw(meshList.Get(3),
			glm::vec3(10, -5.5, 20), glm::vec3(0), glm::vec3(1));
	}


	//壁を表示(縦)
	progLighting.BindTexture(0, texWallHeight.Get());
	{
		progLighting.Draw(meshList.Get(4),
			glm::vec3(19,-3.9,-10), glm::vec3(0), glm::vec3(1));
		progLighting.Draw(meshList.Get(4),
			glm::vec3(19,-3.9,10), glm::vec3(0), glm::vec3(1));
		progLighting.Draw(meshList.Get(4),
			glm::vec3(-19,-3.9, -10), glm::vec3(0), glm::vec3(1));
		progLighting.Draw(meshList.Get(4),
			glm::vec3(-19,-3.9,10), glm::vec3(0), glm::vec3(1));
	}
}

/**
* 終了
*/
void MainGameScene::Finalize()
{

}