#pragma once
#include "Singleton.h"

#include "stdafx.h"

class GameManager : public Singleton<GameManager>
{
public:
	void RegisterKirby(GameObject* pKirby) { m_pKirby = pKirby; };
	void RegisterEnemy(GameObject* pEnemy) { m_Enemies.push_back(pEnemy); }

	void Initialize() override { };

	GameManager(const GameManager& other) = delete;
	GameManager(GameManager&& other) noexcept = delete;
	GameManager& operator=(const GameManager& other) = delete;
	GameManager& operator=(GameManager&& other) noexcept = delete;

	GameObject* GetClosestEnemy() const;

private:
	friend Singleton<GameManager>;
	GameManager() = default;
	~GameManager() = default;

private:
	GameObject* m_pKirby;
	std::vector<GameObject*> m_Enemies;
};

