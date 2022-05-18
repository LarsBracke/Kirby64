#include "stdafx.h"
#include "PongScene.h"

#include "Prefabs/SpherePrefab.h"
#include "Prefabs/CubePrefab.h"

#include <sstream>

void PongScene::Initialize()
{
	srand(unsigned int(time(0)));

	// Scene settings
	m_SceneContext.settings.clearColor = static_cast<XMFLOAT4>(Colors::Black);
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawGrid = false;

	// Materials
	PxPhysics& physX = PxGetPhysics();
	PxMaterial* pGroundMaterial = physX.createMaterial(0.0f, 0.0f, 0.0f);
	PxMaterial* pWallMaterial = physX.createMaterial(0.0f, 0.0f, 1.f);
	PxMaterial* pBallMaterial = physX.createMaterial(m_BallFriction, m_BallFriction, m_BallRestitution);
	PxMaterial* pPaddleMaterial = physX.createMaterial(m_PaddleFriction, m_PaddleFriction, m_PaddleRestitution);

	// Input
	m_pInput = m_SceneContext.pInput;

	const InputAction moveLeftUp{ InputIDs::LeftUp, InputState::down, 'Q', -1 };
	const InputAction moveLeftDown{ InputIDs::LeftDown, InputState::down, 'A', -1 };
	const InputAction moveRightUp{ InputIDs::RightUp, InputState::down, VK_UP, -1 };
	const InputAction moveRightDown{ InputIDs::RightDown, InputState::down, VK_DOWN, -1 };
	const InputAction start{ InputIDs::Start, InputState::pressed, VK_SPACE, -1 };
	const InputAction reset{ InputIDs::Reset, InputState::pressed, 'R', -1 };
	m_pInput->AddInputAction(moveLeftUp);
	m_pInput->AddInputAction(moveLeftDown);
	m_pInput->AddInputAction(moveRightUp);
	m_pInput->AddInputAction(moveRightDown);
	m_pInput->AddInputAction(start);
	m_pInput->AddInputAction(reset);

	// Ground plane
	GameSceneExt::CreatePhysXGroundPlane(*this, pGroundMaterial);

	// Camera
	m_pCamera = new FixedCamera();
	AddChild(m_pCamera);
	SetActiveCamera(m_pCamera->GetComponent<CameraComponent>());

	GetActiveCamera()->GetTransform()->Translate(0.f, m_CameraHeight, 0.f);
	GetActiveCamera()->GetTransform()->Rotate(90.f, 0.f, 0.f, true);

	// Ball
	m_pBall = new SpherePrefab(m_BallRadius, 10, static_cast<XMFLOAT4>(Colors::Red));
	RigidBodyComponent* pRigidBody = m_pBall->AddComponent(new RigidBodyComponent());
	pRigidBody->AddCollider(PxSphereGeometry{ m_BallRadius }, *pBallMaterial);
	m_pBall->GetTransform()->Translate(0, 2.f, 0.f);
	AddChild(m_pBall);

	// Level
	for (unsigned int count = 0; count < 2; ++count) /*top and bottom wall*/
	{
		GameObject* pWall = new GameObject();
		pRigidBody = pWall->AddComponent(new RigidBodyComponent(true));
		AddChild(pWall); /*init rigidbody befor adding collider*/
		pRigidBody->AddCollider(PxBoxGeometry{ 1.f,5,m_PlayAreaWidth + m_PlayAreaHeight }, *pWallMaterial);

		m_BounceWalls.push_back(pWall);
	}

	m_BounceWalls[0]->GetTransform()->Translate(0.f, 0.f, -m_PlayAreaHeight / 2.f);
	m_BounceWalls[1]->GetTransform()->Translate(0.f, 0.f, m_PlayAreaHeight / 2.f);
	m_BounceWalls[0]->GetTransform()->Rotate(0.f, 90.f, 0.f, true);
	m_BounceWalls[1]->GetTransform()->Rotate(0.f, 90.f, 0.f, true);

	for (unsigned int count = 0; count < 2; ++count) /*side walls*/
	{
		GameObject* pWall = new GameObject();
		pRigidBody = pWall->AddComponent(new RigidBodyComponent(true));
		AddChild(pWall); /*init rigidbody befor adding collider*/

		const auto colliderId = pRigidBody->AddCollider(PxBoxGeometry{ 1.f,5,m_PlayAreaWidth + m_PlayAreaHeight }, *pWallMaterial);
		ColliderInfo colliderInfo = pRigidBody->GetCollider(colliderId);
		colliderInfo.SetTrigger(true);

		m_TriggerWalls.push_back(pWall);
	}

	m_TriggerWalls[0]->GetTransform()->Translate(-m_PlayAreaWidth / 2.f, 0.f, 0.f);
	m_TriggerWalls[1]->GetTransform()->Translate(m_PlayAreaWidth / 2.f, 0.f, 0.f);

	std::function<void(GameObject*, GameObject*, PxTriggerAction)> onWallHit =
		[this](GameObject*, GameObject* pOther, PxTriggerAction action)
	{
		if (action != PxTriggerAction::ENTER)
			return;

		if (pOther == m_pBall)
			ResetGame();
	};

	m_TriggerWalls[0]->SetOnTriggerCallBack(onWallHit);
	m_TriggerWalls[1]->SetOnTriggerCallBack(onWallHit);

	// Paddles
	m_pLeftPaddle = new CubePrefab(m_PaddleSize, static_cast<XMFLOAT4>(Colors::Gray));
	pRigidBody = m_pLeftPaddle->AddComponent(new RigidBodyComponent());
	pRigidBody->SetKinematic(true);
	pRigidBody->AddCollider(PxBoxGeometry{ m_PaddleSize.x / 2.f, m_PaddleSize.y / 2.f, m_PaddleSize.z / 2.f }, *pPaddleMaterial);
	m_pLeftPaddle->GetTransform()->Translate(-m_PlayAreaWidth / 2.f + m_PaddleOffset, 0.f, 0.f);

	m_pRightPaddle = new CubePrefab(m_PaddleSize, static_cast<XMFLOAT4>(Colors::Gray));
	pRigidBody = m_pRightPaddle->AddComponent(new RigidBodyComponent());
	pRigidBody->SetKinematic(true);
	pRigidBody->AddCollider(PxBoxGeometry{ m_PaddleSize.x / 2.f, m_PaddleSize.y / 2.f, m_PaddleSize.z / 2.f }, *pPaddleMaterial);
	m_pRightPaddle->GetTransform()->Translate(m_PlayAreaWidth / 2.f - m_PaddleOffset, 0.f, 0.f);

	AddChild(m_pLeftPaddle);
	AddChild(m_pRightPaddle);
}

