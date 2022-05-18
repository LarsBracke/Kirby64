#pragma once
#include "Prefabs/BoneObject.h"

class SoftwareSkinningScene_3 final : public GameScene
{
public:
	SoftwareSkinningScene_3() : GameScene(L"SoftwareSkinningScene_3") { }
	~SoftwareSkinningScene_3() override = default;
	SoftwareSkinningScene_3(const SoftwareSkinningScene_3& other) = delete;
	SoftwareSkinningScene_3(SoftwareSkinningScene_3&& other) noexcept = delete;
	SoftwareSkinningScene_3& operator=(const SoftwareSkinningScene_3& other) = delete;
	SoftwareSkinningScene_3& operator=(SoftwareSkinningScene_3&& other) noexcept = delete;

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

	struct VertexSoftWareSkinned
	{
		VertexSoftWareSkinned(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT4 color, float blendWeight0, float blendWeight1)
			: transformedVertex(position, normal, color)
			, originalVertex(position, normal, color)
			, blendWeight0(blendWeight0)
			, blendWeight1(blendWeight1)
		{ }

		VertexPosNormCol transformedVertex{ };
		VertexPosNormCol originalVertex{ };
		float blendWeight0{ };
		float blendWeight1{ };
	};

	void InitializeVertices(float length);

	MeshDrawComponent* m_pMeshDrawer{ nullptr };
	std::vector<VertexSoftWareSkinned> m_SkinnedVertices{ };
};

