#include "stdafx.h"
#include "InGameHudPrefab.h"

#include "Helpers/GameManager.h"
#include "Components/SpriteComponent.h"

void InGameHudPrefab::Initialize(const SceneContext&)
{
	//auto* pHUD = new GameObject();
	//AddChild(pHUD);
	//pHUD->AddComponent(new SpriteComponent(L"Textures/UI_Background.png"));
	//pHUD->GetTransform()->Translate(0, 720 - 193, 0);
}

void InGameHudPrefab::PostInitialize(const SceneContext&)
{
	m_pKirbyHealthComponent = GameManager::Get()->GetKirby()->GetComponent<HealthComponent>();
	m_HealthAmount = m_pKirbyHealthComponent->GetHealth();

	AddHealthElement(XMFLOAT3{ 10, 720 - 75, 0 });
	AddHealthElement(XMFLOAT3{ 75, 720 - 75, 0 });
}

GameObject* InGameHudPrefab::AddHealthElement(const XMFLOAT3& position)
{
	auto* pElement = AddChild(new GameObject());
	pElement->AddComponent(new SpriteComponent(L"Textures/HealthElement.png"));
	pElement->GetTransform()->Translate(position);

	return pElement;
}
