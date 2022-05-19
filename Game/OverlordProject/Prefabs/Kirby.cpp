#include "stdafx.h"
#include "Kirby.h"

#include "Character.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

Kirby::Kirby()
	: m_pPhysicsMaterial(PxGetPhysics().createMaterial(0.5f,0.5f,0.5f))
	, m_CharacterDesc(m_pPhysicsMaterial)
{
}

void Kirby::Initialize(const SceneContext& sceneContext)
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	/*transformations*/
	GetTransform()->Scale(25.f, 25.f, 25.f);
	GetTransform()->Rotate(90.f, -90.f, 0.f);

	/*model*/
	m_pModelComponent = AddComponent(new ModelComponent(L"Meshes/Kirby.ovm"));
	auto* pMainMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pMainMaterial->SetDiffuseTexture(L"Textures/kirby_kirby_diffuse.png");
	auto* pSecundaryMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pSecundaryMaterial->SetDiffuseTexture(L"Textures/kirby_foot_diffuse.png");
	m_pModelComponent->SetMaterial(pMainMaterial, 1);
	m_pModelComponent->SetMaterial(pSecundaryMaterial, 0);

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
	if (sceneContext.pInput->IsActionTriggered(MoveRight))
	{
		const float displacement = m_CharacterDesc.maxMoveSpeed * sceneContext.pGameTime->GetElapsed();
		m_pController->Move(XMFLOAT3{ displacement, 0.f, 0.f });
	}
	if (sceneContext.pInput->IsActionTriggered(MoveLeft))
	{
		const float displacement = m_CharacterDesc.maxMoveSpeed * sceneContext.pGameTime->GetElapsed();
		m_pController->Move(XMFLOAT3{ -displacement, 0.f, 0.f });
	}
}
