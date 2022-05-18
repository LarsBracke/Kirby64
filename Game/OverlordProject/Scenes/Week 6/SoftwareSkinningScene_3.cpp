#include "stdafx.h"
#include "SoftwareSkinningScene_3.h"

#include "Materials/ColorMaterial.h"
#include "Helpers/MyHelpers.h"

void SoftwareSkinningScene_3::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;

	ColorMaterial* pColorMaterial = MaterialManager::Get()->CreateMaterial<ColorMaterial>();

	const auto pRoot = new GameObject();
	AddChild(pRoot);
	m_pBone0 = new BoneObject(pColorMaterial, 15.f);
	m_pBone1 = new BoneObject(pColorMaterial, 15.f);
	m_pBone0->AddBone(m_pBone1);
	pRoot->AddChild(m_pBone0);

	m_pBone0->CalculateBindPose();

	const auto pBoxDrawer = new GameObject();
	AddChild(pBoxDrawer);
	m_pMeshDrawer = pBoxDrawer->AddComponent(new MeshDrawComponent(24, true));

	InitializeVertices(15.f);
}

void SoftwareSkinningScene_3::Update()
{
	/*transform vertices*/
	const XMMATRIX boneWorldTransform0 = XMLoadFloat4x4(&m_pBone0->GetTransform()->GetWorld());
	const XMMATRIX boneWorldTransform1 = XMLoadFloat4x4(&m_pBone1->GetTransform()->GetWorld());
	const XMMATRIX boneBindPose0 = XMLoadFloat4x4(&m_pBone0->GetBindPose());
	const XMMATRIX boneBindPose1 = XMLoadFloat4x4(&m_pBone1->GetBindPose());
	const XMMATRIX boneTransform0 = XMMatrixMultiply(boneBindPose0, boneWorldTransform0);
	const XMMATRIX boneTransform1 = XMMatrixMultiply(boneBindPose1, boneWorldTransform1);

	for (int index = 0; index < m_SkinnedVertices.size(); ++index) /*bone0*/
	{
		VertexSoftWareSkinned& v{ m_SkinnedVertices[index] };

		const XMVECTOR originalVertex = XMLoadFloat3(&v.originalVertex.Position);
		const XMVECTOR transformedVertex0 = XMVector3TransformCoord(originalVertex, boneTransform0);
		const XMVECTOR transformedVertex1 = XMVector3TransformCoord(originalVertex, boneTransform1);

		XMVECTOR finalPosition = XMVectorScale(transformedVertex0, v.blendWeight0) + XMVectorScale(transformedVertex1, v.blendWeight1);
		XMStoreFloat3(&v.transformedVertex.Position, finalPosition);
	}

	/*draw vertices*/
	m_pMeshDrawer->RemoveTriangles();
	for (int index = 0; index < m_SkinnedVertices.size(); index += 4)
	{
		const VertexPosNormCol v0 = m_SkinnedVertices[index].transformedVertex;
		const VertexPosNormCol v1 = m_SkinnedVertices[index + 1].transformedVertex;
		const VertexPosNormCol v2 = m_SkinnedVertices[index + 2].transformedVertex;
		const VertexPosNormCol v3 = m_SkinnedVertices[index + 3].transformedVertex;
		m_pMeshDrawer->AddQuad(v0, v1, v2, v3);
	}
	m_pMeshDrawer->UpdateBuffer();

	/*animation (transform bones)*/
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

void SoftwareSkinningScene_3::OnGUI()
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

void SoftwareSkinningScene_3::InitializeVertices(float length)
{
	auto pos = XMFLOAT3(length / 2.f, 0.f, 0.f);
	const auto offset = XMFLOAT3(length / 2.f, 2.5f, 2.5f);
	auto col = XMFLOAT4(1.f, 0.f, 0.f, 0.5f);
#pragma region BOX 1
	//FRONT
	XMFLOAT3 norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 1.f, 0.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 1.f, 0.f);
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 1.f, 0.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 1.f, 0.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 1.f, 0.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 1.f, 0.f);
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 1.f, 0.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 1.f, 0.f);
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 1.f, 0.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 1.f, 0.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 1.f, 0.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 1.f, 0.f);
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
#pragma endregion

	col = { 0.f, 1.f, 0.f, 0.5f };
	pos = { 22.5f, 0.f, 0.f };
#pragma region BOX 2
	//FRONT
	norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.f, 1.f);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f ,0.5f);
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.5f, 0.5f);
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, 0.f, 1.f);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, 0.f, 1.f);
#pragma endregion
}
