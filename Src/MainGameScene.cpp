/**
* @file MainGameScene.cpp
*/
#include "MainGameScene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

/**
* �v���C���[�̏�Ԃ��X�V����
*
* @param deltaTime �o�ߎ���
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
* ������
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

	//�e�N�X�`���̓ǂݍ���
	texGround.Reset(Texture::LoadImage2D("Res/Ground.tga"));
	texHuman.Reset(Texture::LoadImage2D("Res/Human.tga"));
	texWallWidth.Reset(Texture::LoadImage2D("Res/wallWidth.tga"));
	texWallHeight.Reset(Texture::LoadImage2D("Res/wallHeight.tga"));

	//���̐ݒ�
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
* ���͂̔��f
*/
void MainGameScene::ProcessInput()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();

	//�v���C���[���ړ�����
	player.velocity = glm::vec3(0);
	if (window.IsKeyPressed(GLFW_KEY_A))
	{
		const glm::mat4x4 matRotLeft =
			glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));//��]�s��
		const glm::vec3 LtFrontVector = matRotLeft * glm::vec4(0, 0, -1, 1);//�O���x�N�g��
		player.rotation.y += glm::radians(0.03f);//��]�̑���
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
* ��Ԃ̍X�V
*/
void MainGameScene::Update()
{
	const float deltaTime = (float)GLFWEW::Window::Instance().DeltaTime();

	angleY += glm::radians(1.0f);
	if (angleY > glm::radians(360.0f))
	{
		angleY -= glm::radians(360.0f);
	}

	//�v���C���[�̏�Ԃ��X�V
	player.Update(deltaTime);

	//���_�̈ʒu���X�V
	const glm::vec3 viewOffset(0,2,0);
	viewPos = player.position + viewOffset;
}

/**
* �`��
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

	//���W�ϊ��s����쐬����
	const glm::mat4x4 matProj =
		glm::perspective(glm::radians(45.0f), 1000.0f / 600.0f, 0.1f, 500.0f);
	const glm::mat4x4 matRotY =
		glm::rotate(glm::mat4(1), player.rotation.y, glm::vec3(0, 1, 0));
	const glm::vec3 viewVector = matRotY * glm::vec4(0, 2, -2, 1);
	const glm::mat4x4 matView =
		glm::lookAt(viewPos,player.position + viewVector,glm::vec3(0,1,0));

	progLighting.SetViewProjectionMatrix(matProj * matView);

	//������ݒ肷��
	progLighting.SetLightList(lights);

	progLighting.BindTexture(0, player.texture);
	progLighting.Draw(meshList.Get(player.mesh),
		player.position, player.rotation, player.scale);

	progLighting.BindTexture(0, texGround.Get());
	//�n�ʂ�\��
	{
		progLighting.Draw(meshList.Get(0),
			glm::vec3(0,-3,0), glm::vec3(0), glm::vec3(1));
	}


	//�ǂ�\��(��)
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


	//�ǂ�\��(�c)
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
* �I��
*/
void MainGameScene::Finalize()
{

}