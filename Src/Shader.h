/**
* @file Shader.h
*/
#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct Mesh;//メッシュ構造体の先行宣言

namespace Shader{

	GLuint Build(const GLchar* vsCode, const GLchar* fsCode);
	GLuint BuildFromFile(const char* vsPath, const char* fsPath);

/**
* 環境光
*/
	struct AmbientLight
	{
		glm::vec3 color;
	};

/**
* 指向性ライト
*/
	struct DirectionalLight
	{
		glm::vec3 direction;
		glm::vec3 color;
	};

/**
* ライトをまとめた構造体
*/
	struct LightList
	{
		AmbientLight ambient;
		DirectionalLight directional;

		void Init();
	};

/**
* シェーダー プログラム
*/
	class Program
	{
	public:
		Program();
		explicit Program(GLuint programId);
		~Program();

		void Reset(GLuint programId);
		bool IsNull() const;
		void Use();
		void BindTexture(GLuint, GLuint);
		void SetLightList(const LightList&);
		void SetViewProjectionMatrix(const glm::mat4&);
		void Draw(const Mesh& mesh,
			const glm::vec3& translate, const glm::vec3& rotate, const glm::vec3& scale);

	private:
		GLuint id = 0;//プログラムID

		//uniform変数の位置
		GLint locMatMVP = -1;
		GLint locDirLightDir = -1;
		GLint locDirLightCol = -1;
		GLint locAmbLightCol = -1;

		glm::mat4 matVP = glm::mat4(1);//ビュープロジェクション行列
		LightList lights;
	};
}//shader namespace
#endif //SHADER_H_INCLUDED
