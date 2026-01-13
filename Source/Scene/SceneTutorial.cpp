#include "System/Graphics.h"
#include"System/Input.h"
#include "SceneTutorial.h"
#include "Camera.h"
#include "../Game/EnemyManager.h"
#include "time.h"
#include "random"
#include "algorithm"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include "SceneGameproject.h"


// 初期化
void SceneTutorial::Initialize()
{
	//ステージ初期化
	stage = new Stage();

	//プレイヤー初期化
	player = new Player();

	//レティクル関数
	sprite = new Sprite("Data/Sprite/レティクル.png");
	sprite2 = new Sprite("Data/Sprite/チュートリアル問題.png");
	sprite3 = new Sprite("Data/Sprite/チュートリアル解答.png");

	//レティクル関数
	SceneGame::RoadModel();
	
	
	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),//視点
		DirectX::XMFLOAT3(0, 0, 0),//注視点
		DirectX::XMFLOAT3(0, 1, 0)//上方向
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),//視野角
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),//画面アスペクト比
		0.1f,//クリップ距離（近）
		1000.0f//クリップ距離（遠）
	);


	//カメラコントローラー初期化
	cameraController = new CameraController();
	player->cameraController = cameraController;

	//エネミー初期化
	int num = 0;
	int hei = 0;
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < 20; i++)
	{
		//EnemySlime* target = new EnemySlime();
		balloon = new Balloon();
		/*target->setobjnum(i);*/

		//Board* board = new Board();
		board = new Board();

		//Box* box = new Box();
		box = new Box();
		/*box->setobjnum(i);*/


		//エネミー位置
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float>dist(-4.0f, 4.0f);
		std::uniform_real_distribution<float>dist2(0.5f, 4.0f);
		std::uniform_real_distribution<float>dist3(5.0f, 10.0f);

		balloon->SetPosition(DirectX::XMFLOAT3(dist(gen), dist2(gen), dist3(gen)));
		balloon->SetAngle(DirectX::XMFLOAT3(0, DirectX::XM_PI, 0));


		board->SetPosition(DirectX::XMFLOAT3(0, 4.0f, 14));
		board->SetAngle(DirectX::XMFLOAT3(0, DirectX::XM_PI, 0));
		

		if (i % 5 == 0) { num = 0; hei++; }

		box->SetPosition(DirectX::XMFLOAT3(-4.6f + 2.3f * num, 1.45f * hei, 12));

		num++;

		enemyManager.Register(balloon);
		enemyManager.Register(board);
		enemyManager.Register(box);
		balloon->box = box;
	}

	

	//マウス位置の取得とロック
	Input::Instance().GetMouse().Lock();
	//Input::Instance().GetMouse();
}

// 終了化
void SceneTutorial::Finalize()
{
	Input::Instance().GetMouse().Unlock();

	//カメラコントローラー終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//ステージ終了化
	if (stage != nullptr)
	{
		delete stage;
		stage = nullptr;
	}

	//プレイヤー終了化
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	//エネミー終了化
	EnemyManager::Instance().Clear();
}

// 更新処理
void SceneTutorial::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	const GamePadButton anyButton =
		GamePad::BTN_START;
	

	switch (change)
	{
	case 0:

		if (gamePad.GetButtonDown() & anyButton)
		{
			change++;
		}

		break;

	case 1:
		//カメラコントローラー更新処理
		DirectX::XMFLOAT3 target = player->GetPosition();
		target.y += 0.5f;
		cameraController->SetTarget(target);
		cameraController->Update(elapsedTime);

		player->Update(elapsedTime);

		//ステージ更新処理
		stage->Update(elapsedTime);

		//プレイヤー更新処理

		//エネミー更新処理
		EnemyManager::Instance().Update(elapsedTime);

		if (gamePad.GetButtonDown() & anyButton)
		{
			change++;
		}

		break;

	case 2:

		//シーン遷移
		if (gamePad.GetButtonDown() & anyButton)
		{
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGameproject));
		}
		break;
	};
}

// 描画処理
void SceneTutorial::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ShapeRenderer* shapeRenderer = graphics.GetShapeRenderer();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();

	// 描画準備
	RenderContext rc;
	rc.deviceContext = dc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f };	// ライト方向（下方向）
	rc.renderState = graphics.GetRenderState();

	//描画処理
	//RenderContext rc;
	rc.renderState = graphics.GetRenderState();
	rc.lightDirection = { 0.0f , -1.0f , 0.0f };	//ライト方向（下方向）

	//カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 3Dモデル描画
	{
		//ステージ描画
		stage->Render(rc, modelRenderer);

		player->Render(rc, modelRenderer);

		EnemyManager::Instance().Render(rc, modelRenderer);

		player->RenderDebugPrimitive(rc, shapeRenderer);
	}

	// 3Dデバッグ描画
	{
		//プレイヤーデバッグプリミティブ描画
		//player->RenderDebugPrimitive(rc, shapeRenderer);

		//エネミーデバッグプリミティブ描画
		EnemyManager::Instance();
		//.RenderDebugPrimitive(rc, shapeRenderer);
	}

	// 2Dスプライト描画
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());

		if (change == 1)
		{
			sprite->Render(rc,
				610, 335, 0, 64.0f, 64.0f,
				0,
				1, 1, 1, 1);
		}

		if (change == 0)
		{
			sprite2->Render(rc,
				0, 0, 0, screenWidth, screenHeight,
				0,
				1, 1, 1, 1);
		}

		if (change == 2)
		{
			sprite3->Render(rc,
				0, 0, 0, screenWidth, screenHeight,
				0,
				1, 1, 1, 1);
		}

	}
}

// GUI描画
void SceneTutorial::DrawGUI()
{
	//プレーヤーデバッグ処理
	//player->DrawDebugGUI();
}
