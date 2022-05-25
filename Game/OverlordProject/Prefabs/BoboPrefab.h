#pragma once

#include "stdafx.h"

class BoboPrefab : public GameObject
{
public:
	BoboPrefab() = default;
	~BoboPrefab() override = default;
	BoboPrefab(const BoboPrefab& other) = delete;
	BoboPrefab(BoboPrefab&& other) noexcept = delete;
	BoboPrefab& operator=(const BoboPrefab& other) = delete;
	BoboPrefab& operator=(BoboPrefab&& other) noexcept = delete;

private:
	void Initialize(const SceneContext&) override;
	void PostInitialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	ModelComponent* m_pModelComponent{ nullptr };
	ControllerComponent* m_pControllerComponent{ nullptr };
	RigidBodyComponent* m_pRigidBodyComponent{ nullptr };
};

