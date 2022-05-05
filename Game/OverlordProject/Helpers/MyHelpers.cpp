#include <stdafx.h>
#include "MyHelpers.h"

XMFLOAT3 MyHelpers::ToDegrees(const XMFLOAT3& float3Radians)
{
	XMFLOAT3 degrees{ };
	degrees.x = float3Radians.x / PxPi * 180.f;
	degrees.y = float3Radians.y / PxPi * 180.f;
	degrees.z = float3Radians.z / PxPi * 180.f;
	return degrees;
}

XMFLOAT3 MyHelpers::ToRadians(const XMFLOAT3& float3Degrees)
{
	XMFLOAT3 radians{ };
	radians.x = float3Degrees.x / 180.f * PxPi;
	radians.y = float3Degrees.y / 180.f * PxPi;
	radians.z = float3Degrees.z / 180.f * PxPi;
	return radians;
}
