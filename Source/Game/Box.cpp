#include "Box.h"

//コンストラクタ
Box::Box()
{
	model = new Model("Data/Model/Boad/Box.mdl");

	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.8f;

	//幅、高さ設定
	radius = 0.2f;
	height = 0.0f;
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
}

//描画処理
void Box::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}
