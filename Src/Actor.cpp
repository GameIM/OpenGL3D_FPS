/**
* @file Actor.cpp
*/
#include "Actor.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

/**
* �A�N�^�[������������
*
* @param mesh �A�N�^�[�Ƃ��ĕ\�����郁�b�V��ID
* @param tex ���b�V���ɓ\��e�N�X�`����ID
* @param hp �A�N�^�[�̑ϋv��
* @param pos ���b�V����\��������W
* @param rot ���b�V����\�������]�p�x
* @param scale ���b�V����\������傫��
*/
void Actor::Initialize(int mesh, GLuint tex, int hp,
	const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
{
	this->mesh = mesh;
	texture = tex;
	position = pos;
	rotation = rot;
	this->scale = scale;
	health = hp;
}

/**
* �A�N�^�[��j������
*/
void Actor::Finalize()
{
	health = 0;
}

/**
* �A�N�^�[���X�V����
*
* @param deltaTime �o�ߎ���
*/
void Actor::Update(float deltaTime)
{
	position += velocity * deltaTime;
	colWorld.origin += colLocal.origin + position;
	colWorld.size = colLocal.size;
}

/**
* ���p�\�ȃA�N�^�[���擾����
*
* @param actorList �����Ώۂ̃A�N�^�[�̃��X�g
*
* @return ���p�\�ȃA�N�^�[�̃|�C���^
*		���p�\�ȃA�N�^�[��������Ȃ����nullptr
*/
Actor* FindAvailableActor(std::vector<Actor*>& actorList)
{
	for (auto& actor : actorList)
	{
		if (actor && actor->health <= 0)
		{
			return actor;
		}
	}
	return nullptr;
}

/**
* �A�N�^�[�̏�Ԃ��X�V����
*
* @param actorList �X�V����A�N�^�[�̃��X�g
* @param deltaTime �O��̍X�V����̌o�ߎ���
*/
void UpdateActorList(std::vector<Actor*>& actorList, float deltaTime)
{
	for (auto& actor : actorList)
	{
		if (actor && actor->health > 0)
		{
			actor->Update(deltaTime);
		}
	}
}

/**
* �A�N�^�[��`�悷��
*
* @param actorList �`�悷��A�N�^�[���X�g
* @param shader �`��Ɏg�p����V�F�[�_�[ �I�u�W�F�N�g
* @param meshList �`��Ɏg�p���郁�b�V�����X�g
*/
void RenderActorList(std::vector<Actor*>& actorList,
	Shader::Program& shader, MeshList& meshList)
{
	for (auto& actor : actorList)
	{
		if (actor && actor->health > 0)
		{
			shader.BindTexture(0, actor->texture);
			shader.Draw(meshList.Get(actor->mesh),
				actor->position, actor->rotation, actor->scale);
		}
	}
}

/**
* �A�N�^�[���X�g����ɂ���
*
* @param actorList ��ɂ���A�N�^�[�̃��X�g
*/
void ClearActorList(std::vector<Actor*>& actorList)
{
	for (auto& actor : actorList)
	{
		delete actor;
	}
	actorList.clear();
}