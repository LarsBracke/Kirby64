#pragma once
#include "Materials/DiffuseMaterial.h"

class ModelTestScene : public GameScene
{
public:
	ModelTestScene() : GameScene(L"ModelTestScene") { }
	~ModelTestScene() { }

	ModelTestScene(const ModelTestScene& other) = delete;
	ModelTestScene(ModelTestScene&& other) noexcept = delete;
	ModelTestScene& operator=(const ModelTestScene& other) = delete;
	ModelTestScene& operator=(ModelTestScene&& other) noexcept = delete;

	void Initialize() override;
	void Update() override;
	void OnGUI() override;

	void OnSceneActivated() override;
private:
	GameObject* m_pChair{ nullptr };
	DiffuseMaterial* m_pDiffuseMaterial{ nullptr };
};
