#pragma once

#include "../Game/Stage.h"
#include "../Game/Player.h"
#include "CameraController.h"
#include "DirectXMath.h"
#include "System/Sprite.h"
#include "Scene.h"
#include "../Game/Box.h"

// ゲームシーン
class SceneGameproject : public Scene
{
public:
	SceneGameproject() {};
	~SceneGameproject() override {};

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
	float game_timer;

	Stage* stage = nullptr;

	Player* player = nullptr;

	Sprite* sprite = nullptr;
	Sprite* sprite_number = nullptr;
	Sprite* sprite_text = nullptr;

	Box* box = nullptr;

	CameraController* cameraController = nullptr;
public:

	SceneGameproject(Stage* stage, Player* player, CameraController* cameraController)
		: stage(stage), player(player), cameraController(cameraController)
	{
	}
};
