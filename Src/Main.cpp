/**
* @file Main.cpp
*/
#include "GLFWEW.h"
#include "TitleScene.h"
#include "MainGameScene.h"

MeshList meshList;

/**
* エントリーポイント
*/
int main(void)
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	if (!window.Init(1000,600,"OpenGL3D_FPS"))
	{
		return 1;
	}

	TitleScene* pTitleScene = new TitleScene;
	MainGameScene* pMainGameScene = nullptr;
	if (!pTitleScene || !pTitleScene->Initialize())
	{
		delete pTitleScene;
		return 1;
	}


	//メインループ
	window.InitTimer();
	while(!window.ShouldClose())
	{
		window.UpdateTimer();
		if (pTitleScene)
		{
			pTitleScene->ProcessInput();
			pTitleScene->Update();
			pTitleScene->Render();
			if (pTitleScene->IsFinish())
			{
				pTitleScene->Finalize();
				delete pTitleScene;
				pTitleScene = nullptr;
				pMainGameScene = new MainGameScene;
				if (pMainGameScene && !pMainGameScene->Initialize())
				{
					break;
				}
			}
		}
		else if (pMainGameScene)
		{
			pMainGameScene->ProcessInput();
			pMainGameScene->Update();
			pMainGameScene->Render();
		}
		window.SwapBuffers();
	}
	if (pTitleScene)
	{
		pTitleScene->Finalize();
		delete pTitleScene;
	}
	if (pMainGameScene)
	{
		pMainGameScene->Finalize();
		delete pMainGameScene;
	}

	return 0;
}