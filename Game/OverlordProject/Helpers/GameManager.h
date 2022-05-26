#pragma once
#include "Singleton.h"

#include "stdafx.h"

class GameManager : public Singleton<GameManager>
{
public:
	void RegisterKirby(GameObject* pKirby) { m_pKirby = pKirby; };
	void RegisterEnemy(GameObject* pEnemy) { m_Enemies.push_back(pEnemy); }
	void UnRegisterEnemy(GameObject* pEnemy) { m_Enemies.erase(std::remove(m_Enemies.begin(), m_Enemies.end(), pEnemy), m_Enemies.end()); }

	void Initialize() override { };

	GameManager(const GameManager& other) = delete;
	GameManager(GameManager&& other) noexcept = delete;
	GameManager& operator=(const GameManager& other) = delete;
	GameManager& operator=(GameManager&& other) noexcept = delete;

	GameObject* GetClosestEnemy(float range = 50.0f) const;
	const XMFLOAT3& GetKirbyPosition() const { return m_pKirby->GetTransform()->GetPosition(); }

	bool IsKirbyInRange(XMFLOAT3 origin, float range) const;

	void ShootStar(XMFLOAT3 position, XMFLOAT3 direction);

private:
	friend Singleton<GameManager>;
	GameManager() = default;
	~GameManager() = default;

private:
	GameObject* m_pKirby;
	std::vector<GameObject*> m_Enemies;

	GameObject* m_pStars{ nullptr };
};

