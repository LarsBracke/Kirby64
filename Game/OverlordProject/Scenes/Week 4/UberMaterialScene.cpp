#include "stdafx.h"
#include "UberMaterialScene.h"

#include "Materials/UberMaterial.h"

void UberMaterialScene::Initialize()
{
	// Scene settings
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.clearColor = XMFLOAT4{ 0.25f, 0.25f,0.25f,1.f };

	// Sphere
	m_pSphere = new GameObject();
	m_pSphere->GetTransform()->Scale(5, 5, 5);
	ModelComponent* pModelComponent = m_pSphere->AddComponent(new ModelComponent(L"Meshes/Sphere.ovm"));
	m_pUberMaterial = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pModelComponent->SetMaterial(m_pUberMaterial);
	AddChild(m_pSphere);

	// Textures
	m_pUberMaterial->SetDiffuseTexture(L"Textures/Skulls_Diffusemap.tga");
	m_pUberMaterial->SetSpecularIntensityTexture(L"Textures/Skulls_Heightmap.tga");
	m_pUberMaterial->SetNormalTexture(L"Textures/Skulls_Normalmap.tga");
	m_pUberMaterial->SetEnvironmentTexture(L"Textures/Sunol_Cubemap.dds");
	m_pUberMaterial->SetOpacityTexture(L"Textures/Specular_Level.jpg");

	// Camera
	m_pCamera = new FreeCamera();
	AddChild(m_pCamera);
	SetActiveCamera(m_pCamera->GetComponent<CameraComponent>());
	GetActiveCamera()->GetTransform()->Rotate(10, 0, 0);
	GetActiveCamera()->GetTransform()->Translate(0, 1, -20);
}

void UberMaterialScene::Update()
{
	const float elapsedTime = m_SceneContext.pGameTime->GetTotal();
	m_pSphere->GetTransform()->Rotate(0, elapsedTime * m_RotationSpeed, 0);
}

void UberMaterialScene::OnGUI()
{
	m_pUberMaterial->DrawImGui();
}