void PongScene::PostInitialize()
{ }

void PongScene::Update()
{
	HandlePaddleMovement();
	HandleBallLaunch();
	HandleGameReset();
}

void PongScene::OnGUI()
{ }

void PongScene::HandlePaddleMovement() const
{
	const float deltaTime = m_SceneContext.pGameTime->GetElapsed();

	if (m_pInput->IsActionTriggered(InputIDs::LeftUp))
	{
		RigidBodyComponent* pRigidBody = m_pLeftPaddle->GetComponent<RigidBodyComponent>();
		PxRigidDynamic* pActor = static_cast<PxRigidDynamic*>(pRigidBody->GetPxRigidActor());

		XMFLOAT3 currentPos = m_pLeftPaddle->GetTransform()->GetPosition();
		PxVec3 newPos{ currentPos.x, currentPos.y, currentPos.z + m_PaddleSpeed * deltaTime };
		pActor->setKinematicTarget(PxTransform{ PxVec3{newPos.x, newPos.y, newPos.z} });
	}
	else if (m_pInput->IsActionTriggered(InputIDs::LeftDown))
	{
		RigidBodyComponent* pRigidBody = m_pLeftPaddle->GetComponent<RigidBodyComponent>();
		PxRigidDynamic* pActor = static_cast<PxRigidDynamic*>(pRigidBody->GetPxRigidActor());

		XMFLOAT3 currentPos = m_pLeftPaddle->GetTransform()->GetPosition();
		PxVec3 newPos{ currentPos.x, currentPos.y, currentPos.z - m_PaddleSpeed * deltaTime };
		pActor->setKinematicTarget(PxTransform{ PxVec3{newPos.x, newPos.y, newPos.z} });
	}

	if (m_pInput->IsActionTriggered(InputIDs::RightUp))
	{
		RigidBodyComponent* pRigidBody = m_pRightPaddle->GetComponent<RigidBodyComponent>();
		PxRigidDynamic* pActor = static_cast<PxRigidDynamic*>(pRigidBody->GetPxRigidActor());

		XMFLOAT3 currentPos = m_pRightPaddle->GetTransform()->GetPosition();
		PxVec3 newPos{ currentPos.x, currentPos.y, currentPos.z + m_PaddleSpeed * deltaTime };
		pActor->setKinematicTarget(PxTransform{ PxVec3{newPos.x, newPos.y, newPos.z} });

	}
	else if (m_pInput->IsActionTriggered(InputIDs::RightDown))
	{
		RigidBodyComponent* pRigidBody = m_pRightPaddle->GetComponent<RigidBodyComponent>();
		PxRigidDynamic* pActor = static_cast<PxRigidDynamic*>(pRigidBody->GetPxRigidActor());

		XMFLOAT3 currentPos = m_pRightPaddle->GetTransform()->GetPosition();
		PxVec3 newPos{ currentPos.x, currentPos.y, currentPos.z - m_PaddleSpeed * deltaTime };
		pActor->setKinematicTarget(PxTransform{ PxVec3{newPos.x, newPos.y, newPos.z} });
	}
}

void PongScene::HandleBallLaunch()
{
	if (m_GameStarted)
		return;

	if (m_pInput->IsActionTriggered(InputIDs::Start))
	{
		bool launchLeft = static_cast<bool>(rand() % 2);
		float verticalForceReduction = ((rand() % 200) - 100) / 100.f;

		if (launchLeft)
			m_pBall->GetComponent<RigidBodyComponent>()->AddForce(XMFLOAT3{ -m_BallLaunchForce, 0.f, verticalForceReduction * m_BallLaunchForce }, PxForceMode::eIMPULSE);
		else
			m_pBall->GetComponent<RigidBodyComponent>()->AddForce(XMFLOAT3{ m_BallLaunchForce, 0.f, verticalForceReduction * m_BallLaunchForce }, PxForceMode::eIMPULSE);

		m_GameStarted = true;
	}
}

void PongScene::HandleGameReset()
{
	if (m_pInput->IsActionTriggered(InputIDs::Reset))
		ResetGame();
}

void PongScene::ResetGame()
{
	m_pBall->GetTransform()->Translate(0, m_BallRadius * 2, 0);
	m_pLeftPaddle->GetTransform()->Translate(-m_PlayAreaWidth / 2.f + m_PaddleOffset, 0.f, 0.f);
	m_pRightPaddle->GetTransform()->Translate(m_PlayAreaWidth / 2.f - m_PaddleOffset, 0.f, 0.f);
	m_GameStarted = false;
}
