#pragma once

#include "stdafx.h"

class EnemyComponent : public BaseComponent
{
public:
	EnemyComponent() = default;
	~EnemyComponent() override = default;

protected:
	void Initialize(const SceneContext& /*sceneContext*/) override { };
};

