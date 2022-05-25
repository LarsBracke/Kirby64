#pragma once

#include "stdafx.h"
#include "Character.h"

#include "Helpers/GameManager.h"

class BoboPrefab : public GameObject
{
public:
	BoboPrefab();
	~BoboPrefab() override = default;
	BoboPrefab(const BoboPrefab& other) = delete;
	BoboPrefab(BoboPrefab&& other) noexcept = delete;
	BoboPrefab& operator=(const BoboPrefab& other) = delete;
	BoboPrefab& operator=(BoboPrefab&& other) noexcept = delete;

private:
	void Initialize(const SceneContext&) override;
	void PostInitialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

	void MoveToKirby(const SceneContext& sceneContext);

private:
	GameManager* m_pGameManager{ nullptr };

	ModelComponent* m_pModelComponent{ nullptr };
	ControllerComponent* m_pControllerComponent{ nullptr };
	RigidBodyComponent* m_pRigidBodyComponent{ nullptr };

	PxMaterial* m_pPhysicsMaterial{ nullptr };
	CharacterDesc m_CharacterDesc;

	const float m_AttackRange{ 25.f };
	const float m_Speed{ 5.f };
};

