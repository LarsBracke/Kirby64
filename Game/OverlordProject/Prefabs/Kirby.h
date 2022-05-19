#pragma once

#include "stdafx.h"

#include "Character.h"

class Kirby : public GameObject
{
public:
	Kirby();
	~Kirby() = default;
	//Kirby(const Kirby& other) = delete;
	//Kirby(Kirby&& other) noexcept = delete;
	//Kirby& operator=(const Kirby& other) = delete;
	//Kirby& operator=(Kirby&& other) noexcept = delete;

	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	ModelComponent* m_pModelComponent{ nullptr };
	CameraComponent* m_pCameraComponent{ nullptr };
	ControllerComponent* m_pController{ nullptr };

	PxMaterial* m_pPhysicsMaterial;
	CharacterDesc m_CharacterDesc;

	XMFLOAT3 m_TotalVelocity{ };
	const float m_FallAcceleration{ };

	enum InputIDs
	{
		MoveRight,
		MoveLeft,
		Jump
	};
};

