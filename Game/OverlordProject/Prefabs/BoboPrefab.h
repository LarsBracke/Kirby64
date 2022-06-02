#pragma once

#include "stdafx.h"
#include "Character.h"

#include "Helpers/GameManager.h"

#include "Components/EnemyComponent.h"

class BoboPrefab : public GameObject
{
public:
	BoboPrefab();
	~BoboPrefab();
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
	EnemyComponent* m_pEnemyComponent{ nullptr };
	ParticleEmitterComponent* m_pParticleEmitterComponent{ nullptr };

	PxMaterial* m_pPhysicsMaterial{ nullptr };
	CharacterDesc m_CharacterDesc;

	const float m_AttackRange{ 25.f };
	const float m_DashRange{ 5.f };
	const float m_Speed{ 5.f };
	const float m_DashSpeed{ 30.f };

	enum class AnimationState : UINT
	{
		Idle,
		Running
	};

	AnimationState m_CurrentAnimationState{ AnimationState::Idle };
	void SetAnimationState(AnimationState newState);
};

