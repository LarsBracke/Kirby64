#include "stdafx.h"
#include "InGameHudPrefab.h"

#include "Helpers/GameManager.h"

void InGameHudPrefab::Initialize(const SceneContext&)
{
	auto* pHUD = new GameObject();
	AddChild(pHUD);
	pHUD->AddComponent(new SpriteComponent(L"Textures/UI_Background.png"));
	pHUD->GetTransform()->Translate(0, 720 - 193, 0);
}

void InGameHudPrefab::PostInitialize(const SceneContext&)
{
	m_pKirbyHealthComponent = GameManager::Get()->GetKirby()->GetComponent<HealthComponent>();
}
