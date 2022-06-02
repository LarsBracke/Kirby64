#include "stdafx.h"
#include "InGameHudPrefab.h"

#include "Helpers/GameManager.h"
#include "Components/SpriteComponent.h"

void InGameHudPrefab::Initialize(const SceneContext& /*sceneContext*/)
{
	auto* pHUD = new GameObject();
	AddChild(pHUD);

	auto* pHealthIcon = new GameObject();
	pHealthIcon->AddComponent(new SpriteComponent(L"Textures/Health.png"));
	pHealthIcon->GetTransform()->Translate(5, 665, 0.5f);
	pHealthIcon->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	pHUD->AddChild(pHealthIcon);

	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt");
}

void InGameHudPrefab::PostInitialize(const SceneContext&)
{
	m_pKirbyHealthComponent = GameManager::Get()->GetKirby()->GetComponent<HealthComponent>();
	m_HealthAmount = m_pKirbyHealthComponent->GetHealth();


}

void InGameHudPrefab::Update(const SceneContext&)
{
	m_Text = std::to_string(m_pKirbyHealthComponent->GetHealth());
	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(m_Text), m_TextPosition, m_TextColor);
}

GameObject* InGameHudPrefab::AddHealthElement(const XMFLOAT3& position)
{
	auto* pElement = AddChild(new GameObject());
	pElement->AddComponent(new SpriteComponent(L"Textures/HealthElement.png"));
	pElement->GetTransform()->Translate(position);

	return pElement;
}
