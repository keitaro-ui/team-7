#pragma once

#include "../Game/Stage.h"
#include "../Game/Player.h"
#include "CameraController.h"
#include "DirectXMath.h"
#include "System/Sprite.h"
#include "Scene.h"
#include "../Game/Box.h"
#include "../Game/Grid.h"
#include "SoundManager.h"

// ゲームシーン
class SceneGame : public Scene
{
public:
	SceneGame() {};
	~SceneGame() override {};

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
	Grid grid;

	//std::unique_ptr<Grid>grid = nullptr;

	float game_timer;
	const float coolTime = 1.0f;

	std::unique_ptr<Stage> stage = nullptr;

	std::unique_ptr<Player> player = nullptr;

	Sprite* sprite = nullptr;
	Sprite* sprite_number = nullptr;
	Sprite* sprite_text = nullptr;

	CameraController* cameraController = nullptr;

	//使う箱の種類
	std::unique_ptr<Box> boxes[11];

	//重なったらtrue
	bool overlap = false;

	bool up = false;

	//mapの1マスの間隔
	float tileSize;
	//map[0][0]の位置
	DirectX::XMFLOAT3 startPos;

	bool isMoving = false;

	void UpdateCursorToggle();
};
