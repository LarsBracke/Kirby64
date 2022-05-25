#include "stdafx.h"
#include "KirbyPrefab.h"

#include "Character.h"
#include "Components/EnemyComponent.h"
#include "Helpers/GameManager.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"



KirbyPrefab::KirbyPrefab()
	: m_pPhysicsMaterial(PxGetPhysics().createMaterial(0.f,0.f,0.5f))
	, m_CharacterDesc(m_pPhysicsMaterial)
	, m_FallAcceleration(m_CharacterDesc.maxFallSpeed / m_CharacterDesc.fallAccelerationTime)
{
	m_pGameManager = GameManager::Get();
}

void KirbyPrefab::Initialize(const SceneContext& sceneContext)
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	/*transformations*/


	/*model*/
	m_pModelComponent = new ModelComponent(L"Meshes/Kirby.ovm");
	auto* pMainMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMainMaterial->SetDiffuseTexture(L"Textures/kirby_kirby_diffuse.png");
	auto* pSecundaryMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pSecundaryMaterial->SetDiffuseTexture(L"Textures/kirby_foot_diffuse.png");
	m_pModelComponent->SetMaterial(pMainMaterial, 1);
	m_pModelComponent->SetMaterial(pSecundaryMaterial, 0);

	auto* pModel = new GameObject();
	pModel->AddComponent(m_pModelComponent);
	AddChild(pModel);
	pModel->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	pModel->GetTransform()->Rotate(90.f, -90.f, 0.f);
	pModel->GetTransform()->Translate(0, 0, 0);

	/*collision*/
	auto* pCollision = new GameObject();
	m_pRigidBodyComponent = pCollision->AddComponent(new RigidBodyComponent(true));
	AddChild(pCollision);
	m_pRigidBodyComponent->AddCollider(PxBoxGeometry{ 2.25f,0.5f,2.25f }, *pDefaultMaterial, true);

	auto onKirbyHit = [&](GameObject*, GameObject* pOther, PxTriggerAction /*action*/)
	{
		if (pOther->GetComponent<EnemyComponent>())
		{
			Logger::LogDebug(L"Collision with enemy");
		}
	};
	pCollision->SetOnTriggerCallBack(onKirbyHit);

	/*controller*/
	m_CharacterDesc = CharacterDesc{ pDefaultMaterial };
	m_CharacterDesc.actionId_MoveLeft = MoveLeft;
	m_CharacterDesc.actionId_MoveRight = MoveRight;
	m_CharacterDesc.actionId_Jump = Jump;
	m_CharacterDesc.JumpSpeed = 25.f;
	m_pController = AddComponent(new ControllerComponent(m_CharacterDesc.controller));

	const InputAction actionMoveRight{ MoveRight, InputState::down, VK_RIGHT, -1 };
	const InputAction actionMoveLeft{ MoveLeft, InputState::down, VK_LEFT, -1 };
	const InputAction actionJump{ Jump, InputState::pressed, VK_SPACE };
	const InputAction actionStartInhale{ StartInhale, InputState::pressed, 'Q', -1 };
	const InputAction actionStopInhale{ StopInhale, InputState::released, 'Q', -1 };

	sceneContext.pInput->AddInputAction(actionMoveRight);
	sceneContext.pInput->AddInputAction(actionMoveLeft);
	sceneContext.pInput->AddInputAction(actionJump);
	sceneContext.pInput->AddInputAction(actionStartInhale);
	sceneContext.pInput->AddInputAction(actionStopInhale);

	/*camera*/

}

void KirbyPrefab::PostInitialize(const SceneContext&)
{
	/*register kirby*/
	GameManager::Get()->RegisterKirby(this);

	/*start animating*/
	m_pModelComponent->GetAnimator()->Play();
}

void KirbyPrefab::Update(const SceneContext& sceneContext)
{
	HandleMovement(sceneContext);
	HandleInhaling(sceneContext);
}

void KirbyPrefab::HandleMovement(const SceneContext& sceneContext)
{
	/*movement*/
	m_TotalVelocity.x = 0;
	if (sceneContext.pInput->IsActionTriggered(MoveRight))
	{
		const float displacement = m_CharacterDesc.maxMoveSpeed;
		m_TotalVelocity.x += displacement;
		GetTransform()->Rotate(0, 0, 0);
		;
	}
	if (sceneContext.pInput->IsActionTriggered(MoveLeft))
	{
		const float displacement = m_CharacterDesc.maxMoveSpeed;
		m_TotalVelocity.x -= displacement;
		GetTransform()->Rotate(0, 180, 0);
	}

	/*gravity*/
	PxVec3 origin = PhysxHelper::ToPxVec3(GetTransform()->GetPosition());
	const float characterHeight = m_CharacterDesc.controller.height;
	origin.y -= characterHeight / 2.f;
	origin.y -= 0.25f;
	const PxVec3 direction = PxVec3{ 0,-1,0 };

	const PhysxProxy* pPhysXProxy = GetScene()->GetPhysxProxy();
	PxRaycastBuffer hit{ };
	constexpr float castDistance = 0.5f;
	const bool isGrounded = pPhysXProxy->Raycast(origin, direction, castDistance, hit);

	if (!isGrounded)
	{
		m_TotalVelocity.y -= (sceneContext.pGameTime->GetElapsed() * m_FallAcceleration);
		if (m_TotalVelocity.y < -m_CharacterDesc.maxFallSpeed)
			m_TotalVelocity.y = -m_CharacterDesc.maxFallSpeed;
	}
	else if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_Jump))
		m_TotalVelocity.y = m_CharacterDesc.JumpSpeed;
	else
		m_TotalVelocity.y = 0;

	/*jumping*/
	if (m_JumpCount > 0 && sceneContext.pInput->IsActionTriggered(Jump))
	{
		m_TotalVelocity.y = m_CharacterDesc.JumpSpeed;
		--m_JumpCount;
	}
	if (isGrounded)
		m_JumpCount = m_MaxJumpCount;


	/*apply the displacement*/
	XMVECTOR totalVelocity = XMLoadFloat3(&m_TotalVelocity);
	totalVelocity *= sceneContext.pGameTime->GetElapsed();
	XMFLOAT3 displacement{ };
	XMStoreFloat3(&displacement, totalVelocity);
	m_pController->Move(displacement);

	/*collider movement*/
	m_pRigidBodyComponent->GetTransform()->Translate(GetTransform()->GetPosition());
}

void KirbyPrefab::HandleInhaling(const SceneContext& sceneContext)
{
	if (sceneContext.pInput->IsActionTriggered(StartInhale))
		m_IsInhaling = true;
	if (sceneContext.pInput->IsActionTriggered(StopInhale))
		m_IsInhaling = false;

	if (!m_IsInhaling)
		return;

	auto* pClosestEnemy = m_pGameManager->GetClosestEnemy(m_InhaleRange);
	if (!pClosestEnemy)
		return;

	auto* pControllerComponent = pClosestEnemy->GetComponent<ControllerComponent>();
	if (!pControllerComponent)
		return;

	XMVECTOR myPos = XMLoadFloat3(&GetTransform()->GetPosition());
	XMVECTOR enemyPos = XMLoadFloat3(&pClosestEnemy->GetTransform()->GetPosition());
	XMVECTOR toMe = XMVector3Normalize(XMVectorSubtract(myPos, enemyPos));
	XMFLOAT3 displacement{ };
	XMStoreFloat3(&displacement, toMe * m_InhaleSpeed * sceneContext.pGameTime->GetElapsed());
	
	pControllerComponent->Move(displacement);
}
