#include "stdafx.h"
#include "TrainingStage.h"

#include "Components/KirbyController.h"
#include "Helpers/GameManager.h"
#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/DiffuseMaterial_Skinned.h"

void TrainingStage::Initialize()
{
	/*settings*/
	m_SceneContext.settings.drawGrid = true;

	/*ground plane*/
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	/*stage*/
	GameObject* pObject = new GameObject();
	AddChild(pObject);
	pObject->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	pObject->GetTransform()->Rotate(0.f, 90.f, 0.f);

	ModelComponent* pModelComponent = pObject->AddComponent(new ModelComponent(L"Meshes/TrainingStage.ovm"));
	DiffuseMaterial* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMaterial->SetDiffuseTexture(L"Textures/TrainingStage.png");
	pModelComponent->SetMaterial(pMaterial);

	/*kirby*/
	auto* pKirby = new GameObject();
	AddChild(pKirby);

	pKirby->GetTransform()->Scale(25.f, 25.f, 25.f);
	pKirby->GetTransform()->Rotate(90.f, -90.f, 0.f);

	pModelComponent = pKirby->AddComponent(new ModelComponent(L"Meshes/Kirby.ovm"));
	auto* pMainMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMainMaterial->SetDiffuseTexture(L"Textures/kirby_kirby_diffuse.png");
	auto* pSecundaryMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pSecundaryMaterial->SetDiffuseTexture(L"Textures/kirby_foot_diffuse.png");
	pModelComponent->SetMaterial(pMainMaterial, 1);
	pModelComponent->SetMaterial(pSecundaryMaterial, 0);

	//pKirby->AddComponent(new KirbyController())


}

void TrainingStage::Update()
{
}
