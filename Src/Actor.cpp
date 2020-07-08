/**
* @file Actor.cpp
*/
#include "Actor.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

/**
* アクターを初期化する
*
* @param mesh アクターとして表示するメッシュID
* @param tex メッシュに貼るテクスチャのID
* @param hp アクターの耐久力
* @param pos メッシュを表示する座標
* @param rot メッシュを表示する回転角度
* @param scale メッシュを表示する大きさ
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
* アクターを破棄する
*/
void Actor::Finalize()
{
	health = 0;
}

/**
* アクターを更新する
*
* @param deltaTime 経過時間
*/
void Actor::Update(float deltaTime)
{
	position += velocity * deltaTime;
	colWorld.origin += colLocal.origin + position;
	colWorld.size = colLocal.size;
}

/**
* 利用可能なアクターを取得する
*
* @param actorList 検索対象のアクターのリスト
*
* @return 利用可能なアクターのポインタ
*		利用可能なアクターが見つからなければnullptr
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
* アクターの状態を更新する
*
* @param actorList 更新するアクターのリスト
* @param deltaTime 前回の更新からの経過時間
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
* アクターを描画する
*
* @param actorList 描画するアクターリスト
* @param shader 描画に使用するシェーダー オブジェクト
* @param meshList 描画に使用するメッシュリスト
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
* アクターリストを空にする
*
* @param actorList 空にするアクターのリスト
*/
void ClearActorList(std::vector<Actor*>& actorList)
{
	for (auto& actor : actorList)
	{
		delete actor;
	}
	actorList.clear();
}