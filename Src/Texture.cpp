/**
* @file Texture.cpp
*/
#include "Texture.h"
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>

//テクスチャ関連の関数やクラスを格納する名前空間
namespace Texture{

/**
* 2Dテクスチャを作成する
*
* @param width テクスチャの幅(ピクセル数)
* @param height テクスチャの高さ(ピクセル数)
* @param data テクスチャデータへのポインタ
*
* @retval 0以外 作成したテクスチャ オブジェクトのID
* @retval 0 テクスチャの作成に失敗
*/
	GLuint CreateImage2D(GLsizei width, GLsizei height, const GLvoid* data,
		GLenum format, GLenum type)
	{
		GLuint id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
			width, height, 0, format, type, data);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		const GLenum result = glGetError();
		if (result != GL_NO_ERROR)//テクスチャの作成したか
		{
			std::cerr << "ERROR: テクスチャの作成に失敗(0x" << std::hex << result << ")";
			glBindTexture(GL_TEXTURE_2D, 0);
			glDeleteTextures(1, &id);
			return 0;
		}

		//テクスチャのパラメータを設定する
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//1要素の画像データの場合、(R,R,R,1)として読み取られるようにする
		if (format == GL_RED)
		{
			const GLint swizzle[] = { GL_RED,GL_RED,GL_RED,GL_ONE };
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		return id;
	}

/**
* ファイルから2Dテクスチャを読み込む
*
* @param path 2Dテクスチャとして読み込むファイル名
*
* @retval 0以外 作成したテクスチャ オブジェクトのID
* @retval 0 テクスチャの作成に失敗
*/
	GLuint LoadImage2D(const char* path)
	{
		//TGAヘッダを読み込む
		std::basic_ifstream<uint8_t> ifs;
		ifs.open(path, std::ios_base::binary);
		std::vector <uint8_t> buf1(1000000);
		ifs.rdbuf()->pubsetbuf(buf1.data(), buf1.size());

		uint8_t tgaHeader[18];
		ifs.read(tgaHeader, 18);

		//イメージIDを読み飛ばす
		ifs.ignore(tgaHeader[0]);

		//カラーマップを読み飛ばす
		if (tgaHeader[1])
		{
			const int colorMapLength = tgaHeader[5] + tgaHeader[6] * 0x100;
			const int colorMapEntrySize = tgaHeader[7];
			const int colorMapSize = colorMapLength * colorMapEntrySize / 8;
			ifs.ignore(colorMapSize);
		}

		//画像データを読み込む
		const int width = tgaHeader[12] + tgaHeader[13] * 0x100;
		const int height = tgaHeader[14] + tgaHeader[15] * 0x100;
		const int pixelDepth = tgaHeader[16];
		const int imageSize = width * height * pixelDepth / 8;
		std::vector<uint8_t> buf(imageSize);
		ifs.read(buf.data(), imageSize);

		//画像データが「上から下」で格納されていた場合、上下を入れ替える
		if (tgaHeader[17] & 0x20)
		{
			const int lineSize = width * pixelDepth / 8;
			std::vector<uint8_t> tmp(imageSize);
			std::vector<uint8_t>::iterator source = buf.begin();
			std::vector<uint8_t>::iterator destination = tmp.end();
			for (int i = 0; i < height; i++)
			{
				destination -= lineSize;
				std::copy(source, source + lineSize, destination);
				source += lineSize;
			}
			buf.swap(tmp);
		}

		//読み込んだ画像データからテクスチャを作成する
		GLenum type = GL_UNSIGNED_BYTE;
		GLenum format = GL_BGRA;
		if (tgaHeader[2] == 3)//圧縮なし8ビット形式なら
		{
			format = GL_RED;
		}
		if (tgaHeader[16] == 24)//圧縮なし24ビット形式なら
		{
			format = GL_BGR;
		}
		else if (tgaHeader[16] == 16)//圧縮なし16ビット形式なら
		{
			type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
		}
		//読み込んだ画像データからテクスチャを作成する
		return CreateImage2D(width, height, buf.data(),format,type);
	}

/**
* コンストラクタ
*
* @param texId テクスチャ オブジェクトのID
*/
	Image2D::Image2D(GLuint texId)
	{
		Reset(texId);
	}

/**
* デストラクタ
*/
	Image2D::~Image2D()
	{
		glDeleteTextures(1, &id);
	}

/**
* テクスチャ オブジェクトを設定する
*
* @param texId テクスチャ オブジェクトのID
*/
	void Image2D::Reset(GLuint texid)
	{
		glDeleteTextures(1, &id);
		id = texid;
	}

/**
* テクスチャ オブジェクトが設定されているか調べる
*
* @param true 設定されていない
* @param false 設定されている
*/
	bool Image2D::IsNull() const
	{
		return id == 0;
	}

/**
* テクスチャ オブジェクトを取得する
*
* @return テクスチャ オブジェクトのID
*/
	GLuint Image2D::Get() const
	{
		return id;
	}
}//namespace Texture