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
public:
	void targetCheck();

	SceneGame(Stage* stage, Player* player, CameraController* cameraController)
		: stage(stage), player(player), cameraController(cameraController)
	{}
	static void RoadModel()
	{
		
		//的のロード
		extern Model* models[4];

		models[0] = new Model("Data/Model/Target/target_1.mdl");
		models[1] = new Model("Data/Model/Target/target_2.mdl");
		models[2] = new Model("Data/Model/Target/target_3.mdl");
		models[3] = new Model("Data/Model/Target/target_4.mdl");

		//問題文のロード
		extern Model* model_boad[21];

		model_boad[0] = new Model("Data/Model/Boad/boad_tutorial.mdl");
		model_boad[1] = new Model("Data/Model/Boad/boad_1.mdl");
		model_boad[2] = new Model("Data/Model/Boad/boad_2.mdl");
		model_boad[3] = new Model("Data/Model/Boad/boad_3.mdl");
		model_boad[4] = new Model("Data/Model/Boad/boad_4.mdl");
		model_boad[5] = new Model("Data/Model/Boad/boad_5.mdl");
		model_boad[6] = new Model("Data/Model/Boad/boad_6.mdl");
		model_boad[7] = new Model("Data/Model/Boad/boad_7.mdl");
		model_boad[8] = new Model("Data/Model/Boad/boad_8.mdl");
		model_boad[9] = new Model("Data/Model/Boad/boad_9.mdl");
		model_boad[10] = new Model("Data/Model/Boad/boad_10.mdl");
		model_boad[11] = new Model("Data/Model/Boad/boad_11.mdl");
		model_boad[12] = new Model("Data/Model/Boad/boad_12.mdl");
		model_boad[13] = new Model("Data/Model/Boad/boad_13.mdl");
		model_boad[14] = new Model("Data/Model/Boad/boad_14.mdl");
		model_boad[15] = new Model("Data/Model/Boad/boad_15.mdl");
		model_boad[16] = new Model("Data/Model/Boad/boad_16.mdl");
		model_boad[17] = new Model("Data/Model/Boad/boad_17.mdl");
		model_boad[18] = new Model("Data/Model/Boad/boad_18.mdl");
		model_boad[19] = new Model("Data/Model/Boad/boad_19.mdl");
		model_boad[20] = new Model("Data/Model/Boad/boad_20.mdl");

	};

};
