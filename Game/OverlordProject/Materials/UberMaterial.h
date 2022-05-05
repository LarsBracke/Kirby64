#pragma once
class UberMaterial final : public Material<UberMaterial>
{
public:
	UberMaterial() : Material(L"Effects/Uber.fx") { InitializeEffectVariables(); }
	~UberMaterial() override = default;

	UberMaterial(const UberMaterial& other) = delete;
	UberMaterial(UberMaterial&& other) noexcept = delete;
	UberMaterial& operator=(const UberMaterial& other) = delete;
	UberMaterial& operator=(UberMaterial&& other) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& texturePath);
	void SetSpecularIntensityTexture(const std::wstring& texturePath);
	void SetNormalTexture(const std::wstring& texturePath);
	void SetEnvironmentTexture(const std::wstring& texturePath);
	void SetOpacityTexture(const std::wstring& texturePath);

protected:
	void InitializeEffectVariables() override;

	TextureData* m_pDiffuseTexture{ nullptr };
	TextureData* m_pSpecularIntensityTexture{ nullptr };
	TextureData* m_pNormalTexture{ nullptr };
	TextureData* m_pEnvironmentTexture{ nullptr };
	TextureData* m_pOpacityTexture{ nullptr };
};

