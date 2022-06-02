#include "stdafx.h"
#include "MainGame.h"

#include "Helpers/GameManager.h"

/*GAME Content*/
#define GAME



#ifdef GAME
#include "Scenes/Game/MainMenu.h"
#include "Scenes/Game/PopStarOne.h"
#endif

#pragma endregion

//Game is preparing
void MainGame::OnGamePreparing(GameContext& gameContext)
{
	GameManager::Create(gameContext);

	gameContext.windowTitle = L"GP2 - Exam Project (2022) | (2DAE14) Bracke Lars";
}

void MainGame::Initialize()
{
#ifdef GAME
	//SceneManager::Get()->AddGameScene(new TrainingStage());
	SceneManager::Get()->AddGameScene(new MainMenu());
	SceneManager::Get()->AddGameScene(new PopStarOne());
#endif
}

LRESULT MainGame::WindowProcedureHook(HWND /*hWnd*/, UINT message, WPARAM wParam, LPARAM lParam)
{

	if(message == WM_KEYUP)
	{
		if ((lParam & 0x80000000) != 0x80000000)
			return -1;

		//[F1] Toggle Scene Info Overlay
		if(wParam == VK_F1)
		{
			const auto pScene = SceneManager::Get()->GetActiveScene();
			pScene->GetSceneSettings().Toggle_ShowInfoOverlay();
		}

		//[F2] Toggle Debug Renderer (Global)
		if (wParam == VK_F2)
		{
			DebugRenderer::ToggleDebugRenderer();
			return 0;

		}

		//[F3] Previous Scene
		if (wParam == VK_F3)
		{
			SceneManager::Get()->PreviousScene();
			return 0;

		}

		//[F4] Next Scene
		if (wParam == VK_F4)
		{
			SceneManager::Get()->NextScene();
			return 0;
		}

		//[F5] If PhysX Framestepping is enables > Next Frame	
		if (wParam == VK_F6)
		{
			const auto pScene = SceneManager::Get()->GetActiveScene();
			pScene->GetPhysxProxy()->NextPhysXFrame();
		}
	}
	

	return -1;
}
