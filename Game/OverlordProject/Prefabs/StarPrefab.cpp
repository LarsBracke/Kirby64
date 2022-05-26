#include "stdafx.h"
#include "StarPrefab.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

StarPrefab::StarPrefab(const XMFLOAT3& launchDirection)
	: m_LaunchDirection(launchDirection)
{ }

void StarPrefab::Initialize(const SceneContext&)
{
	// Model
	m_pModelComponent = AddComponent(new ModelComponent(L"Meshes/Star.ovm"));
	auto* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Star.png");
	m_pModelComponent->SetMaterial(pMaterial);
	GetTransform()->Scale(0.1f, 0.1f, 0.1f);
}

void StarPrefab::PostInitialize(const SceneContext&)
{
	//m_pModelComponent->GetAnimator()->SetAnimation(0);
}

void StarPrefab::Update(const SceneContext&)
{
	m_pModelComponent->GetAnimator()->Play();

	const XMVECTOR currentPosition = XMLoadFloat3(&GetTransform()->GetPosition());
	const XMVECTOR launchDirection = XMLoadFloat3(&m_LaunchDirection);
	XMFLOAT3 newPosition{ };
	XMStoreFloat3(&newPosition, currentPosition + (launchDirection * m_LaunchSpeed));

	GetTransform()->Translate(newPosition);
}
