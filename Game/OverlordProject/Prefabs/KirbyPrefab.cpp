#include "stdafx.h"
#include "KirbyPrefab.h"

#include "Character.h"
#include "Components/EnemyComponent.h"

#include "Helpers/GameManager.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

#include "Prefabs/StarPrefab.h"

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
	m_pModelComponent->SetMaterial(pSecundaryMaterial, 4);

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
		if (pOther == nullptr || pOther == m_pPushingObject)
			return;

		if (auto* pEnemyComponent = pOther->GetComponent<EnemyComponent>())
		{
			if (m_IsInhaling)
			{
				m_pInhaledEnemy = pOther;
				pEnemyComponent->Kill();
			}
			else
			{
				m_PushBack = true;
				m_pPushingObject = pOther;
				m_pHealthComponent->DoDamage(1);
				if (m_pHealthComponent->GetHealth() == 0)
					SceneManager::Get()->NextScene();
			}
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

	const InputAction actionMoveRight{ MoveRight, InputState::down, VK_RIGHT, -1, 8 };
	const InputAction actionMoveLeft{ MoveLeft, InputState::down, VK_LEFT, -1, 4 };
	const InputAction actionJump{ Jump, InputState::pressed, VK_SPACE, -1, 4096 };
	const InputAction actionStartInhale{ StartInhale, InputState::pressed, 'Q', -1, 16384 };
	const InputAction actionStopInhale{ StopInhale, InputState::released, 'Q', -1, 16384  };
	const InputAction actionExhale{ Exhale, InputState::pressed, 'Z', -1, 8192 };

	sceneContext.pInput->AddInputAction(actionMoveRight);
	sceneContext.pInput->AddInputAction(actionMoveLeft);
	sceneContext.pInput->AddInputAction(actionJump);
	sceneContext.pInput->AddInputAction(actionStartInhale);
	sceneContext.pInput->AddInputAction(actionStopInhale);
	sceneContext.pInput->AddInputAction(actionExhale);

	/*health*/
	m_pHealthComponent = AddComponent(new HealthComponent(25));

	/*particles*/
	ParticleEmitterSettings settings{ };
	settings.velocity = { -m_ParticleSpeed,0.f,0.f };
	settings.minSize = 0.75f;
	settings.maxSize = 1.0f;
	settings.minEnergy = 0.2f;
	settings.maxEnergy = 0.25f;
	settings.minScale = 3.5f;
	settings.maxScale = 4.0f;
	settings.minEmitterRadius = 2.5f;
	settings.maxEmitterRadius = 3.5f;
	settings.color = { 1.f,1.f,1.f, .75f };

	m_pParticleObject = new GameObject();
	m_pParticleObject->GetTransform()->Translate(10.f, 0, 0);
	m_pInhaleParticleSystem = m_pParticleObject->AddComponent(new ParticleEmitterComponent(L"Textures/Inhale.png", settings));

	/*audio*/
	auto* pSoundSystem = SoundManager::Get()->GetSystem();
	FMOD_RESULT result{ };

	result = pSoundSystem->createStream("Resources/Audio/Kirby_Jump.mp3", FMOD_DEFAULT, nullptr, &m_pSoundJump);
	HANDLE_ERROR(result);
	result = pSoundSystem->createStream("Resources/Audio/Kirby_Hurt.mp3", FMOD_DEFAULT, nullptr, &m_pSoundHurt);
	HANDLE_ERROR(result);
	result = pSoundSystem->createStream("Resources/Audio/Kirby_Inhale.mp3", FMOD_DEFAULT, nullptr, &m_pSoundInhale);
	HANDLE_ERROR(result);
	result = pSoundSystem->createStream("Resources/Audio/Punch.mp3", FMOD_DEFAULT, nullptr, &m_pSoundPunch);
	HANDLE_ERROR(result);
	result = pSoundSystem->createStream("Resources/Audio/Star.mp3", FMOD_DEFAULT, nullptr, &m_pSoundStar);
	HANDLE_ERROR(result);

	m_pAudioChannel->setVolume(0.5f);
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
	HandleExhaling(sceneContext);
	HandleAnimations(sceneContext);
	HandleAudio(sceneContext);
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
		m_pInhaleParticleSystem->GetSettings().velocity.x = -m_ParticleSpeed;
	}
	if (sceneContext.pInput->IsActionTriggered(MoveLeft))
	{
		const float displacement = m_CharacterDesc.maxMoveSpeed;
		m_TotalVelocity.x -= displacement;
		GetTransform()->Rotate(0, 180, 0);
		m_pInhaleParticleSystem->GetSettings().velocity.x = m_ParticleSpeed;
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

	/*animations*/
	if (!m_IsInhaling && isGrounded)
	{
		if (abs(displacement.x) > 0.01f)
			SetAnimationState(AnimationState::Running);
		else
			SetAnimationState(AnimationState::Idle);
	}

	/*collider movement*/
	m_pRigidBodyComponent->GetTransform()->Translate(GetTransform()->GetPosition());

	/*pushing back*/
	if (m_PushBack && m_pPushingObject)
	{
		PushBack(sceneContext, m_pPushingObject);
		m_PushBack = false;
		m_pPushingObject = nullptr;
	}
}

