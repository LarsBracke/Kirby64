#include "stdafx.h"
#include "ModelTestScene.h"

#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"
#include "Components/ModelComponent.h"

void ModelTestScene::Initialize()
{
	// Scene settings
	m_SceneContext.settings.enableOnGUI = true;

	// Ground plane
	auto& physX = PxGetPhysics();
	PxMaterial* pMaterial = physX.createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pMaterial);

	// Chair
	m_pChair = new GameObject();
	m_pChair->GetTransform()->Translate(0, 2.5f, 0);

	ModelComponent* pModelComponent = m_pChair->AddComponent(new ModelComponent(L"Meshes/Chair.ovm"));
	m_pDiffuseMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	m_pDiffuseMaterial->SetDiffuseTexture(L"Textures/Chair_Dark.dds");
	pModelComponent->SetMaterial(m_pDiffuseMaterial);

	RigidBodyComponent* pRigidBody = m_pChair->AddComponent(new RigidBodyComponent(false));
	PxConvexMesh* pConvexMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/Chair.ovpc");
	pRigidBody->AddCollider(PxConvexMeshGeometry{ pConvexMesh }, *pMaterial);

	AddChild(m_pChair);
}

void ModelTestScene::Update()
{ }

void ModelTestScene::OnGUI()
{
	m_pDiffuseMaterial->DrawImGui();
}

void ModelTestScene::OnSceneActivated()
{
	m_pChair->GetTransform()->Translate(0, 2.5f, 0);
}
