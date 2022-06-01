#pragma once

#include "stdafx.h"

class PostOutline : public PostProcessingMaterial
{
public:
	PostOutline() : PostProcessingMaterial(L"Effects/Post/Outline.fx"){ }
	~PostOutline() override = default;
	PostOutline(const PostOutline& other) = delete;
	PostOutline(PostOutline&& other) noexcept = delete;
	PostOutline& operator=(const PostOutline& other) = delete;
	PostOutline& operator=(PostOutline&& other) noexcept = delete;

	void Initialize(const GameContext& gameContext) override;

private:
	
};

