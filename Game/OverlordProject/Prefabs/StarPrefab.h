#pragma once

#include "stdafx.h"

class StarPrefab : public GameObject
{
public:
	StarPrefab(const XMFLOAT3& launchDirection);
	~StarPrefab() override = default;

protected:
	void Initialize(const SceneContext&) override;
	void PostInitialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	// Components
	ModelComponent* m_pModelComponent{ nullptr };

	// Movement
	XMFLOAT3 m_LaunchDirection{ };
	const float m_LaunchSpeed{ 0.5f };
};

