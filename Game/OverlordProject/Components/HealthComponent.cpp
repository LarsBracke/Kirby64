#include "stdafx.h"
#include "HealthComponent.h"

HealthComponent::HealthComponent(unsigned int initialHealth)
	: m_InitialHealth(initialHealth)
	, m_Health(initialHealth)
{ }

void HealthComponent::DoDamage(unsigned int amount)
{
	if (amount >= m_Health)
		m_Health = 0;
	else
		m_Health -= amount;
}

void HealthComponent::Heal(unsigned int amount)
{
	m_Health += amount;
	MathHelper::Clamp(m_Health, m_InitialHealth, static_cast<unsigned int>(0));
}

void HealthComponent::ResetHealth()
{
	m_Health = m_InitialHealth;
}
