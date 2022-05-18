#pragma once

#include "Materials/UberMaterial.h"

class UberMaterialScene final : public GameScene
{
public:
	UberMaterialScene(): GameScene(L"UberMaterialScene") { }
	~UberMaterialScene() override = default;

	UberMaterialScene(const UberMaterialScene& other) = delete;
	UberMaterialScene(UberMaterialScene&& other) noexcept = delete;
	UberMaterialScene& operator=(const UberMaterialScene& other) = delete;
	UberMaterialScene& operator=(UberMaterialScene&& other) noexcept = delete;

	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	GameObject* m_pSphere{ nullptr };
	UberMaterial* m_pUberMaterial{ nullptr };

	const float m_RotationSpeed{ 10.f };

	GameObject* m_pCamera{ nullptr };
};

