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
};

