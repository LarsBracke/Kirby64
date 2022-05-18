#include "stdafx.h"
#include "SpikyScene.h"

#include "Components/ModelComponent.h"
#include "Materials/ColorMaterial.h"

void SpikyScene::Initialize()
{
	// Settings
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.clearColor = XMFLOAT4{ 0.66f, 0.66f, 0.66f, 1.f };

	// Sphere
	m_pOctaSphere = new GameObject();
	ModelComponent* pModelComponent = m_pOctaSphere->AddComponent(new ModelComponent(L"Meshes/OctaSphere.ovm"));
	m_pSpikyMaterial = MaterialManager::Get()->CreateMaterial<SpikyMaterial>();
	pModelComponent->SetMaterial(m_pSpikyMaterial);
	m_pOctaSphere->GetTransform()->Scale(5, 5, 5);
	AddChild(m_pOctaSphere);

	// Camera
	m_pCamera = new FreeCamera();
	AddChild(m_pCamera);
	SetActiveCamera(m_pCamera->GetComponent<CameraComponent>());
	GetActiveCamera()->GetTransform()->Rotate(10, 0, 0);
	GetActiveCamera()->GetTransform()->Translate(0, 0, -20.f);
}

void SpikyScene::Update()
{
	const float newRotation = m_RotationSpeed * m_SceneContext.pGameTime->GetTotal();
	m_pOctaSphere->GetTransform()->Rotate(0.f, newRotation, 0.f);
}

void SpikyScene::OnGUI()
{
	m_pSpikyMaterial->DrawImGui();
}
