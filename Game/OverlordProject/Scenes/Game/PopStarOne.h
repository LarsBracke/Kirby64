#pragma once

#include "stdafx.h"
#include "Prefabs/FixedCamera.h"

class PopStarOne final : public GameScene
{
public:
	PopStarOne() : GameScene(L"PopStarOne") { }
	~PopStarOne();
	PopStarOne(const PopStarOne& other) = delete;
	PopStarOne(PopStarOne&& other) noexcept = delete;
	PopStarOne& operator=(const PopStarOne& other) = delete;
	PopStarOne& operator=(PopStarOne&& other) noexcept = delete;

private:
	void Initialize() override;
	void PostInitialize() override;
	void Update() override;
	void OnGUI() override;

	void OnSceneActivated() override;

	void HandleCameraMovement();

	GameObject* SpawnBobo(const XMFLOAT3& spawnLocation);
	GameObject* SpawnRocky(const XMFLOAT3& spawnLocation);
	void SpawnBackGroundBoxes();
	GameObject* AddBridge(const XMFLOAT3& location);
	GameObject* AddPathBox(const XMFLOAT3& location);

private:
	// Camera
	FixedCamera* m_pCamera{ nullptr };
	CameraComponent* m_pCameraComponent{ nullptr };
	const XMFLOAT3 m_LightDirection{ 0.740129888f, -0.597205281f, 0.309117377f };
	XMFLOAT3 m_CameraPosition{ };
	const XMFLOAT3 m_CameraRotation{ 20.0f, -20.0f, 0.0f };
	const XMFLOAT3 m_CameraOffset{ 15.0f, 10.0f, -30.0f };
	const float m_CameraSnapSpeed{ 2.5f };

	// Lighting
	const XMFLOAT3 m_LightOffset{ -69.f, 35.f, 0.f  };
	const XMFLOAT3 m_LightRotation{ .433f, -0.597f, 1.192f };

	// Audio
	FMOD::Channel* m_pMusicChannel{ nullptr };
	FMOD::Sound* m_pMusic{ nullptr };
};

