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

private:
	HealthComponent* m_pKirbyHealthComponent{ nullptr };

	UINT m_HealthAmount{ };
	std::vector<GameObject*> m_pHealthElements{ };
	const UINT m_ElementOffset{ 125 };

	GameObject* AddHealthElement(const XMFLOAT3& position);
};

