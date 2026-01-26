#pragma once

#include "../Game/Stage.h"
#include "../Game/Player.h"
#include "CameraController.h"
#include "DirectXMath.h"
#include "System/Sprite.h"
#include "Scene.h"
#include "../Game/Box.h"

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

	//マス目の数
	enum { GRID_MAX = 5 };

	//ステージデータ
	int map[GRID_MAX][GRID_MAX] = { 0 };

	//移動後のデータいれて、比較するステージデータ
	bool merged[GRID_MAX][GRID_MAX] = { 0 };

	//移動しているかを判断
	bool moved = false;

	//重なったらtrue
	bool overlap = false;

	bool up = false;

	//mapの1マスの間隔
	float tileSize = 4.4f;

	//map[0][0]の位置
	DirectX::XMFLOAT3 startPos = { -8.8f, 0.0f, -13.0f };

	//方向キーでBox動かす関数
	bool pushUp();
	bool pushDown();
	bool pushLeft();
	bool pushRight();

	void UpdateCursorToggle();
};
