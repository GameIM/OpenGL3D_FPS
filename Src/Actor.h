/**
* Actor.h
*/
#ifndef ACTOR_H_INCLUDED
#define ACTOR_H_INCLUDED
#include <GL/glew.h>
#include "Shader.h"
#include "MeshList.h"
#include <glm/vec3.hpp>
#include <vector>

/**
* 直方体
*/
struct Rect
{
	glm::vec3 origin;
	glm::vec3 size;
};

/**
* シーンに配置するオブジェクト
*/
class Actor
{
public:
	Actor() = default;
	virtual ~Actor() = default;

	void Initialize(int mesh, GLuint tex, int hp,
		const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);
	void Finalize();
	virtual void Update(float deltaTime);

public:
	int mesh = 0;
	GLuint texture = 0;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::vec3 velocity;
	int health = 0;
	Rect colLocal;
	Rect colWorld;
};

Actor* FindAvailableActor(std::vector<Actor*>&);
void UpdateActorList(std::vector<Actor*>&, float);
void RenderActorList(std::vector<Actor*>&, Shader::Program&, MeshList&);
void ClearActorList(std::vector<Actor*>&);

#endif //ACTOR_H_INCLUDED