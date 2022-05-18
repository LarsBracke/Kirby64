#pragma once

class PongScene final : public GameScene
{
public:
	PongScene() : GameScene(L"PongScene") { }
	~PongScene() = default;
	PongScene(const PongScene& other) = delete;
	PongScene(PongScene&& other) noexcept = delete;
	PongScene& operator=(const PongScene& other) = delete;
	PongScene& operator=(PongScene&& other) noexcept = delete;

private:
	void Initialize() override;
	void PostInitialize() override;
	void Update() override;
	void OnGUI() override;

	void HandlePaddleMovement() const;
	void HandleBallLaunch();
	void HandleGameReset();

	void ResetGame();

private:
	// Input
	InputManager* m_pInput{ nullptr };

	enum InputIDs
	{
		LeftUp, LeftDown,
		RightUp, RightDown,
		Start,
		Reset
	};

	// Ball
	GameObject* m_pBall{ nullptr };
	const float m_BallFriction{ 0.0f };
	const float m_BallRestitution{ 0.9f };
	const float m_BallRadius{ 1.f };
	const float m_BallLaunchForce{ 20.f };
	bool m_GameStarted{ false };

	// Paddles
	GameObject* m_pLeftPaddle{ nullptr };
	GameObject* m_pRightPaddle{ nullptr };
	const float m_PaddleFriction{ 0.5f };
	const float m_PaddleRestitution{ 0.9f };
	const XMFLOAT3 m_PaddleSize{ 0.5f, 3.f, 6.66f };
	const float m_PaddleOffset{ 2.f };
	const float m_PaddleSpeed{ 15.f };

	// Level
	const float m_GroundFriction{ 0.0f };
	const float m_GroundRestitution{ 0.0f };
	std::vector<GameObject*> m_BounceWalls{ };
	std::vector<GameObject*> m_TriggerWalls{ };
	const float m_PlayAreaWidth{ 60.f };
	const float m_PlayAreaHeight{ 40.f };

	// Camera
	GameObject* m_pCamera{ nullptr };
	const float m_CameraHeight{ 50.f };
};

