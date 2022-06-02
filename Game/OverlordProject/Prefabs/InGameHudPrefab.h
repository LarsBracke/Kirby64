#pragma once

#include "Components/HealthComponent.h"

class InGameHudPrefab : public GameObject
{
public:
	InGameHudPrefab() = default;
	~InGameHudPrefab() override = default;

protected:
	void Initialize(const SceneContext&) override;
	void PostInitialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	HealthComponent* m_pKirbyHealthComponent{ nullptr };

	UINT m_HealthAmount{ };
	GameObject* AddHealthElement(const XMFLOAT3& position);

	std::string m_Text{"Health"};
	SpriteFont* m_pFont;
	XMFLOAT2 m_TextPosition{ 65, 672 };
	XMFLOAT4 m_TextColor {0,0,0,1};
};

