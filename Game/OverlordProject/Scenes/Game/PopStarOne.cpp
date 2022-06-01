#include "stdafx.h"
#include "PopStarOne.h"

#include "Helpers/GameManager.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Misc/ShadowMapMaterial.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/KirbyPrefab.h"
#include "Prefabs/BoboPrefab.h"
#include "Prefabs/RockyPrefab.h"
#include "Prefabs/InGameHudPrefab.h"

class ShadowMapMaterial;

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

	SpawnBackGroundBoxes();

	/*kirby*/
	KirbyPrefab* pKirby{ new KirbyPrefab() };
	pKirby->GetTransform()->Translate(-275, 10, 0);
	AddChild(pKirby);

	/*bobo*/
	//SpawnBobo(XMFLOAT3{ -125, 2, 0 });
	//SpawnBobo(XMFLOAT3{ 50, 2, 0 });
	//SpawnBobo(XMFLOAT3{ 100, 2, 0 });
	//SpawnBobo(XMFLOAT3{ 125, 2, 0 });
	//SpawnBobo(XMFLOAT3{ 175, 2, 0 });

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

	/*testing cube*/
	auto* pCube = new CubePrefab(5,5,5);
	pCube->AddComponent(new RigidBodyComponent())->AddCollider(PxBoxGeometry{ 2.5f,2.5f,2.5f }, *pDefaultMaterial);
	pCube->GetTransform()->Translate(10, 5, 0);
	AddChild(pCube);

	/*camera*/
	m_pCamera = new FixedCamera();
	AddChild(m_pCamera);

	/*audio*/
	auto* pFmodSystem = SoundManager::Get()->GetSystem();
	FMOD::Sound* pMusic{ nullptr };
	auto result = pFmodSystem->createStream("Resources/Audio/Music.mp3", FMOD_DEFAULT, nullptr, &pMusic);
	HANDLE_ERROR(result);
	pMusic->setMode(FMOD_LOOP_NORMAL);
	result = pFmodSystem->playSound(pMusic, nullptr, false, &m_pMusicChannel);
	HANDLE_ERROR(result);
	m_pMusicChannel->setVolume(0.1f);
}

void PopStarOne::PostInitialize()
{
	m_pCameraComponent = m_pCamera->GetComponent<CameraComponent>();
	m_pCameraComponent->SetActive(true);
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
