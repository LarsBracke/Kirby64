#include "stdafx.h"
#include "UberMaterial.h"

void UberMaterial::InitializeEffectVariables()
{
	// Textures
	SetVariable_Scalar(L"gUseTextureDiffuse", true);
	SetVariable_Scalar(L"gUseTextureSpecularIntensity", true);
	SetVariable_Scalar(L"gUseTextureNormal", true);
	SetVariable_Scalar(L"gFlipGreenChannel", true);
	SetVariable_Scalar(L"gUseFresnelFalloff", true);
	SetVariable_Scalar(L"gUseEnvironmentMapping", true);
	SetVariable_Scalar(L"gTextureOpacityIntensity", false);

	// Lighting
	SetVariable_Scalar(L"gUseSpecularBlinn", true);

	// Colors
	SetVariable_Vector(L"gColorDiffuse", XMFLOAT4{ 0.1f,0.25f,0.75f,1 });
	SetVariable_Vector(L"gColorSpecular", XMFLOAT4{ 1,0,1,1 });
	SetVariable_Vector(L"gColorAmbient", XMFLOAT4{ 0,0,1,1 });

	// Other parameters
	SetVariable_Scalar(L"gShininess", 12);
	SetVariable_Scalar(L"gReflectionStrength", 0.25f);
	SetVariable_Scalar(L"gRefractionStrength", 0.50f);
	SetVariable_Scalar(L"gRefractionIndex", 0.33f);
	SetVariable_Scalar(L"gFresnelPower", 0.5f);
	SetVariable_Scalar(L"gFresnelMultiplier", 1.5f);
	SetVariable_Scalar(L"gFresnelHardness", 2.f);
	SetVariable_Scalar(L"gOpacityIntensity", 1.f);

}

void UberMaterial::SetDiffuseTexture(const std::wstring& texturePath)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(texturePath);
	SetVariable_Texture(L"gTextureDiffuse", m_pDiffuseTexture);
}

void UberMaterial::SetSpecularIntensityTexture(const std::wstring& texturePath)
{
	m_pSpecularIntensityTexture = ContentManager::Load<TextureData>(texturePath);
	SetVariable_Texture(L"gTextureSpecularIntensity", m_pSpecularIntensityTexture);
}

void UberMaterial::SetNormalTexture(const std::wstring& texturePath)
{
	m_pNormalTexture = ContentManager::Load<TextureData>(texturePath);
	SetVariable_Texture(L"gTextureNormal", m_pNormalTexture);
}

void UberMaterial::SetEnvironmentTexture(const std::wstring& texturePath)
{
	m_pEnvironmentTexture = ContentManager::Load<TextureData>(texturePath);
	SetVariable_Texture(L"gCubeEnvironment", m_pEnvironmentTexture);
}

void UberMaterial::SetOpacityTexture(const std::wstring& texturePath)
{
	m_pOpacityTexture = ContentManager::Load<TextureData>(texturePath);
	SetVariable_Texture(L"gTextureOpacity", m_pOpacityTexture);
}
