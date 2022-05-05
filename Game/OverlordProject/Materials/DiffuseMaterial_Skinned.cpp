#include "stdafx.h"
#include "DiffuseMaterial_Skinned.h"

void DiffuseMaterial_Skinned::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void DiffuseMaterial_Skinned::InitializeEffectVariables()
{

}

void DiffuseMaterial_Skinned::OnUpdateModelVariables(const SceneContext&, const ModelComponent* pModel) const
{
	ModelAnimator* pModelAnimator = pModel->GetAnimator();
	ASSERT_NULL_(pModelAnimator);

	const auto& boneTransforms = pModelAnimator->GetBoneTransforms();
	const float* boneData = reinterpret_cast<const float*>(boneTransforms.data());
	const unsigned int boneCount = static_cast<unsigned int>(boneTransforms.size());

	SetVariable_MatrixArray(L"gBones", boneData, boneCount);
}
