#include "stdafx.h"
#include "BoboPrefab.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

void BoboPrefab::Initialize(const SceneContext&)
{
	/*model*/
	auto* pModel = new GameObject();
	AddChild(pModel);
	m_pModelComponent = pModel->AddComponent(new ModelComponent(L"Meshes/Bobo.ovm"));

	auto* pBodyMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pBodyMaterial->SetDiffuseTexture(L"Textures/Bobo_Body.png");

	auto* pFeetMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pFeetMaterial->SetDiffuseTexture(L"Textures/Bobo_Feet.png");

	auto* pEyesMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pEyesMaterial->SetDiffuseTexture(L"Textures/Bobo_Eyes.png");

	m_pModelComponent->SetMaterial(pFeetMaterial, 0);
	m_pModelComponent->SetMaterial(pBodyMaterial, 1);
	m_pModelComponent->SetMaterial(pEyesMaterial, 2);
	pModel->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	pModel->GetTransform()->Rotate(0, 90, 0);

	/*controller*/

	/*Collider*/
}

void BoboPrefab::PostInitialize(const SceneContext&)
{
}

void BoboPrefab::Update(const SceneContext&)
{
}
