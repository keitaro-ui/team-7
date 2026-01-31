#pragma once

#include "System/Sprite.h"
#include "Scene.h"

#include "../Game/Stage.h"
#include "../Game/Player.h"
#include "CameraController.h"
#include "DirectXMath.h"

#include "../Game/Box.h"
#include "../Game/Grid.h"
#include "SoundManager.h"
#include "SceneResult.h"

//タイトルシーン
class SceneTitle :public Scene
{
public:
	SceneTitle() {}
	~SceneTitle() override {}

	//初期化
	void Initialize() override;

	//終了化
	void Finalize() override;

	//更新処理
	void Update(float elapsedTime) override;

	//描画処理
	void Render() override;

	//GUI描画
	void DrawGUI() override;


private:

	CameraController* cameraController = nullptr;

	std::unique_ptr<Stage> titlestage = nullptr;

	std::unique_ptr<Player> player = nullptr;
	//std::unique_ptr<Model>model = nullptr;
	Sprite* sprite = nullptr;
	Sprite* sprite2 = nullptr;
	Sprite* sprite3 = nullptr;
	POINT cursorPos;
	float state = 0;
};
