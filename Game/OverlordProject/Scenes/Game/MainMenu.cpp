#include "stdafx.h"
#include "MainMenu.h"

#include "PopStarOne.h"

void MainMenu::Initialize()
{
	auto* pMenu = new GameObject();
	AddChild(pMenu);
	m_pMainMenu = pMenu->AddComponent(new SpriteComponent(L"Textures/MainMenu.png"));

	const InputAction actionStart{ Start, InputState::released, VK_SPACE, -1, 4096 };
	const InputAction actionExit{ Exit, InputState::released, VK_ESCAPE, -1, 8192 };
	m_SceneContext.pInput->AddInputAction(actionStart);
	m_SceneContext.pInput->AddInputAction(actionExit);
}

void MainMenu::Update()
{
	if (m_SceneContext.pInput->IsActionTriggered(Start))
	{
		if (auto* pLevelScene = SceneManager::Get()->GetNextScene())
		{
			SceneManager::Get()->RemoveGameScene(pLevelScene);
		}
		SceneManager::Get()->AddGameScene(new PopStarOne());
		SceneManager::Get()->NextScene();
	}
	if (m_SceneContext.pInput->IsActionTriggered(Exit))
	{
		PostQuitMessage(NULL);
	}
}

void MainMenu::OnSceneActivated()
{
	if (auto* pLevelScene = SceneManager::Get()->GetNextScene())
	{
		SceneManager::Get()->RemoveGameScene(pLevelScene);
	}
	SceneManager::Get()->AddGameScene(new PopStarOne());
}
