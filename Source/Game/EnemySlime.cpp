#include "EnemySlime.h"

#include <random>
//#include <algorithm>

Model* models[4];


//コンストラクタ
EnemySlime::EnemySlime()
{

	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.1f;

	//幅、高さ設定
	radius = 0.2f;
	height = 0.0f;

	CreateModel();
}

//デストラクタ
EnemySlime::~EnemySlime()
{
	
}

//更新処理
void EnemySlime::Update(float elapsedTime)
{
	//速力更新処理
	//UpdateVelocity(elapsedTime);

	//オブジェクト行列を更新
	UpdateTransform();

	//モデル行列更新
	//model->UpdateTransform();

	//無敵時間更新
	UpdateInvincibleTimer(elapsedTime);
}

//描画処理
void EnemySlime::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, models[model_index], ShaderId::Lambert);

}


void EnemySlime::CreateModel()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int>dist(0, 3);

	model_index = dist(gen);
}

void EnemySlime::OnDead()
{
	//自信を破棄
	Destroy();
}
