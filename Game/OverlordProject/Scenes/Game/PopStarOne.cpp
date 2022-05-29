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
	auto* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMaterial->SetDiffuseTexture(L"Textures/GroundPlane.png");
	pModelComponent->SetMaterial(pMaterial);
	pLevel->AddChild(pGroundPlane);

	auto* pPath = new GameObject();
	pPath->GetTransform()->Translate(0, 0.1f, 0);
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
	pSky->GetTransform()->Translate(0, 33.33f, 175);
	pModelComponent = pSky->AddComponent(new ModelComponent(L"Meshes/Sky.ovm"));
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMaterial->SetDiffuseTexture(L"Textures/Sky.png");
	pModelComponent->SetMaterial(pMaterial);
	pLevel->AddChild(pSky);

	//auto* pBackgroundProps = new GameObject();
	//pBackgroundProps->GetTransform()->Translate(0, 5, 75);
	//pBackgroundProps->GetTransform()->Rotate(0, 90, 0);
	//pBackgroundProps->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	//pModelComponent = pBackgroundProps->AddComponent(new ModelComponent(L"Meshes/BackgroundProps.ovm"));
	//pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	//pModelComponent->SetMaterial(pMaterial);
	//AddChild(pBackgroundProps);


	/*kirby*/
	KirbyPrefab* pKirby{ new KirbyPrefab() };
	pKirby->GetTransform()->Translate(0, 10, 0);
	AddChild(pKirby);

	/*bobo*/
	auto* pBobo = new BoboPrefab();
	pBobo->GetTransform()->Translate(50, 2, 0);
	AddChild(pBobo);

	/*rocky*/
	auto* pRocky = new RockyPrefab();
	pRocky->GetTransform()->Translate(25, 2, 0);
	AddChild(pRocky);

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
	// Camera movement
	const XMFLOAT3 kirbyPos = GameManager::Get()->GetKirbyPosition();
	const XMFLOAT3 newPos{ kirbyPos.x + m_CameraOffset.x, kirbyPos.y + m_CameraOffset.y, kirbyPos.z + m_CameraOffset.z };

	const float t = m_CameraSnapSpeed * m_SceneContext.pGameTime->GetElapsed();
	XMStoreFloat3(&m_CameraPosition, XMVectorLerp(XMLoadFloat3(&m_CameraPosition), XMLoadFloat3(&newPos), t));

	m_pCamera->GetTransform()->Rotate(m_CameraRotation);
	m_pCamera->GetTransform()->Translate(m_CameraPosition);

	// Update light
	m_SceneContext.pLights->SetDirectionalLight(m_pCamera->GetTransform()->GetPosition(), m_LightDirection);
}
