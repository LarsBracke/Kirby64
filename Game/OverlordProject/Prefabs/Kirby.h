#pragma once

#include "stdafx.h"

#include "Character.h"

class Kirby final : public GameObject
{
public:
	Kirby();
	~Kirby() override = default;
	//Kirby(const Kirby& other) = delete;
	//Kirby(Kirby&& other) noexcept = delete;
	//Kirby& operator=(const Kirby& other) = delete;
	//Kirby& operator=(Kirby&& other) noexcept = delete;

	void Initialize(const SceneContext&) override;
	void PostInitialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	void HandleMovement(const SceneContext& sceneContext);

private:
	ModelComponent* m_pModelComponent{ nullptr };
	CameraComponent* m_pCameraComponent{ nullptr };
	ControllerComponent* m_pController{ nullptr };
	RigidBodyComponent* m_pRigidBody{ nullptr };

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
		Jump
	};
};

