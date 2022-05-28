#include "stdafx.h"
#include "StarPrefab.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Components/EnemyComponent.h"

StarPrefab::StarPrefab(const XMFLOAT3& launchDirection)
	: m_LaunchDirection(launchDirection)
{ }

void StarPrefab::Initialize(const SceneContext&)
{
	/*model*/
	m_pModelComponent = AddComponent(new ModelComponent(L"Meshes/Star.ovm"));
	auto* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Star.png");
	m_pModelComponent->SetMaterial(pMaterial);
	GetTransform()->Scale(0.1f, 0.1f, 0.1f);

	/*collision*/
	const PxMaterial* pPhysicsMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	auto* pCollision = new GameObject();
	AddChild(pCollision);
	m_pRigidBodyComponent = pCollision->AddComponent(new RigidBodyComponent(true));
	m_pRigidBodyComponent->AddCollider(PxBoxGeometry{ 1.f,1.f,1.f }, *pPhysicsMaterial, true);

	auto onStarHit = [&](GameObject*, GameObject* pOther, PxTriggerAction)
	{
		if (pOther == nullptr)
			return;

		if (auto* pEnemyComponent = pOther->GetComponent<EnemyComponent>())
		{
			m_ObjectHit = true;
			pEnemyComponent->Kill();
		}
	};
	pCollision->SetOnTriggerCallBack(onStarHit);
}

void StarPrefab::PostInitialize(const SceneContext&)
{
	m_pModelComponent->GetAnimator()->SetAnimation(0);
}

void StarPrefab::Update(const SceneContext&)
{
	m_pModelComponent->GetAnimator()->Play();

	const XMVECTOR currentPosition = XMLoadFloat3(&GetTransform()->GetPosition());
	const XMVECTOR launchDirection = XMLoadFloat3(&m_LaunchDirection);
	XMFLOAT3 newPosition{ };
	XMStoreFloat3(&newPosition, currentPosition + (launchDirection * m_LaunchSpeed));

	GetTransform()->Translate(newPosition);
	m_pRigidBodyComponent->GetTransform()->Translate(newPosition);

	if (m_ObjectHit)
		MarkForDeletion();
}
