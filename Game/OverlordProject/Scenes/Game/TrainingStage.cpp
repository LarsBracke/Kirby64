#include "stdafx.h"
#include "TrainingStage.h"

#include "Helpers/GameManager.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/KirbyPrefab.h"
#include "Prefabs/BoboPrefab.h"

void TrainingStage::Initialize()
{
	/*settings*/
	m_SceneContext.settings.drawGrid = true;

	/*ground plane*/
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	/*set light*/
	m_SceneContext.pLights->SetDirectionalLight({ -95.6139526f,66.1346436f,-41.1850471f }, { 0.740129888f, -0.597205281f, 0.309117377f });

	/*stage*/
	GameObject* pObject = new GameObject();
	AddChild(pObject);
	pObject->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	pObject->GetTransform()->Rotate(0.f, 90.f, 0.f);

	ModelComponent* pModelComponent = pObject->AddComponent(new ModelComponent(L"Meshes/TrainingStage.ovm"));
	auto* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pMaterial->SetDiffuseTexture(L"Textures/TrainingStage.png");
	pModelComponent->SetMaterial(pMaterial);

	/*kirby*/
	KirbyPrefab* pKirby{ new KirbyPrefab() };
	pKirby->GetTransform()->Translate(0, 10, 0);
	AddChild(pKirby);

	/*bobo*/
	auto* pBobo = new BoboPrefab();
	pBobo->GetTransform()->Translate(25, 2, 0);
	AddChild(pBobo);

	/*testing cube*/
	auto* pCube = new CubePrefab(5,5,5);
	pCube->AddComponent(new RigidBodyComponent())->AddCollider(PxBoxGeometry{ 2.5f,2.5f,2.5f }, *pDefaultMaterial);
	pCube->GetTransform()->Translate(10, 5, 0);
	AddChild(pCube);
}

void TrainingStage::Update()
{
}
