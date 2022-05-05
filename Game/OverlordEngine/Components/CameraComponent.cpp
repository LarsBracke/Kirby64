#include "stdafx.h"
#include "CameraComponent.h"

CameraComponent::CameraComponent() :
	m_FarPlane(2500.0f),
	m_NearPlane(0.1f),
	m_FOV(XM_PIDIV4),
	m_Size(25.0f),
	m_PerspectiveProjection(true)
{
	XMStoreFloat4x4(&m_Projection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewInverse, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjectionInverse, XMMatrixIdentity());
}

void CameraComponent::Update(const SceneContext& sceneContext)
{
	// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
	using namespace DirectX;

	XMMATRIX projection{};

	if (m_PerspectiveProjection)
	{
		projection = XMMatrixPerspectiveFovLH(m_FOV, sceneContext.aspectRatio, m_NearPlane, m_FarPlane);
	}
	else
	{
		const float viewWidth = (m_Size > 0) ? m_Size * sceneContext.aspectRatio : sceneContext.windowWidth;
		const float viewHeight = (m_Size > 0) ? m_Size : sceneContext.windowHeight;
		projection = XMMatrixOrthographicLH(viewWidth, viewHeight, m_NearPlane, m_FarPlane);
	}

	const XMVECTOR worldPosition = XMLoadFloat3(&GetTransform()->GetWorldPosition());
	const XMVECTOR lookAt = XMLoadFloat3(&GetTransform()->GetForward());
	const XMVECTOR upVec = XMLoadFloat3(&GetTransform()->GetUp());

	const XMMATRIX view = XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);
	const XMMATRIX viewInv = XMMatrixInverse(nullptr, view);
	const XMMATRIX viewProjectionInv = XMMatrixInverse(nullptr, view * projection);

	XMStoreFloat4x4(&m_Projection, projection);
	XMStoreFloat4x4(&m_View, view);
	XMStoreFloat4x4(&m_ViewInverse, viewInv);
	XMStoreFloat4x4(&m_ViewProjection, view * projection);
	XMStoreFloat4x4(&m_ViewProjectionInverse, viewProjectionInv);
}

void CameraComponent::SetActive(bool active)
{
	if (m_IsActive == active) return;

	const auto pGameObject = GetGameObject();
	ASSERT_IF(!pGameObject, L"Failed to set active camera. Parent game object is null");

	if (!pGameObject) return; //help the compiler... (C6011)
	const auto pScene = pGameObject->GetScene();
	ASSERT_IF(!pScene, L"Failed to set active camera. Parent game scene is null");

	m_IsActive = active;
	pScene->SetActiveCamera(active?this:nullptr); //Switch to default camera if active==false
}

GameObject* CameraComponent::Pick(CollisionGroup ignoreGroups) const
{
	//TODO_W5(L"Implement Picking Logic")

	// Get mouse screen position
	POINT mouseScreenPos = InputManager::GetMousePosition();

	// Get screen properties
	const float screenWidth = GetScene()->GetSceneContext().windowWidth;
	const float screenHeight = GetScene()->GetSceneContext().windowHeight;
	const float halfWidth = screenWidth / 2.f;
	const float halfHeight = screenHeight / 2.f;

	// Mouse to ndc coords
	XMFLOAT2 mouseNdcPos{ };
	mouseNdcPos.x = (static_cast<float>(mouseScreenPos.x) - halfWidth) / halfWidth;
	mouseNdcPos.y = (halfHeight - static_cast<float>(mouseScreenPos.y)) / halfHeight;

	// Inverse view projection matrix
	const XMMATRIX viewProjection = XMLoadFloat4x4(&m_ViewProjection);
	const XMMATRIX viewProjInverse = XMMatrixInverse(nullptr, viewProjection);

	// Near point
	XMFLOAT4 ndcCoordsNear{ mouseNdcPos.x, mouseNdcPos.y, 0,0 };
	XMVECTOR calcVectorNear = XMLoadFloat4(&ndcCoordsNear);

	XMVECTOR resultVectorNear = XMVector3TransformCoord(calcVectorNear, viewProjInverse);

	XMFLOAT4 nearPoint{ };
	XMStoreFloat4(&nearPoint, resultVectorNear);

	// Far point
	XMFLOAT4 ndcCoordsFar = XMFLOAT4{ mouseNdcPos.x, mouseNdcPos.y, 1,0 };
	XMVECTOR calcVectorFar = XMLoadFloat4(&ndcCoordsFar);

	XMVECTOR resultVectorFar = XMVector3TransformCoord(calcVectorFar, viewProjInverse);

	XMFLOAT4 farPoint{ };
	XMStoreFloat4(&farPoint, resultVectorFar);

	// Raycast
	PxQueryFilterData filterData{ };
	filterData.data.word0 = ~UINT(ignoreGroups);

	PxRaycastBuffer hit{ };
	const PxVec3 rayStart{ nearPoint.x, nearPoint.y, nearPoint.z };
	PxVec3 rayDirection{ farPoint.x - nearPoint.x, farPoint.y - nearPoint.y, farPoint.z - nearPoint.z };
	rayDirection = rayDirection.getNormalized();

	if (GetScene()->GetPhysxProxy()->Raycast(rayStart, rayDirection, PX_MAX_F32, hit, PxHitFlag::eDEFAULT, filterData))
	{
		void* data = hit.block.actor->userData;

		if (auto pComponent = static_cast<BaseComponent*>(data))
		{
			GameObject* pGameObject{ pComponent->GetGameObject() };
			return pGameObject;
		}
	}

	return nullptr;
}