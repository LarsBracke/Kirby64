#include "stdafx.h"
#include "ModelAnimator.h"

#include <sstream>

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
	m_pMeshFilter{pMeshFilter}
{
	SetAnimation(0);
}

void ModelAnimator::Update(const SceneContext& sceneContext)
{
	//TODO_W7_();

	if (m_IsPlaying && m_ClipSet)
	{
		// Calculating passed ticks
		const float elapsedSeconds = sceneContext.pGameTime->GetElapsed();
		const float ticksPerSecond = m_CurrentClip.ticksPerSecond;
		const float animationSpeed = m_AnimationSpeed;
		float passedTicks = elapsedSeconds * ticksPerSecond * animationSpeed;
		passedTicks = fmodf(passedTicks, m_CurrentClip.duration);

		// Update tick count
		if (m_Reversed)
		{
			m_TickCount -= passedTicks;
			if (m_TickCount < 0)
				m_TickCount += m_CurrentClip.duration;
		}
		else
		{
			m_TickCount += passedTicks;
			if (m_TickCount > m_CurrentClip.duration)
				m_TickCount -= m_CurrentClip.duration;
		}

		// Find enclosing keys
		float tickDelta = std::numeric_limits<float>::max();
		unsigned int keyIndex = std::numeric_limits<unsigned int>::min();
		for (unsigned int index = 0; index < m_CurrentClip.keys.size(); ++index)
		{
			const AnimationKey& key = m_CurrentClip.keys[index];
			const float delta = m_TickCount - key.tick;
			if (delta < tickDelta && delta > 0)
			{
				tickDelta = delta;
				keyIndex = index;
			}
		}
		const AnimationKey keyA = m_CurrentClip.keys[keyIndex];
		const AnimationKey keyB = m_CurrentClip.keys[keyIndex + 1];

		// Blend factor
		const float keyDelta = keyB.tick - keyA.tick;
		const float blendFactor = (m_TickCount - keyA.tick) / keyDelta;

		// Clear transforms
		m_Transforms.clear();
		m_Transforms.resize(keyA.boneTransforms.size());

		// Interpolate transforms
		for (unsigned int boneIndex = 0; boneIndex < keyA.boneTransforms.size(); ++boneIndex)
		{
			// Get transforms from keys
			auto transformA = keyA.boneTransforms[boneIndex];
			auto transformB = keyB.boneTransforms[boneIndex];

			// Decompose transforms
			XMFLOAT3 scaleA{ }, scaleB{ };
			XMFLOAT4 rotQuatA{ }, rotQuatB{ };
			XMFLOAT3 transA{ }, transB{ };
			XMVECTOR scale{ }, rot{ }, trans{ };

			XMMATRIX transform = XMLoadFloat4x4(&transformA);
			XMMatrixDecompose(&scale, &rot, &trans, transform);
			XMStoreFloat3(&scaleA, scale); XMStoreFloat4(&rotQuatA, rot); XMStoreFloat3(&transA, trans);

			transform = XMLoadFloat4x4(&transformB);
			XMMatrixDecompose(&scale, &rot, &trans, transform);
			XMStoreFloat3(&scaleB, scale); XMStoreFloat4(&rotQuatB, rot); XMStoreFloat3(&transB, trans);

			// Interpolate
			XMVECTOR valueA = XMLoadFloat3(&scaleA);
			XMVECTOR valueB = XMLoadFloat3(&scaleB);
			scale = XMVectorLerp(valueA, valueB, blendFactor);

			valueA = XMLoadFloat4(&rotQuatA);
			valueB = XMLoadFloat4(&rotQuatB);
			rot = XMQuaternionSlerp(valueA, valueB, blendFactor);

			valueA = XMLoadFloat3(&transA);
			valueB = XMLoadFloat3(&transB);
			trans = XMVectorLerp(valueA, valueB, blendFactor);

			// Create and store new transform
			transform = XMMatrixAffineTransformation(scale, XMVectorZero(), rot, trans);
			XMFLOAT4X4 newTransform{ };
			XMStoreFloat4x4(&newTransform, transform);
			m_Transforms[boneIndex] = newTransform;
		}
	}
}

void ModelAnimator::SetAnimation(const std::wstring& clipName)
{
	m_ClipSet = false;

	for (AnimationClip& clip : m_pMeshFilter->m_AnimationClips)
	{
		if (clip.name == clipName)
		{
			SetAnimation(clip);
			return;
		}
	}

	Reset();
	std::wstringstream warning{ };
	warning << L"Clip with " << clipName << L" was not found" << std::endl;
	Logger::LogWarning(warning.str());
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	m_ClipSet = false;

	if (clipNumber < static_cast<UINT>(m_pMeshFilter->m_AnimationClips.size()))
	{
		const AnimationClip& clip = m_pMeshFilter->m_AnimationClips[clipNumber];
		SetAnimation(clip);
	}
	else
	{
		Reset();
		std::wstringstream warning{ };
		warning << L"Clip with number " << clipNumber << L" not found" << std::endl;
		Logger::LogWarning(warning.str());
	}
}

void ModelAnimator::SetAnimation(const AnimationClip& clip)
{
	m_ClipSet = true;
	m_CurrentClip = clip;
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	if (pause)
		m_IsPlaying = false;

	m_TickCount = 0;
	m_AnimationSpeed = 1.0f;

	if (m_ClipSet)
	{
		const auto begin = m_CurrentClip.keys[0].boneTransforms.begin();
		const auto end = m_CurrentClip.keys[0].boneTransforms.end();
		m_Transforms.assign(begin, end);
	}
	else
	{
		XMFLOAT4X4 identity{ };
		XMStoreFloat4x4(&identity, XMMatrixIdentity());
		m_Transforms.assign(m_pMeshFilter->m_BoneCount, identity);
	}
}
