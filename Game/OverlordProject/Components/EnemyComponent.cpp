#include "stdafx.h"
#include "EnemyComponent.h"

#include "Helpers/GameManager.h"

void EnemyComponent::Kill()
{
	GameManager::Get()->UnRegisterEnemy(m_pGameObject);
	m_pGameObject->MarkForDeletion();
}
