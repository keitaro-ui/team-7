#include "System/Graphics.h"
#include "System/Input.h"
#include "SceneGame.h"
#include "Camera.h"
#include "../Game/EnemyManager.h"
#include "time.h"
#include "random"
#include "algorithm"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneLoading.h"
#include <imgui.h>

//float game_timer;

// 初期化
void SceneGame::Initialize()
{
	//時間
	game_timer = 0;

	//ステージ初期化
	stage = std::make_unique<Stage>();

	//プレイヤー初期化
	player = std::make_unique<Player>();

	//レティクル関数
	sprite = new Sprite("Data/Sprite/レティクル.png");
	sprite_number = new Sprite("Data/Sprite/number.png");
	sprite_text = new Sprite("Data/Sprite/残り時間.png");

	//BOXモデル読み込み
	for (int i = 0; i < 11; i++)
	{
		boxes[i] = std::make_unique<Box>(i);
	}
	//boxes[0] = std::make_unique<Box>(0);

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

	//箱の初期位置をランダムで決定
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int>distX (0, GRID_MAX - 1);
	std::uniform_int_distribution<int>distZ(0, GRID_MAX - 1);

	map[distX(gen)][distZ(gen)] = 1;
	map[distZ(gen)][distX(gen)] = 1;
	
	//debug
	{
		/*map[0][0] = 1;
		map[0][1] = 1;
		map[0][2] = 1;
		map[0][3] = 1;*/
	}
}

// 終了化
void SceneGame::Finalize()
{
	/*Input::Instance().GetMouse().Unlock();*/

	//カメラコントローラー終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	delete sprite;
	delete sprite_number;
	delete sprite_text;

	//エネミー終了化
	EnemyManager::Instance().Clear();
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	//カメラコントローラー更新処理
	/*DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);*/
	cameraController->Update(elapsedTime);

	//ステージ更新処理
	stage->Update(elapsedTime);

	//プレイヤー更新処理
	player->Update(elapsedTime);

	//エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);

	//
	UpdateCursorToggle();

	//シーン遷移
	GamePad& gamePad = Input::Instance().GetGamePad();

	const GamePadButton anyButton = GamePad::BTN_START;

	game_timer += elapsedTime;

	//方向キーでBox動かす関数
	if (game_timer > coolTime)
	{
		if ((pushUp() || pushDown() || pushLeft() || pushRight()) )
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int>distX(0, GRID_MAX - 1);
			std::uniform_int_distribution<int>distY(0, GRID_MAX - 1);
			while (true)
			{
				int x = distX(gen);
				int	y = distY(gen);

				if (map[y][x] == 0)
				{
					//2の箱だす
					map[y][x] = 1; 
					game_timer = 0.0f;
					up = false;
					break;
				}
			}
			//合体済みフラグのリセット
			std::memset(merged, false, sizeof(merged));
		}
	}

	if(game_timer < 0)
	{
		//SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
		player->finish = true;
	}

	if (gamePad.GetButtonDown() & anyButton&&player->finish==true)
	{
		//SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
	}
}

// 描画処理
void SceneGame::Render()
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
		stage->SetPosition({ 0.0f, -3.0f, 5.0f });
		stage->UpdateTransform();
		stage->Render(rc, modelRenderer);

		player->Render(rc, modelRenderer);

		EnemyManager::Instance().Render(rc, modelRenderer);

		player->RenderDebugPrimitive(rc, shapeRenderer);

		// box
		for (int y = 0; y < GRID_MAX; y++)
		{
			for (int x = 0; x < GRID_MAX; x++)
			{
				int v = map[y][x];

				//空のマスはスキップ
				if (v == 0) continue;

				// map=1 -> models[0]
				int modelIndex = v - 1; 

				// model存在するかチェック
				if (!boxes[modelIndex]) continue;

				//map->world座標に変換してpos決定
				DirectX::XMFLOAT3 pos = 
				{ startPos.x + x * tileSize, startPos.y, -(startPos.z + y * tileSize) };
				boxes[modelIndex]->SetPosition(pos);
				boxes[modelIndex]->SetScale({ 1.0f, 1.0f, 1.0f });
				boxes[modelIndex]->UpdateTransform();
				boxes[modelIndex]->Render(rc, modelRenderer);
			}
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

			//int n[2]{};
			//n[0] = static_cast<int>(game_timer) / 100 % 10;
			//n[0] = static_cast<int>(game_timer) / 10 % 10;
			//n[1] = static_cast<int>(game_timer) % 10;

			/*for (int i = 0; i < 2; i++)
			{
				sprite_number->Render(rc,
					32 * 2 * i + 1100, 10,
					0,
					32 * 2, 32 * 2,
					372.5 * n[i], 0,
					372.5, 514,
					0,
					1, 1, 1, 1);
			}*/
		}
	}
}

