#pragma once

#include "stdafx.h"

class HealthComponent : public BaseComponent
{
public:
	HealthComponent(unsigned int initialHealth);
	~HealthComponent() override = default;
	HealthComponent(const HealthComponent& other) = delete;
	HealthComponent(HealthComponent&& other) noexcept = delete;
	HealthComponent& operator=(const HealthComponent& other) = delete;
	HealthComponent& operator=(HealthComponent&& other) noexcept = delete;

	unsigned int GetHealth() const { return m_Health; }

	void DoDamage(unsigned int amount);
	void Heal(unsigned int amount);
	void ResetHealth();

private:
	const unsigned int m_InitialHealth;
	unsigned int m_Health;
};

