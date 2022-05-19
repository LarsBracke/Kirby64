#include "stdafx.h"
#include "Kirby.h"

#include "Character.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

Kirby::Kirby()
	: m_pPhysicsMaterial(PxGetPhysics().createMaterial(0.f,0.f,0.5f))
	, m_CharacterDesc(m_pPhysicsMaterial)
	, m_FallAcceleration(m_CharacterDesc.maxFallSpeed / m_CharacterDesc.fallAccelerationTime)
{
}

void Kirby::Initialize(const SceneContext& sceneContext)
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	/*transformations*/


	/*model*/
	m_pModelComponent = new ModelComponent(L"Meshes/Kirby.ovm");
	auto* pMainMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pMainMaterial->SetDiffuseTexture(L"Textures/kirby_kirby_diffuse.png");
	auto* pSecundaryMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pSecundaryMaterial->SetDiffuseTexture(L"Textures/kirby_foot_diffuse.png");
	m_pModelComponent->SetMaterial(pMainMaterial, 1);
	m_pModelComponent->SetMaterial(pSecundaryMaterial, 0);

	auto* pModel = new GameObject();
	AddChild(pModel);
	pModel->AddComponent(m_pModelComponent);
	pModel->GetTransform()->Scale(25.f, 25.f, 25.f);
	pModel->GetTransform()->Rotate(90.f, -90.f, 0.f);
	pModel->GetTransform()->Translate(0, -2.f, 0);


	/*controller*/
	m_CharacterDesc = CharacterDesc{ pDefaultMaterial };
	m_CharacterDesc.actionId_MoveLeft = MoveLeft;
	m_CharacterDesc.actionId_MoveRight = MoveRight;
	m_CharacterDesc.actionId_Jump = Jump;
	m_pController = AddComponent(new ControllerComponent(m_CharacterDesc.controller));

	const InputAction actionMoveRight{ MoveRight, InputState::down, VK_RIGHT, -1 };
	const InputAction actionMoveLeft{ MoveLeft, InputState::down, VK_LEFT, -1 };
	const InputAction actionJump{ Jump, InputState::pressed, VK_SPACE };

	sceneContext.pInput->AddInputAction(actionMoveRight);
	sceneContext.pInput->AddInputAction(actionMoveLeft);
	sceneContext.pInput->AddInputAction(actionJump);

	/*camera*/

}

void Kirby::Update(const SceneContext& sceneContext)
{
	/*movement*/
	m_TotalVelocity.x = 0;
	if (sceneContext.pInput->IsActionTriggered(MoveRight))
	{
		const float displacement = m_CharacterDesc.maxMoveSpeed;
		m_TotalVelocity.x += displacement;
		GetTransform()->Rotate(0, 0, 0);
;	}
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
	const float castDistance = 0.5f;
	bool isGrounded = pPhysXProxy->Raycast(origin, direction, castDistance, hit);

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


	/*apply the displacement*/
	XMVECTOR totalVelocity = XMLoadFloat3(&m_TotalVelocity);
	totalVelocity *= sceneContext.pGameTime->GetElapsed();
	XMFLOAT3 displacement{ };
	XMStoreFloat3(&displacement, totalVelocity);
	m_pController->Move(displacement);
}
