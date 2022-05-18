#include "stdafx.h"
#include "DiffuseMaterial_Shadow.h"

DiffuseMaterial_Shadow::DiffuseMaterial_Shadow():
	Material(L"Effects/Shadow/PosNormTex3D_Shadow.fx")
{}

void DiffuseMaterial_Shadow::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void DiffuseMaterial_Shadow::InitializeEffectVariables()
{
}

void DiffuseMaterial_Shadow::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
{
	/*
	 * TODO_W8
	 * Update The Shader Variables
	 * 1. Update the LightWVP > Used to Transform a vertex into Light clipping space
	 * 	LightWVP = model_world * light_viewprojection
	 * 	(light_viewprojection [LightVP] can be acquired from the ShadowMapRenderer)
	 *
	 * 2. Update the ShadowMap texture
	 *
	 * 3. Update the Light Direction (retrieve the direction from the LightManager > sceneContext)
	*/
	const XMMATRIX world = XMLoadFloat4x4(&pModel->GetTransform()->GetWorld());
	const XMMATRIX lightViewProj = XMLoadFloat4x4(&ShadowMapRenderer::Get()->GetLightVP());
	XMFLOAT4X4 lightWorldViewProj{ };

	XMStoreFloat4x4(&lightWorldViewProj, world * lightViewProj);
	const XMFLOAT4 lightDirection = sceneContext.pLights->GetDirectionalLight().direction;

	auto* pShadowMap = ShadowMapRenderer::Get()->GetShadowMap();

	//Update Shadow Variables
	SetVariable_Matrix(L"gWorldViewProj_Light", lightWorldViewProj);
	SetVariable_Vector(L"gLightDirection", lightDirection);
	SetVariable_Texture(L"gShadowMap", pShadowMap);
}
