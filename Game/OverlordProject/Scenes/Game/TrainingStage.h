#pragma once

class TrainingStage final : public GameScene
{
public:
	TrainingStage() : GameScene(L"TrainingStage") { }
	~TrainingStage() override = default;
	TrainingStage(const TrainingStage& other) = delete;
	TrainingStage(TrainingStage&& other) noexcept = delete;
	TrainingStage& operator=(const TrainingStage& other) = delete;
	TrainingStage& operator=(TrainingStage&& other) noexcept = delete;

private:
	void Initialize() override;
	void Update() override;
};

