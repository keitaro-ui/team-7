//#pragma once

#include "../Source/Game/Stage.h"
#include "../Source/Game/Player.h"
#include "CameraController.h"
#include "DirectXMath.h"
#include "Scene.h"
#include "System/Sprite.h"

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

	Stage* stage = nullptr;

	Player* player = nullptr;

	//レティクル関数
	Sprite* sprite = nullptr;
	Sprite* sprite_number = nullptr;
	Sprite* sprite_text = nullptr;

	CameraController* cameraController = nullptr;

	// tile
	std::unique_ptr<Model> models[10];
	int map[4][4] = { 0 };

};