void KirbyPrefab::HandleInhaling(const SceneContext& sceneContext)
{
	if (sceneContext.pInput->IsActionTriggered(StartInhale))
	{
		m_IsInhaling = true;
		AddChild(m_pParticleObject);
	}
	if (sceneContext.pInput->IsActionTriggered(StopInhale))
	{
		m_IsInhaling = false;
		m_IsInhaling = false;
		RemoveChild(m_pParticleObject);
	}

	if (!m_IsInhaling)
		return;

	auto* pClosestEnemy = m_pGameManager->GetClosestEnemy(m_InhaleRange);
	if (!pClosestEnemy)
		return;

	auto* pControllerComponent = pClosestEnemy->GetComponent<ControllerComponent>();
	if (!pControllerComponent)
		return;

	const XMVECTOR myPos = XMLoadFloat3(&GetTransform()->GetPosition());
	const XMVECTOR enemyPos = XMLoadFloat3(&pClosestEnemy->GetTransform()->GetPosition());
	const XMVECTOR toMe = XMVector3Normalize(XMVectorSubtract(myPos, enemyPos));
	XMFLOAT3 displacement{ };
	XMStoreFloat3(&displacement, toMe * m_InhaleSpeed * sceneContext.pGameTime->GetElapsed());
	
	pControllerComponent->Move(displacement);
}

void KirbyPrefab::HandleExhaling(const SceneContext& sceneContext)
{
	if (sceneContext.pInput->IsActionTriggered(Exhale) && m_pInhaledEnemy != nullptr)
	{
		if (GetTransform()->GetForward().x < 0)
			m_pGameManager->ShootStar(GetTransform()->GetPosition(), { -1,0,0 });
		else
			m_pGameManager->ShootStar(GetTransform()->GetPosition(), { 1,0,0 });

		m_pInhaledEnemy = nullptr;
	}
}

void KirbyPrefab::HandleAudio(const SceneContext& sceneContext)
{
	auto* pSoundSystem = SoundManager::Get()->GetSystem();
	if (sceneContext.pInput->IsActionTriggered(Jump) && m_JumpCount > 0)
	{
		pSoundSystem->playSound(m_pSoundJump, nullptr, false, &m_pAudioChannel);
	}
	if (sceneContext.pInput->IsActionTriggered(StartInhale))
	{
		pSoundSystem->playSound(m_pSoundInhale, nullptr, false, &m_pAudioChannel);
	}
	if (sceneContext.pInput->IsActionTriggered(StopInhale))
	{
		pSoundSystem->playSound(m_pSoundInhale, nullptr, true, &m_pAudioChannel);
	}
	if (sceneContext.pInput->IsActionTriggered(Exhale))
	{
		pSoundSystem->playSound(m_pSoundStar, nullptr, false, &m_pAudioChannel);
	}
}

void KirbyPrefab::HandleAnimations(const SceneContext& sceneContext)
{
	if (sceneContext.pInput->IsActionTriggered(Jump) && m_JumpCount > 0)
		SetAnimationState(AnimationState::Jump, true);

	if (sceneContext.pInput->IsActionTriggered(StartInhale))
		SetAnimationState(AnimationState::Inhaling);
	else if (sceneContext.pInput->IsActionTriggered(StopInhale))
		SetAnimationState(AnimationState::Idle);
}

void KirbyPrefab::PushBack(const SceneContext& sceneContext, const GameObject* pOther)
{
	m_pHealthComponent->DoDamage(1);
	const float myPosX = GetTransform()->GetPosition().x;
	const float enemyPosX = pOther->GetTransform()->GetPosition().x;

	if (myPosX < enemyPosX)
		m_pController->Move(XMFLOAT3{ -m_PushBackSpeed * sceneContext.pGameTime->GetElapsed(), 0 ,0 });
	else
		m_pController->Move(XMFLOAT3{ m_PushBackSpeed * sceneContext.pGameTime->GetElapsed(), 0 ,0 });

	SoundManager::Get()->GetSystem()->playSound(m_pSoundHurt, nullptr, false, &m_pAudioChannel);
	SoundManager::Get()->GetSystem()->playSound(m_pSoundPunch, nullptr, false, &m_pSecondaryAudioChannel);

	m_pPushingObject = nullptr;
}

void KirbyPrefab::SetAnimationState(AnimationState newState, bool forceAnimationChange)
{
	if (m_CurrentAnimationState == newState && !forceAnimationChange)
		return;

	m_CurrentAnimationState = newState;
	m_pModelComponent->GetAnimator()->SetAnimation(static_cast<UINT>(newState));
	m_pModelComponent->GetAnimator()->Reset();
	m_pModelComponent->GetAnimator()->Play();
}
