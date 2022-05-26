#pragma once

#include "stdafx.h"
#include "Misc/AbilityType.h"

class EnemyComponent : public BaseComponent
{
public:
	EnemyComponent() = default;
	~EnemyComponent() override = default;

	void Kill();

	void SetAbilityType(AbilityType newType) { m_AbilityType = newType; }
	AbilityType GetAbilityType() const { return m_AbilityType; }

protected:
	void Initialize(const SceneContext& /*sceneContext*/) override { };

private:
	AbilityType m_AbilityType{ AbilityType::None };
};

