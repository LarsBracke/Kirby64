#pragma once

#include "Prefabs/BoneObject.h"

class SoftwareSkinningScene_1 : public GameScene
{
public: 
	SoftwareSkinningScene_1() : GameScene(L"SoftwareSkinningScene_1") { }
	~SoftwareSkinningScene_1() override = default;
	SoftwareSkinningScene_1(const SoftwareSkinningScene_1& other) = delete;
	SoftwareSkinningScene_1(SoftwareSkinningScene_1&& other) = delete;
	SoftwareSkinningScene_1& operator=(const SoftwareSkinningScene_1& other) = delete;
	SoftwareSkinningScene_1& operator=(SoftwareSkinningScene_1&& other) = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	BoneObject* m_pBone0{ nullptr };
	BoneObject* m_pBone1{ nullptr };

	XMFLOAT3 m_ManualRotation0{ };
	XMFLOAT3 m_ManualRotation1{ };
	XMFLOAT3 m_AutoRotation0{ };
	XMFLOAT3 m_AutoRotation1{ };

	float m_BoneRotation{ };
	int m_RotationSign{ 1 };

	bool m_AutoRotate = true;
};