//GUI描画
void SceneGame::DrawGUI()
{
	//プレーヤーデバッグ処理
	player->DrawDebugGUI();

	// ImGui
	ImGui::Begin("Map Settings");

	// mapの1マスの間隔
	ImGui::DragFloat(
		"Tile Size",
		&tileSize,
		0.1f,   // 変化量
		0.1f,   // 最小
		10.0f   // 最大
	);

	// map[0][0]の位置
	ImGui::DragFloat3(
		"Start Position",
		&startPos.x,
		0.1f    // 変化量
	);

	ImGui::End();

}

//方向キーでBox動かす関数
bool SceneGame::pushUp()
{
	moved = false;

	if (GetAsyncKeyState('I') & 0x8000)
	{
		for (int x = 0; x < GRID_MAX; x++)
		{
			for (int y = 1; y < GRID_MAX; y++)
			{
				if (map[y][x] != 0)
				{
					int cy = y;
					while (cy > 0)
					{
						//一個上が0なら移動させる
						if (map[cy - 1][x] == 0)
						{
							map[cy - 1][x] = map[cy][x];
							map[cy][x] = 0;
							cy--;
							moved = true;
						}
						//一個上と一緒なら合体
						else if (map[cy - 1][x] == map[cy][x]
							&& !merged[cy - 1][x])
						{
							map[cy - 1][x]++;
							map[cy][x] = 0;
							merged[cy - 1][x] = true;
							moved = true;
							break;
						}
						// 一個上が0じゃなくて、違う数字ならそのまま
						else break;
					}
				}
			}
		}
	}
	return moved;
}

bool SceneGame::pushDown()
{
	moved = false;

	if (GetAsyncKeyState('K') & 0x8000)
	{
		for (int x = 0; x < GRID_MAX; x++)
		{
			for (int y = GRID_MAX - 2; y >= 0; y--)
			{
				if (map[y][x] != 0)
				{
					int cy = y;
					while (cy < GRID_MAX - 1)
					{
						//一個下が0なら移動させる
						if (map[cy + 1][x] == 0)
						{
							map[cy + 1][x] = map[cy][x];
							map[cy][x] = 0;
							cy++;
							moved = true;
						}
						//一個下と一緒なら合体
						else if (map[cy + 1][x] == map[cy][x]
							&& !merged[cy + 1][x])
						{
							map[cy + 1][x]++;
							map[cy][x] = 0;
							merged[cy + 1][x] = true;
							moved = true;
							break;
						}
						// 一個下が0じゃなくて、違う数字ならそのまま
						else break;
					}
				}
			}
		}
	}
	return moved;
}

bool SceneGame::pushLeft()
{
	moved = false;

	if (GetAsyncKeyState('J') & 0x8000)
	{
		for (int y = 0; y < GRID_MAX; y++)
		{
			for (int x = 1; x < GRID_MAX; x++)
			{
				if (map[y][x] != 0)
				{
					int cx = x;
					while (cx > 0)
					{
						//一個左が0なら移動させる
						if (map[y][cx - 1] == 0)
						{
							map[y][cx - 1] = map[y][cx];
							map[y][cx] = 0;
							cx--;
							moved = true;
						}
						//一個左と一緒なら合体
						else if (map[y][cx - 1] == map[y][cx]
							&& !merged[y][cx - 1])
						{
							map[y][cx - 1]++;
							map[y][cx] = 0;
							merged[y][cx - 1] = true;
							moved = true;
							break;
						}
						// 一個左が0じゃなくて、違う数字ならそのまま
						else break;
					}
				}
			}
		}
	}
	return moved;
}

bool SceneGame::pushRight()
{
	moved = false;

	if (GetAsyncKeyState('L') & 0x8000)
	{
		for (int y = 0; y < GRID_MAX; y++)
		{
			for (int x = GRID_MAX - 2; x >= 0; x--)
			{
				if (map[y][x] != 0)
				{
					int cx = x;
					while (cx < GRID_MAX - 1)
					{
						//一個右が0なら移動させる
						if (map[y][cx + 1] == 0)
						{
							map[y][cx + 1] = map[y][cx];
							map[y][cx] = 0;
							cx++;
							moved = true;
						}
						//一個右と一緒なら合体
						else if (map[y][cx + 1] == map[y][cx]
							&& !merged[y][cx + 1])
						{
							map[y][cx + 1]++;
							map[y][cx] = 0;
							merged[y][cx + 1] = true;
							moved = true;
							break;
						}
						// 一個右が0じゃなくて、違う数字ならそのまま
						else break;
					}
				}
			}
		}
	}
	return moved;
}

void SceneGame::UpdateCursorToggle()
{
	static bool cursorVisible = false;
	static bool prevRight = false;

	bool nowRight = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;

	// 押した瞬間だけ判定
	if (nowRight && !prevRight)
	{
		cursorVisible = !cursorVisible;
		ShowCursor(cursorVisible ? TRUE : FALSE);
	}

	prevRight = nowRight;
}
