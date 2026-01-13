#pragma once

#include "../Game/Stage.h"
#include "../Game/Player.h"
#include "CameraController.h"
#include "DirectXMath.h"
#include "System/Sprite.h"
#include "Scene.h"
#include "../Game/Balloon.h"
#include "../Game/Board.h"
#include "../Game/Box.h"

// ゲームシーン
class SceneTutorial : public Scene
{
public:
	SceneTutorial() {};
	~SceneTutorial() override {};

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

	// GUI描画
	void DrawGUI() override;

private:
	Stage* stage = nullptr;

	Player* player = nullptr;

	Sprite* sprite = nullptr;

	Balloon* balloon = nullptr;

	Board* board = nullptr;

	Box* box = nullptr;

	Sprite* sprite2 = nullptr;
	Sprite* sprite3 = nullptr;

	CameraController* cameraController = nullptr;
	int change = 0;
	
public:
	

	SceneTutorial(Stage* stage, Player* player, CameraController* cameraController)
		: stage(stage), player(player), cameraController(cameraController)
	{
	}
};
