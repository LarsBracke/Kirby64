#include "stdafx.h"
#include "InGameHudPrefab.h"

void InGameHudPrefab::Initialize(const SceneContext&)
{
	auto* pHUD = new GameObject();
	AddChild(pHUD);
	pHUD->AddComponent(new SpriteComponent(L"Textures/UI_Background.png"));
	pHUD->GetTransform()->Translate(0, 720 - 193, 0);
}
