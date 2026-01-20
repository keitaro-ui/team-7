#include "System/Graphics.h"
#include"System/Input.h"
#include "SceneGameproject.h"
#include "Camera.h"
#include "../Game/EnemyManager.h"
#include "time.h"
#include "random"
#include "algorithm"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"

//float game_timer;

// 初期化
void SceneGameproject::Initialize()
{
	//ステージ初期化
	stage = new Stage();
	/*Board* board {};
	board-> CreateModel();*/

	//時間
	game_timer = 30;

	//プレイヤー初期化
	player = new Player();

	//レティクル関数
	sprite = new Sprite("Data/Sprite/レティクル.png");
	sprite_number = new Sprite("Data/Sprite/number.png");
	sprite_text = new Sprite("Data/Sprite/残り時間.png");

	//レティクル関数
	//SceneGame::RoadModel();

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
		/*target->setobjnum(i);*/

		//Board* board = new Board();

		//Box* box = new Box();
		box = new Box();
		/*box->setobjnum(i);*/
		

		//エネミー位置
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float>dist (-4.0f, 4.0f);
		std::uniform_real_distribution<float>dist2(0.5f, 4.0f);
		std::uniform_real_distribution<float>dist3(5.0f, 10.0f);

		if (i % 5 == 0) { num = 0; hei++; }
		
		box->SetPosition(DirectX::XMFLOAT3(-4.6f+2.3f*num, 1.45f*hei, 12));

		num++;
		
		enemyManager.Register(box);
	}

	//マウス位置の取得とロック
	Input::Instance().GetMouse().Lock();
}

// 終了化
void SceneGameproject::Finalize()
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
void SceneGameproject::Update(float elapsedTime)
{

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

	//シーン遷移
	GamePad& gamePad = Input::Instance().GetGamePad();

	const GamePadButton anyButton =
		GamePad::BTN_START;

	game_timer -= elapsedTime;

	//if (gamePad.GetButtonDown() & anyButton)
	if(game_timer < 0)
	{
		//SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
		player->finish = true;
	}

	if (gamePad.GetButtonDown() & anyButton&&player->finish==true)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
	}
}

// 描画処理
void SceneGameproject::Render()
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
		sprite->Render(rc,
			610, 335, 0, 64.0f, 64.0f,
			0,
			1, 1, 1, 1);

		sprite_text->Render(rc,
			950, 0, 0, 120, 80, 0, 1, 1, 1, 1);

		int n[2]{};
		//n[0] = static_cast<int>(game_timer) / 100 % 10;
		n[0] = static_cast<int>(game_timer) / 10 % 10;
		n[1] = static_cast<int>(game_timer) % 10;

		for (int i = 0; i < 2; i++)
		{
			sprite_number->Render(rc,
				32 * 2 * i + 1100, 10,
				0,
				32 * 2, 32 * 2,
				372.5 * n[i], 0,
				372.5, 514,
				0,
				1, 1, 1, 1);
		}
	}
}

// GUI描画
void SceneGameproject::DrawGUI()
{
	//プレーヤーデバッグ処理
	player->DrawDebugGUI();
}
