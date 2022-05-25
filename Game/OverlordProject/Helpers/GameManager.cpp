#include "stdafx.h"
#include "GameManager.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Materials/ColorMaterial.h"

GameObject* GameManager::GetClosestEnemy() const
{
	const XMVECTOR kirbyPosition = XMLoadFloat3(&m_pKirby->GetTransform()->GetPosition());
	float distance{  };
	GameObject* pClosestEnemy{ nullptr };
	for (auto* pEnemy : m_Enemies)
	{
		const XMVECTOR enemyPosition = XMLoadFloat3(&pEnemy->GetTransform()->GetPosition());
		const XMVECTOR toEnemy = XMVectorSubtract(enemyPosition, kirbyPosition);
		const XMVECTOR length = XMVector3Length(toEnemy);
		float temp{ };
		XMStoreFloat(&temp, length);
		if (temp < distance)
		{
			distance = abs(temp);
			pClosestEnemy = pEnemy;
		}
	}

	return pClosestEnemy;
}

bool GameManager::IsKirbyInRange(XMFLOAT3 origin, float range) const
{
	if (!m_pKirby)
		return false;

	const XMVECTOR kirbyPosition = XMLoadFloat3(&m_pKirby->GetTransform()->GetPosition());
	const XMVECTOR myPosition = XMLoadFloat3(&origin);
	const XMVECTOR toKirby = XMVectorSubtract(kirbyPosition, myPosition);
	float distance{ };
	XMStoreFloat(&distance, XMVector3Length(toKirby));

	return distance < range;
}
