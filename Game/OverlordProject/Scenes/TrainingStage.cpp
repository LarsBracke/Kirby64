#include "stdafx.h"
#include "TrainingStage.h"

#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"

void TrainingStage::Initialize()
{
	/*settings*/
	m_SceneContext.settings.drawGrid = false;

	/*testing*/
	GameObject* pObject = new GameObject();
	AddChild(pObject);
	pObject->GetTransform()->Scale(0.1f, 0.1f, 0.1f);

	ModelComponent* pModelComponent = pObject->AddComponent(new ModelComponent(L"Meshes/TrainingStage.ovm"));
	DiffuseMaterial* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMaterial->SetDiffuseTexture(L"Textures/TrainingStage.png");
	pModelComponent->SetMaterial(pMaterial);
}

void TrainingStage::Update()
{
}
