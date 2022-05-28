#include "stdafx.h"
#include "PopStarOne.h"

#include "Helpers/GameManager.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/KirbyPrefab.h"
#include "Prefabs/BoboPrefab.h"
//#include "Prefabs/RockyPrefab.h"
#include "Prefabs/InGameHudPrefab.h"

void PopStarOne::Initialize()
{
	/*settings*/
	m_SceneContext.settings.drawGrid = true;

	/*ground plane*/
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	/*set light*/
	m_SceneContext.pLights->SetDirectionalLight({ -95.6139526f,66.1346436f,-41.1850471f }, { 0.740129888f, -0.597205281f, 0.309117377f });

	/*stage*/
	auto* pLevel = new GameObject();
	AddChild(pLevel);

	auto* pGroundPlane = new GameObject();
	pGroundPlane->GetTransform()->Rotate(0, 90, 0);
	pGroundPlane->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	pGroundPlane->GetTransform()->Translate(0, 0, 100);
	auto* pModelComponent = pGroundPlane->AddComponent(new ModelComponent(L"Meshes/GroundPlane.ovm"));
	auto* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMaterial->SetDiffuseTexture(L"Textures/GroundPlane.png");
	pModelComponent->SetMaterial(pMaterial);
	pLevel->AddChild(pGroundPlane);

	auto* pPath = new GameObject();
	pPath->GetTransform()->Rotate(0, 90, 0);
	pPath->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	pModelComponent = pPath->AddComponent(new ModelComponent(L"Meshes/Path.ovm"));
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMaterial->SetDiffuseTexture(L"Textures/Path.png");
	pModelComponent->SetMaterial(pMaterial);
	pLevel->AddChild(pPath);

	auto* pSky = new GameObject();
	pSky->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	pSky->GetTransform()->Rotate(0, 0, 90);
	pSky->GetTransform()->Translate(0, 33.33f, 200);
	pModelComponent = pSky->AddComponent(new ModelComponent(L"Meshes/Sky.ovm"));
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMaterial->SetDiffuseTexture(L"Textures/Sky.png");
	pModelComponent->SetMaterial(pMaterial);
	pLevel->AddChild(pSky);

	/*kirby*/
	KirbyPrefab* pKirby{ new KirbyPrefab() };
	pKirby->GetTransform()->Translate(0, 10, 0);
	AddChild(pKirby);

	/*bobo*/
	auto* pBobo = new BoboPrefab();
	pBobo->GetTransform()->Translate(25, 2, 0);
	AddChild(pBobo);

	///*rocky*/
	//auto* pRocky = new RockyPrefab();
	//pRocky->GetTransform()->Translate(50, 2, 0);
	//AddChild(pRocky);

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

void PopStarOne::HandleCameraMovement()
{
	const XMFLOAT3 kirbyPos = GameManager::Get()->GetKirbyPosition();
	const XMFLOAT3 cameraPos = m_pCamera->GetTransform()->GetPosition();
	const XMFLOAT3 newPos{ kirbyPos.x + m_CameraOffset.x, kirbyPos.y + m_CameraOffset.y, kirbyPos.z + m_CameraOffset.z };

	m_pCamera->GetTransform()->Rotate(m_CameraRotation);
	m_pCamera->GetTransform()->Translate(newPos);
}
