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

	std::unique_ptr<Stage> stage = nullptr;

	std::unique_ptr<Player> player = nullptr;

	Sprite* sprite = nullptr;
	Sprite* sprite_number = nullptr;
	Sprite* sprite_text = nullptr;

	Box* box = nullptr;

	CameraController* cameraController = nullptr;

	// tile
	//map->world座標変換関数
	DirectX::XMFLOAT3 CalcTilePosition(int x, int y);

	void UpdateCursorToggle();

	std::unique_ptr<Box> boxes[10];
	int map[4][4] = { 0 };
};
