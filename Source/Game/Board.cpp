#include "Board.h"

#include <random>
#include <ctime>
#include <cstdlib>

Model* model_boad[20];
int board_answer = -1;
int BoardModel_index = -1;
bool CreateSelect = true;

//コンストラクタ
Board::Board()
{

	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.6f;

	//幅、高さ設定
	radius = 0.2f;
	height = 0.0f;
	if (CreateSelect == true)
	{
		CreateModel();
	}
	CreateSelect = false;
}

//デストラクタ
Board::~Board()
{
	
}

//更新処理
void Board::Update(float elapsedTime)
{
	//速力更新処理
	//UpdateVelocity(elapsedTime);

	//オブジェクト行列を更新
	UpdateTransform();

	//モデル行列更新
	//model_->UpdateTransform();

	//無敵時間更新
	UpdateInvincibleTimer(elapsedTime);
}

//描画処理
void Board::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model_boad[BoardModel_index], ShaderId::Lambert);
}

void Board::CreateModel()
{
	srand((unsigned int)time(NULL));

	/*std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int>dist(1, 20);
	
	BoardModel_index = dist(gen);
	board_answer = BoardModel_index;*/

	std::srand(static_cast<unsigned int>(std::time(0)));
	BoardModel_index = std::rand() % 20 + 1;
	board_answer = BoardModel_index;
}

void Board::OnDead()
{
	//自信を破棄
	/*if (scale.x > 0.0f)
	{
		Destroy();
	}*/
}
