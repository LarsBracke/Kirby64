#pragma once

class PopStarOne final : public GameScene
{
public:
	PopStarOne() : GameScene(L"PopStarOne") { }
	~PopStarOne() override = default;
	PopStarOne(const PopStarOne& other) = delete;
	PopStarOne(PopStarOne&& other) noexcept = delete;
	PopStarOne& operator=(const PopStarOne& other) = delete;
	PopStarOne& operator=(PopStarOne&& other) noexcept = delete;

private:
	void Initialize() override;
	void Update() override;
};

