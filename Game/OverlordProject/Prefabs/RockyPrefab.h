#pragma once

#include "stdafx.h"

#include "Components/EnemyComponent.h"
#include "Helpers/GameManager.h"
#include "Character.h"

class RockyPrefab final : public GameObject
{
public:
	RockyPrefab();
	~RockyPrefab();

protected:
	void Initialize(const SceneContext&) override;
	void PostInitialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	void MoveToKirby(const SceneContext& sceneContext);

	GameManager* m_pGameManager{ nullptr };

	PxMaterial* m_pPhysicsMaterial{ nullptr };
	CharacterDesc m_CharacterDesc;

	ModelComponent* m_pModelComponent{ nullptr };
	ControllerComponent* m_pControllerComponent{ nullptr };
	EnemyComponent* m_pEnemyComponent{ nullptr };

	const float m_AttackRange{ 33.33f };
	const float m_Speed{ 3.33f };

	enum class AnimationState : UINT
	{
		Idle,
		Running
	};

	AnimationState m_CurrentAnimationState{ AnimationState::Idle };
	void SetAnimationState(AnimationState newState);
};

