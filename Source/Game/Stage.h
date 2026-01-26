#pragma once

#include "Character.h"
#include "System/ModelRenderer.h";

//ステージ
class Stage:public Character
{
public:
	Stage();
	~Stage();

	//更新処理
	void Update(float elapsedTime);

	void DrawDebugGUI();

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);

private:
	Model* model = nullptr;
};

//class StageManager
//{
//private:
//	StageManager() {}
//	~StageManager() {}
//
//	Stage* currentStage;
//
//public:
//	static StageManager& Instance()
//	{
//		static StageManager instance;
//		return instance;
//	}
//
//	void Regist(Stage* stage) { currentStage = stage; }
//
//	Stage* GetStage() { return currentStage; }
//};
