#include "Box.h"

//コンストラクタ
Box::Box(int num)
{
	//if(num == 0) model = new Model("Data/Model/Boad/Box.mdl");
	if(num == 0) model = new Model("Data/Model/Boad/box_2.mdl");
	if(num == 1) model = new Model("Data/Model/Boad/box_4.mdl");
	if(num == 2) model = new Model("Data/Model/Boad/box_8.mdl");
	if(num == 3) model = new Model("Data/Model/Boad/box_16.mdl");
	if(num == 4) model = new Model("Data/Model/Boad/box_32.mdl");
	if(num == 5) model = new Model("Data/Model/Boad/box_64.mdl");
	if(num == 6) model = new Model("Data/Model/Boad/box_128.mdl");
	if(num == 7) model = new Model("Data/Model/Boad/box_256.mdl");
	if(num == 8) model = new Model("Data/Model/Boad/box_512.mdl");
	if(num == 9) model = new Model("Data/Model/Boad/box_1024.mdl");
	if(num == 10) model = new Model("Data/Model/Boad/box_2048.mdl");

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
