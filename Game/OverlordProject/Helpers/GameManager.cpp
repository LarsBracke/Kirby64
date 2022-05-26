#include "stdafx.h"
#include "GameManager.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Materials/ColorMaterial.h"

GameObject* GameManager::GetClosestEnemy(float /*range*/) const
{
	//const XMVECTOR kirbyPosition = XMLoadFloat3(&m_pKirby->GetTransform()->GetPosition());
	//float distance{ 1'000'000 };
	//GameObject* pClosestEnemy{ nullptr };
	//for (auto* pEnemy : m_Enemies)
	//{
	//	if (pEnemy == nullptr)
	//		continue;

	//	const XMVECTOR enemyPosition = XMLoadFloat3(&pEnemy->GetTransform()->GetPosition());
	//	const XMVECTOR toEnemy = XMVectorSubtract(enemyPosition, kirbyPosition);
	//	const XMVECTOR length = XMVector3Length(toEnemy);
	//	float temp{ };
	//	XMStoreFloat(&temp, length);
	//	if (temp < distance && temp < range)
	//	{
	//		distance = abs(temp);
	//		pClosestEnemy = pEnemy;
	//	}
	//}

	//return pClosestEnemy;

	return nullptr;
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
