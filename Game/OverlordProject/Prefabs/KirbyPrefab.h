#pragma once

#include "stdafx.h"

#include "Character.h"
#include "Helpers/GameManager.h"

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

private:
	GameManager* m_pGameManager{ nullptr };

	ModelComponent* m_pModelComponent{ nullptr };
	CameraComponent* m_pCameraComponent{ nullptr };
	ControllerComponent* m_pController{ nullptr };
	RigidBodyComponent* m_pRigidBodyComponent{ nullptr };

	PxMaterial* m_pPhysicsMaterial;
	CharacterDesc m_CharacterDesc;
	const unsigned int m_MaxJumpCount{ 5 };
	unsigned int m_JumpCount{ m_MaxJumpCount };

	XMFLOAT3 m_TotalVelocity{ };
	const float m_FallAcceleration{ };

	enum InputIDs
	{
		MoveRight,
		MoveLeft,
		Jump,
		Inhale
	};

	const float m_InhaleRange{ 15.0f };
	const float m_InhaleSpeed{ 10.0f };
};

