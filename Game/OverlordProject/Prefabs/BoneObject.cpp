#include "stdafx.h"
#include "BoneObject.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float length)
	: m_pMaterial(pMaterial)
	, m_Length(length)
	
{ }

void BoneObject::AddBone(BoneObject* pBone)
{
	const XMFLOAT3 currentPos{ pBone->GetTransform()->GetPosition() };
	pBone->GetTransform()->Translate(m_Length, currentPos.y, currentPos.z);
	this->AddChild(pBone);
}

void BoneObject::CalculateBindPose()
{
	const XMMATRIX world = XMLoadFloat4x4(&GetTransform()->GetWorld());
	const XMMATRIX worldInverse = XMMatrixInverse(nullptr, world);
	XMStoreFloat4x4(&m_BindPose, worldInverse);

	const auto children = GetChildren<BoneObject>();
	for (auto* pChild : children)
		pChild->CalculateBindPose();
}

void BoneObject::Initialize(const SceneContext&)
{
	const auto pEmpty{ new GameObject() };
	this->AddChild(pEmpty);
	ModelComponent* pModelComponent = pEmpty->AddComponent(new ModelComponent(L"Meshes/Bone.ovm"));
	pModelComponent->SetMaterial(m_pMaterial);
	pEmpty->GetTransform()->Rotate(0.f, -90.f, 0.f);
	pEmpty->GetTransform()->Scale(m_Length, m_Length, m_Length);
}
