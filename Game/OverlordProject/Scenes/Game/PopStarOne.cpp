#include "stdafx.h"
#include "PopStarOne.h"

#include "Helpers/GameManager.h"

#include "Materials/DiffuseMaterial.h"
#include "Materials/Post/PostAA.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

#include "Materials/Post/PostBlur.h"
#include "Materials/Post/PostOutline.h"

#include "Prefabs/CubePrefab.h"
#include "Prefabs/KirbyPrefab.h"
#include "Prefabs/BoboPrefab.h"
#include "Prefabs/RockyPrefab.h"
#include "Prefabs/InGameHudPrefab.h"

PopStarOne::~PopStarOne()
{
	m_pMusicChannel->stop();
}

void PopStarOne::Initialize()
{
	/*settings*/
	m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;

	/*light*/
	m_SceneContext.pLights->SetDirectionalLight({ -350, 285, -100 }, { 0.740129888f, -0.597205281f, 0.309117377f });

	/*ground plane*/
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	/*stage*/
	auto* pLevel = new GameObject();
	AddChild(pLevel);

	auto* pGroundPlane = new GameObject();
	pGroundPlane->GetTransform()->Rotate(0, 90, 0);
	pGroundPlane->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	pGroundPlane->GetTransform()->Translate(0, 3, 75);
	auto* pModelComponent = pGroundPlane->AddComponent(new ModelComponent(L"Meshes/GroundPlane.ovm"));
	auto* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pMaterial->SetDiffuseTexture(L"Textures/GroundPlane.png");
	pModelComponent->SetMaterial(pMaterial);
	pLevel->AddChild(pGroundPlane);

	auto* pPath = new GameObject();
	pPath->GetTransform()->Translate(0, 0.1f, 0);
	pPath->GetTransform()->Rotate(0, 90, 0);
	pPath->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	pModelComponent = pPath->AddComponent(new ModelComponent(L"Meshes/Path.ovm"));
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pMaterial->SetDiffuseTexture(L"Textures/Path.png");
	pModelComponent->SetMaterial(pMaterial);
	pLevel->AddChild(pPath);

	auto* pSky = new GameObject();
	pSky->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	pSky->GetTransform()->Rotate(0, 0, 90);
	pSky->GetTransform()->Translate(0, 33.33f, 175);
	pModelComponent = pSky->AddComponent(new ModelComponent(L"Meshes/Sky.ovm"));
	auto* pSkyMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pSkyMaterial->SetDiffuseTexture(L"Textures/Sky.png");
	pModelComponent->SetMaterial(pSkyMaterial);
	pLevel->AddChild(pSky);

	auto* pEndHouse = new GameObject();
	pEndHouse->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	pEndHouse->GetTransform()->Rotate(0,90,0);
	pEndHouse->GetTransform()->Translate(180, 10, 7.5);
	pModelComponent = pEndHouse->AddComponent(new ModelComponent(L"Meshes/EndHouse.ovm"));
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pMaterial->SetDiffuseTexture(L"Textures/EndHouse.png");
	pModelComponent->SetMaterial(pMaterial,0);
	pModelComponent->SetMaterial(pMaterial,1);
	pLevel->AddChild(pEndHouse);

	auto* pRiver = new GameObject();
	pRiver->GetTransform()->Scale(0.2f, 0.2f, 0.2f);
	pRiver->GetTransform()->Rotate(0, 90, 0);
	pRiver->GetTransform()->Translate(-135, 0, 150);
	AddChild(pRiver);
	pModelComponent = pRiver->AddComponent(new ModelComponent(L"Meshes/River.ovm"));
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pMaterial->SetDiffuseTexture(L"Textures/River.png");
	pModelComponent->SetMaterial(pMaterial);

	auto* pRiver2 = new GameObject();
	pRiver2->GetTransform()->Scale(0.2f, 0.2f, 0.2f);
	pRiver2->GetTransform()->Rotate(0, 90, 0);
	pRiver2->GetTransform()->Translate(80, 0, 150);
	AddChild(pRiver2);
	pModelComponent = pRiver2->AddComponent(new ModelComponent(L"Meshes/River.ovm"));
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pMaterial->SetDiffuseTexture(L"Textures/River.png");
	pModelComponent->SetMaterial(pMaterial);


	SpawnBackGroundBoxes();

	AddBridge(XMFLOAT3{ 66,1,0 });
	AddBridge(XMFLOAT3{ 265,1,0 });
	AddBridge(XMFLOAT3{ 278,1,0 });

	//AddPathBox(XMFLOAT3{ 0,0,0 });

	/*kirby*/
	KirbyPrefab* pKirby{ new KirbyPrefab() };
	pKirby->GetTransform()->Translate(-275, 10, 0);
	AddChild(pKirby);

	/*bobo*/
	SpawnBobo(XMFLOAT3{ -125, 2, 0 });
	SpawnBobo(XMFLOAT3{ 50, 2, 0 });
	SpawnBobo(XMFLOAT3{ 100, 2, 0 });
	SpawnBobo(XMFLOAT3{ 125, 2, 0 });
	SpawnBobo(XMFLOAT3{ 175, 2, 0 });

	/*rocky*/
	SpawnRocky(XMFLOAT3{ -175,2,0 });
	SpawnRocky(XMFLOAT3{ -50,2,0 });
	SpawnRocky(XMFLOAT3{ 75,2,0 });
	SpawnRocky(XMFLOAT3{ 150,2,0 });
	SpawnRocky(XMFLOAT3{ 200,2,0 });
	SpawnRocky(XMFLOAT3{ 250,2,0 });

	/*HUD*/
	auto* pHUD = new InGameHudPrefab();
	AddChild(pHUD);

	/*camera*/
	m_pCamera = new FixedCamera();
	AddChild(m_pCamera);

	/*level end*/
	auto* pLevelEnd = new GameObject();
	AddChild(pLevelEnd);
	pLevelEnd->GetTransform()->Translate(295, 0, 0);
	auto* pRigidBodyComponent = pLevelEnd->AddComponent(new RigidBodyComponent(true));
	pRigidBodyComponent->AddCollider(PxBoxGeometry{ 5,5,5 }, * pDefaultMaterial, true);

	auto onLevelEndHit = [&](GameObject*, GameObject* pOther, PxTriggerAction /*action*/)
	{
		if (pOther == GameManager::Get()->GetKirby())
		{
			SceneManager::Get()->PreviousScene();
		}
	};
	pLevelEnd->SetOnTriggerCallBack(onLevelEndHit);
	
}

