#pragma once
class MainMenu : public GameScene
{
public:
	MainMenu() : GameScene(L"Main menu"){ };
	~MainMenu() = default;
	MainMenu(const MainMenu& other) = delete;
	MainMenu(MainMenu&& other) noexcept = delete;
	MainMenu& operator=(const MainMenu& other) = delete;
	MainMenu& operator=(MainMenu&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;

	void OnSceneActivated() override;

private:
	SpriteComponent* m_pIndicator{ nullptr };
	SpriteComponent* m_pMainMenu{ nullptr };

	enum InputIDs
	{
		Start,
		Exit
	};
};

