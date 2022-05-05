#include "stdafx.h"
#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(const std::wstring& spriteAsset, const XMFLOAT2& pivot, const XMFLOAT4& color):
	m_SpriteAsset(spriteAsset),
	m_Pivot(pivot),
	m_Color(color)
{}

void SpriteComponent::Initialize(const SceneContext& /*sceneContext*/)
{
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::SetTexture(const std::wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::Draw(const SceneContext& sceneContext)
{
	if (!m_pTexture)
		return;

	//TODO_W4(L"Draw the sprite with SpriteRenderer::Draw")

	// Here you need to draw the SpriteComponent using the Draw of the sprite renderer
	// The sprite renderer is a singleton
	// you will need to position (X&Y should be in screenspace, Z contains the depth between [0,1]), the rotation and the scale from the owning GameObject
	// You can use the MathHelper::QuaternionToEuler function to help you with the z rotation

	SpriteRenderer* pSpriteRenderer{ SpriteRenderer::Get() };

	const XMFLOAT3 position{ GetTransform()->GetPosition() };
	const XMFLOAT3 rotation{ MathHelper::QuaternionToEuler(GetTransform()->GetRotation()) };
	const XMFLOAT3 scale{ GetTransform()->GetScale() };
	
	pSpriteRenderer->AppendSprite(m_pTexture, XMFLOAT2{ position.x, position.y }, m_Color, m_Pivot, XMFLOAT2{ scale.x, scale.y }, rotation.z);
	pSpriteRenderer->Draw(sceneContext);
}
