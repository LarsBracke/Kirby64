#pragma once
#include "Singleton.h"

class GameManager : public Singleton<GameManager>
{
public:
	GameManager() = default;
	~GameManager() = default;
	GameManager(const GameManager& other) = delete;
	GameManager(GameManager&& other) noexcept = delete;
	GameManager& operator=(const GameManager& other) = delete;
	GameManager& operator=(GameManager&& other) noexcept = delete;

	GameObject* CreateKirby();

private:

};

