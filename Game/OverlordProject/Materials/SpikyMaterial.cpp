#include "stdafx.h"
#include "SpikyMaterial.h"

void SpikyMaterial::InitializeEffectVariables()
{
	SetVariable_Vector(L"gColorDiffuse", XMFLOAT4{ 1.f,0.f,0.f,1.f });
}