void PopStarOne::PostInitialize()
{
	m_pCameraComponent = m_pCamera->GetComponent<CameraComponent>();
	m_pCameraComponent->SetActive(true);

	/*post processing*/
	AddPostProcessingEffect(MaterialManager::Get()->CreateMaterial<PostOutline>());
	AddPostProcessingEffect(MaterialManager::Get()->CreateMaterial<PostAA>());
}

void PopStarOne::Update()
{
	HandleCameraMovement();
}

void PopStarOne::OnGUI()
{
	//static XMFLOAT3 pos{ -69.f, 35.f, 0.f };
	//static XMFLOAT3 rot{  0.433f, -0.597f, 1.192f  };
	//ImGui::SliderFloat3("Light Position", ConvertUtil::ToImFloatPtr(pos), -500, 500);
	//ImGui::SliderFloat3("Light Rotation", ConvertUtil::ToImFloatPtr(rot), 0, 2*XM_PI);
	//m_SceneContext.pLights->SetDirectionalLight(pos, rot);
}

void PopStarOne::OnSceneActivated()
{
	/*audio*/
	auto* pFmodSystem = SoundManager::Get()->GetSystem();
	auto result = pFmodSystem->createStream("Resources/Audio/Music.mp3", FMOD_DEFAULT, nullptr, &m_pMusic);
	HANDLE_ERROR(result);
	m_pMusic->setMode(FMOD_LOOP_NORMAL);
	result = pFmodSystem->playSound(m_pMusic, nullptr, false, &m_pMusicChannel);
	HANDLE_ERROR(result);
	m_pMusicChannel->setVolume(0.1f);
}

