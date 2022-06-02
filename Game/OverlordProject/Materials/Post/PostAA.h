#pragma once

#include "stdafx.h"

class PostAA : public PostProcessingMaterial
{
public:
	PostAA() : PostProcessingMaterial(L"Effects/Post/AA.fx"){ }
	~PostAA() override = default;
	PostAA(const PostAA& other) = delete;
	PostAA(PostAA&& other) noexcept = delete;
	PostAA& operator=(const PostAA& other) = delete;
	PostAA& operator=(PostAA&& other) noexcept = delete;

	void Initialize(const GameContext& gameContext) override;

private:
	
};

