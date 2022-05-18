#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"

#include "Materials/ColorMaterial.h"
#include "Helpers/MyHelpers.h"

void SoftwareSkinningScene_1::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;

	ColorMaterial* pColorMaterial = MaterialManager::Get()->CreateMaterial<ColorMaterial>();

	GameObject* pRoot = new GameObject();
	AddChild(pRoot);

	m_pBone0 = new BoneObject(pColorMaterial, 15.f);
	m_pBone1 = new BoneObject(pColorMaterial, 15.f);
	m_pBone0->AddBone(m_pBone1);

	pRoot->AddChild(m_pBone0);
}

void SoftwareSkinningScene_1::Update()
{
	if (!m_AutoRotate)
		return;

	const float rotationSpeed{ 45.f };
	const float minMaxRotation{ 45.f };
	const float deltaTime = m_SceneContext.pGameTime->GetElapsed();

	if (m_BoneRotation > minMaxRotation)
		m_RotationSign = -abs(m_RotationSign);
	if (m_BoneRotation < -minMaxRotation)
		m_RotationSign = abs(m_RotationSign);

	m_BoneRotation += (m_RotationSign * rotationSpeed * deltaTime);

	m_pBone0->GetTransform()->Rotate(0, 0, m_BoneRotation);
	m_pBone1->GetTransform()->Rotate(0, 0, -m_BoneRotation * 2.f);
}

void SoftwareSkinningScene_1::OnGUI()
{
	const float rotLimit = 90;

	ImGui::Checkbox("Auto-rotate", &m_AutoRotate);

	if (m_AutoRotate)
	{
		m_AutoRotation0 = MyHelpers::ToDegrees(MathHelper::QuaternionToEuler(m_pBone0->GetTransform()->GetRotation()));
		m_AutoRotation1 = MyHelpers::ToDegrees(MathHelper::QuaternionToEuler(m_pBone1->GetTransform()->GetRotation()));

		ImGui::SliderFloat3("Bone 0 - ROT", ConvertUtil::ToImFloatPtr(m_AutoRotation0), -rotLimit, rotLimit);
		ImGui::SliderFloat3("Bone 1 - ROT", ConvertUtil::ToImFloatPtr(m_AutoRotation1), -rotLimit, rotLimit);
	}
	else
	{
		ImGui::SliderFloat3("Bone 0 - ROT", ConvertUtil::ToImFloatPtr(m_ManualRotation0), -rotLimit, rotLimit);
		ImGui::SliderFloat3("Bone 1 - ROT", ConvertUtil::ToImFloatPtr(m_ManualRotation1), -rotLimit, rotLimit);

		m_pBone0->GetTransform()->Rotate(m_ManualRotation0.x, m_ManualRotation0.y, m_ManualRotation0.z);
		m_pBone1->GetTransform()->Rotate(m_ManualRotation1.x, m_ManualRotation1.y, m_ManualRotation1.z);
	}
}
