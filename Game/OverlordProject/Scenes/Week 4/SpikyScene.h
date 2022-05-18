#pragma once

#include "Materials/SpikyMaterial.h"

class SpikyScene : public GameScene
{
public:
	SpikyScene() : GameScene(L"SpikyScene") { }
	~SpikyScene() override = default;
	SpikyScene(const SpikyScene& other) = delete;
	SpikyScene(SpikyScene&& other) noexcept = delete;
	SpikyScene& operator=(const SpikyScene& other) = delete;
	SpikyScene& operator=(SpikyScene&& other) noexcept = delete;

	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	GameObject* m_pOctaSphere{ nullptr };
	GameObject* m_pCamera{ nullptr };
	SpikyMaterial* m_pSpikyMaterial{ nullptr };
	const float m_RotationSpeed{ 5.f };
};

