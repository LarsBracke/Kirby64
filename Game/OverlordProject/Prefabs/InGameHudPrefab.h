#pragma once
class InGameHudPrefab : public GameObject
{
public:
	InGameHudPrefab() = default;
	~InGameHudPrefab() override = default;

protected:
	void Initialize(const SceneContext&) override;
};

