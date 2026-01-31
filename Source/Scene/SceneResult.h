#pragma once

#include "System/Sprite.h"
#include "Scene.h"
#include "../Game/Grid.h"

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

private:
	int beforeScore = 0;
	int temp = 0;

	std::unique_ptr<Sprite> sprite = nullptr;
	std::unique_ptr<Sprite> sprite_number = nullptr;
};
