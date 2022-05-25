#include "stdafx.h"
#include "BoboPrefab.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

BoboPrefab::BoboPrefab()
	: m_pPhysicsMaterial(PxGetPhysics().createMaterial(0.5f,0.5f,0.5f))
	, m_CharacterDesc(m_pPhysicsMaterial)
{
	m_pGameManager = GameManager::Get();
}

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
	m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDesc.controller));
}

void BoboPrefab::PostInitialize(const SceneContext&)
{
	/*register enemy*/
	GameManager::Get()->RegisterEnemy(this);
}

void BoboPrefab::Update(const SceneContext& sceneContext)
{
	if (m_pGameManager->IsKirbyInRange(GetTransform()->GetPosition(), m_AttackRange))
		MoveToKirby(sceneContext);
}

void BoboPrefab::MoveToKirby(const SceneContext& sceneContext)
{
	XMVECTOR kirbyPos = XMLoadFloat3(&m_pGameManager->GetKirbyPosition());
	XMVECTOR myPos = XMLoadFloat3(&GetTransform()->GetPosition());
	XMFLOAT3 toKirby{ };
	XMStoreFloat3(&toKirby, XMVector3Normalize(XMVectorSubtract(kirbyPos, myPos)));
	XMFLOAT3 displacement{ };
	displacement.x = toKirby.x * m_Speed * sceneContext.pGameTime->GetElapsed();
	m_pControllerComponent->Move(displacement);
}