void PopStarOne::HandleCameraMovement()
{
	// Camera movement
	const XMFLOAT3 kirbyPos = GameManager::Get()->GetKirbyPosition();
	const XMFLOAT3 newPos{ kirbyPos.x + m_CameraOffset.x, kirbyPos.y + m_CameraOffset.y, kirbyPos.z + m_CameraOffset.z };

	const float t = m_CameraSnapSpeed * m_SceneContext.pGameTime->GetElapsed();
	XMStoreFloat3(&m_CameraPosition, XMVectorLerp(XMLoadFloat3(&m_CameraPosition), XMLoadFloat3(&newPos), t));

	m_pCamera->GetTransform()->Rotate(m_CameraRotation);
	m_pCamera->GetTransform()->Translate(m_CameraPosition);

	// Update light
	const XMFLOAT3 lightPos{ newPos.x + m_LightOffset.x, m_LightOffset.y, m_LightOffset.z };
	m_SceneContext.pLights->SetDirectionalLight(lightPos, m_LightRotation);
	
}

GameObject* PopStarOne::SpawnBobo(const XMFLOAT3& spawnLocation)
{
	auto* pBobo = new BoboPrefab();
	pBobo->GetTransform()->Translate(50, 2, 0);
	AddChild(pBobo);
	pBobo->GetTransform()->Translate(spawnLocation);

	return pBobo;
}

GameObject* PopStarOne::SpawnRocky(const XMFLOAT3& spawnLocation)
{
	auto* pRocky = new RockyPrefab();
	pRocky->GetTransform()->Translate(25, 2, 0);
	AddChild(pRocky);
	pRocky->GetTransform()->Translate(spawnLocation);

	return pRocky;
}

void PopStarOne::SpawnBackGroundBoxes()
{
	for (UINT count = 0; count < 14; ++count)
	{
		constexpr float boxSpacingX = 50.f;
		constexpr float boxSpacingY = 5.f;
		constexpr float boxSpacingZ = 25.f;
		constexpr XMFLOAT3 startPosition{ -230.f, 7.5f, 40.f };

		auto* pBox = AddChild(new GameObject());
		auto* pModelComponent = pBox->AddComponent(new ModelComponent(L"Meshes/BackgroundBox.ovm"));
		auto* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
		pMaterial->SetDiffuseTexture(L"Textures/BackgroundBox.png");
		pModelComponent->SetMaterial(pMaterial);

		pBox->GetTransform()->Scale(0.1f, 0.1f, 0.1f);

		XMFLOAT3 boxPosition{ startPosition.x + (count * boxSpacingX), (MathHelper::randF(-1,1) * boxSpacingY) + startPosition.y, (MathHelper::randF(-3,3) * boxSpacingZ) + startPosition.z };
		pBox->GetTransform()->Translate(boxPosition);
	}
}

GameObject* PopStarOne::AddPathBox(const XMFLOAT3& location)
{
	auto* pBox = new GameObject();
	auto* pModelComponent = pBox->AddComponent(new ModelComponent(L"Meshes/PathBox.ovm"));
	auto* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pMaterial->SetDiffuseTexture(L"Textures/PathBox.png");
	pModelComponent->SetMaterial(pMaterial);

	pBox->GetTransform()->Translate(location);

	AddChild(pBox);
	return pBox;
}

GameObject* PopStarOne::AddBridge(const XMFLOAT3& location)
{
	auto* pBridge = new GameObject();
	auto* pModelComponent = pBridge->AddComponent(new ModelComponent(L"Meshes/Bridge.ovm"));
	auto* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pMaterial->SetDiffuseTexture(L"Textures/Bridge.png");
	pModelComponent->SetMaterial(pMaterial);

	pBridge->GetTransform()->Rotate(0, 90, 0);
	pBridge->GetTransform()->Scale(0.1f, 0.1f, 0.1f);

	pBridge->GetTransform()->Translate(location);

	AddChild(pBridge);
	return pBridge;
}
