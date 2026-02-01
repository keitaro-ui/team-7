#pragma once

#include "System/Sprite.h"
#include "Scene.h"
#include "../Game/Grid.h"
#include"../Game/player2.h"

//タイトルシーン
class SceneResult :public Scene
{
public:
	SceneResult() {}
	~SceneResult() override {}

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

	//void answerCheck();


	void UpdateCursorToggle();

private:
	int beforeScore = 0;
	int temp = 0;

	std::unique_ptr<Sprite> sprite = nullptr;
	std::unique_ptr<Sprite> sprite_number = nullptr;
	std::unique_ptr<Sprite> sprite_txt = nullptr;
	std::unique_ptr<Player2>player = nullptr;
	std::unique_ptr<Player2>player2 = nullptr;

	float numberPosX = 626.0f;
	float numberPosY = 167.0f;

	float NumberPosX = 626.0f;
	float NumberPosY = 462.0f;

	float playerY = -12.0f;

};
