#include "Box.h"
#include<imgui.h>

//コンストラクタ
Box::Box()
{
	model = new Model("Data/Model/Boad/Box_2.mdl");

	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.000001f;

	//幅、高さ設定
	radius = 1.0f;
	height = 1.0f;
}

//デストラクタ
Box::~Box()
{
	delete model;
}

//更新処理
void Box::Update(float elapsedTime)
{
	//速力更新処理
	//UpdateVelocity(elapsedTime);

	//オブジェクト行列を更新
	UpdateTransform();

	//モデル行列更新
	model->UpdateTransform();

	//無敵時間更新
	UpdateInvincibleTimer(elapsedTime);

	ImGui::Begin("Model Scale");

	// 等方スケール用の1変数
	static float modelScale = 0.000001f;

	if (ImGui::DragFloat(
		"Scale",
		&modelScale,
		0.001f,   // 変化量
		0.0f,
		1.0f,
		"%.7f"))
	{
		scale.x = scale.y = scale.z = modelScale;
	}

	ImGui::End();
}

//描画処理
void Box::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}
