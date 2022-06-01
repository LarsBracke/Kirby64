#include "stdafx.h"
#include "PostBlur.h"

PostBlur::PostBlur()
	: PostProcessingMaterial(L"Effects/Post/Blur.fx")
{ }

void PostBlur::Initialize(const GameContext&)
{ }
