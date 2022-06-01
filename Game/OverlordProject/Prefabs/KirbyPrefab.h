#pragma once

#include "stdafx.h"

#include "Character.h"
#include "Helpers/GameManager.h"

#include "Components/HealthComponent.h"

#include "Misc/AbilityType.h"

#include "Prefabs/FixedCamera.h"

class KirbyPrefab final : public GameObject
{
public:
	KirbyPrefab();
	~KirbyPrefab() override = default;
	//KirbyPrefab(const KirbyPrefab& other) = delete;
	//KirbyPrefab(KirbyPrefab&& other) noexcept = delete;
	//KirbyPrefab& operator=(const KirbyPrefab& other) = delete;
	//KirbyPrefab& operator=(KirbyPrefab&& other) noexcept = delete;

	void Initialize(const SceneContext&) override;
	void PostInitialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	void HandleMovement(const SceneContext& sceneContext);
	void HandleInhaling(const SceneContext& sceneContext);
	void HandleExhaling(const SceneContext& sceneContext);
	void HandleAudio(const SceneContext& sceneContext);

	void PushBack(const SceneContext& sceneContext, const GameObject* pOther);

private:
	GameManager* m_pGameManager{ nullptr };

	// Components
	ModelComponent* m_pModelComponent{ nullptr };
	CameraComponent* m_pCameraComponent{ nullptr };
	ControllerComponent* m_pController{ nullptr };
	RigidBodyComponent* m_pRigidBodyComponent{ nullptr };
	HealthComponent* m_pHealthComponent{ nullptr };
	ParticleEmitterComponent* m_pInhaleParticleSystem{ nullptr };


	// Movement
	PxMaterial* m_pPhysicsMaterial;
	CharacterDesc m_CharacterDesc;
	const unsigned int m_MaxJumpCount{ 5 };
	unsigned int m_JumpCount{ m_MaxJumpCount };

	XMFLOAT3 m_TotalVelocity{ };
	const float m_FallAcceleration{ };

	bool m_PushBack{ false };
	const float m_PushBackSpeed{ 200.0f };
	GameObject* m_pPushingObject{ nullptr };

	// Input
	enum InputIDs
	{
		MoveRight,
		MoveLeft,
		Jump,
		StartInhale,
		StopInhale,
		Exhale
	};

	// Camera
	FixedCamera* m_pCamera{ nullptr };
	XMFLOAT3 m_CameraOffset{ 2.5f, 5.f, 10.f };

	// Animations
	enum class AnimationState : UINT
	{
		Idle,
		Running,
		Jump,
		Inhaling
	};

	AnimationState m_CurrentAnimationState{ AnimationState::Idle };
	void SetAnimationState(AnimationState newState, bool forceAnimationChange = false);

	// Inhaling
	bool m_IsInhaling{ false };
	const float m_InhaleRange{ 15.0f };
	const float m_InhaleSpeed{ 10.0f };

	GameObject* m_pInhaledEnemy{ nullptr };

	// Abilities
	AbilityType* m_pActiveAbility{ nullptr };

	// Particles
	GameObject* m_pParticleObject{ nullptr };
	const float m_ParticleSpeed{ 20.0f };

	// Audio
	FMOD::Channel* m_pAudioChannel{ nullptr };
	FMOD::Channel* m_pSecondaryAudioChannel{ nullptr };
	FMOD::Sound* m_pSoundHurt{ nullptr };
	FMOD::Sound* m_pSoundJump{ nullptr };
	FMOD::Sound* m_pSoundInhale{ nullptr };
	FMOD::Sound* m_pSoundPunch{ nullptr };
};

