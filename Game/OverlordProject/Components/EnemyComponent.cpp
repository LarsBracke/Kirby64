#include "stdafx.h"
#include "EnemyComponent.h"

void EnemyComponent::Kill()
{
	m_pGameObject->MarkForDeletion();
}
