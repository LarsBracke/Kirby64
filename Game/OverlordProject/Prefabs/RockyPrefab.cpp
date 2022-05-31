#include "stdafx.h"
#include "RockyPrefab.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

RockyPrefab::RockyPrefab()
	: m_pGameManager(GameManager::Get())
	, m_pPhysicsMaterial(PxGetPhysics().createMaterial(0.5f,0.5f,0.5f))
	, m_CharacterDesc(m_pPhysicsMaterial)
{ }

RockyPrefab::~RockyPrefab()
{
	auto* pActor = GetComponent<ControllerComponent>()->GetPxController()->getActor();
	SceneManager::Get()->GetActiveScene()->GetPhysxProxy()->GetPhysxScene()->removeActor(*pActor);
	m_pGameManager->UnRegisterEnemy(this);
}

void RockyPrefab::Initialize(const SceneContext&)
{
	/*model*/
	auto* pModel = new GameObject();
	AddChild(pModel);
	pModel->GetTransform()->Translate(0, -2, 0);
	pModel->GetTransform()->Rotate(90, 90, 0);
	pModel->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	m_pModelComponent = pModel->AddComponent(new ModelComponent(L"Meshes/Rocky.ovm"));

	auto* pBodyMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pBodyMaterial->SetDiffuseTexture(L"Textures/Rocky_Body.png");
	auto* pFeetMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pFeetMaterial->SetDiffuseTexture(L"Textures/Rocky_Feet.png");
	auto* pEyesMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pEyesMaterial->SetDiffuseTexture(L"Textures/Rocky_Eyes.png");

	m_pModelComponent->SetMaterial(pBodyMaterial, 0);
	m_pModelComponent->SetMaterial(pEyesMaterial, 1);
	m_pModelComponent->SetMaterial(pFeetMaterial, 2);
	m_pModelComponent->SetMaterial(pFeetMaterial, 3);

	/*enemy and type*/
	m_pEnemyComponent = AddComponent(new EnemyComponent());
	m_pEnemyComponent->SetAbilityType(AbilityType::Stone);

	/*controller*/
	m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDesc.controller));
}

void RockyPrefab::PostInitialize(const SceneContext&)
{
	/*register*/
	m_pGameManager->RegisterEnemy(this);

	/*animation*/
	m_pModelComponent->GetAnimator()->SetAnimation(0);
	m_pModelComponent->GetAnimator()->Play();
}

void RockyPrefab::Update(const SceneContext& sceneContext)
{
	if (m_pGameManager->IsKirbyInRange(GetTransform()->GetPosition(), m_AttackRange))
		MoveToKirby(sceneContext);
}

void RockyPrefab::MoveToKirby(const SceneContext& sceneContext)
{
	const XMVECTOR kirbyPos = XMLoadFloat3(&m_pGameManager->GetKirbyPosition());
	const XMVECTOR myPos = XMLoadFloat3(&GetTransform()->GetPosition());
	XMFLOAT3 toKirby{ };
	XMStoreFloat3(&toKirby, XMVector3Normalize(XMVectorSubtract(kirbyPos, myPos)));
	XMFLOAT3 displacement{ };
	displacement.x = toKirby.x * m_Speed * sceneContext.pGameTime->GetElapsed();
	m_pControllerComponent->Move(displacement);

	if (displacement.x > 0)
		GetTransform()->Rotate(0, 180, 0);
	else
		GetTransform()->Rotate(0, 0, 0);

	if (abs(displacement.x) > 0.01f)
		SetAnimationState(AnimationState::Running);
	else
		SetAnimationState(AnimationState::Idle);
}

void RockyPrefab::SetAnimationState(AnimationState newState)
{
	if (m_CurrentAnimationState == newState)
		return;

	m_CurrentAnimationState = newState;
	m_pModelComponent->GetAnimator()->SetAnimation(static_cast<UINT>(newState));
	m_pModelComponent->GetAnimator()->Reset();
	m_pModelComponent->GetAnimator()->Play();
}

