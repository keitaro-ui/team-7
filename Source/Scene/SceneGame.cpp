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
#include"../Game/PlayerManager.h"
#include "../Game/GridManager.h"
#include <imgui.h>

// 初期化
void SceneGame::Initialize()
{
	//時間
	game_timer = 0;

	//ステージ初期化
	stage = std::make_unique<Stage>();
	stage->SetPosition({ 0.0f, -3.0f, 3.8f });

	//プレイヤー初期化
	player = std::make_unique<Player>();

	PlayerManager::Instance().Register(player.get());
	//GridManager::Instance().Register(grid.)
	
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

	//カメラコントローラー初期化
	cameraController = new CameraController();
	//cameraController->angle.y = DirectX::XMConvertToRadians(45.0f);
	cameraController->angle.x = DirectX::XMConvertToRadians(78.0f);
	cameraController->distance = 29.8f;
	player->cameraController = cameraController;
	DirectX::XMFLOAT3 target = stage->GetPosition();
	cameraController->SetTarget(target);

	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 15, -5),//視点
		DirectX::XMFLOAT3(0, 0, 0),//注視点
		DirectX::XMFLOAT3(0, 1, 0)//上方向
		);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),//視野角
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),//画面アスペクト比
		0.1f,//クリップ距離（近）
		1000.0f//クリップ距離（遠）
	);

	//game情報を初期化し、ランダム二か所に描画
	tileSize = 4.45f;
	startPos = { -8.9f, 0.0f, -12.4f };

	std::memset(grid.map, 0, sizeof(grid.map));
	std::memset(grid.merged, false, sizeof(grid.merged));
	grid.moved = false;

	grid.Spawn();
	grid.Spawn();

	//debug
	{
		/*map[0][0] = 1;
		map[0][1] = 1;
		map[0][2] = 1;
		map[0][3] = 1;*/
	}

	//BGM
	//SoundManager::Instance().GetSound(SoundList::gameBGM)->Play(true, 0.02f);
}

// 終了化
void SceneGame::Finalize()
{
	//BGM
	//SoundManager::Instance().GetSound(SoundList::gameBGM)->Stop();

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
	cameraController->Update(elapsedTime);
	
	//ゲームタイマー更新処理
	game_timer += elapsedTime;

	//ステージ更新処理
	stage->Update(elapsedTime);

	//プレイヤー更新処理
	player->Update(elapsedTime);

	//エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);

	//カーソル出すやつ
	UpdateCursorToggle();

	//シーン遷移
	GamePad& gamePad = Input::Instance().GetGamePad();
	const GamePadButton anyButton = GamePad::BTN_START;


	//grid.CanSlideW();

	//方向キーでBox動かす関数
	if (game_timer > coolTime)
	{
		if (GetAsyncKeyState('I') & 0x8000) grid.moved = grid.MoveUp();
		if (GetAsyncKeyState('K') & 0x8000) grid.moved = grid.MoveDown();
		if (GetAsyncKeyState('J') & 0x8000) grid.moved = grid.MoveLeft();
		if (GetAsyncKeyState('L') & 0x8000) grid.moved = grid.MoveRight();

		if (grid.moved)
		{
			grid.Spawn();
			game_timer = 0.0f;
			grid.moved = false;
		}
	}

	//ゲームオーバー処理
	if (grid.IsGameOver())
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
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
		stage->UpdateTransform();
		stage->Render(rc, modelRenderer);

		player->Render(rc, modelRenderer);

		EnemyManager::Instance().Render(rc, modelRenderer);

		player->RenderDebugPrimitive(rc, shapeRenderer);

		// box
		if (isMoving)
		{


		}
		else
		{
			for (int y = 0; y < grid.GRID_MAX; y++)
			{
				for (int x = 0; x < grid.GRID_MAX; x++)
				{
					int v = grid.map[y][x];
					if (v == 0) continue;

					int modelIndex = v - 1;
					if (modelIndex < 0 || modelIndex >= 11) continue;

					DirectX::XMFLOAT3 pos =
					{
						startPos.x + x * tileSize,
						startPos.y,
						-(startPos.z + y * tileSize)
					};

					boxes[modelIndex]->SetPosition(pos);
					boxes[modelIndex]->UpdateTransform();
					boxes[modelIndex]->Render(rc, modelRenderer);
				}
			}
		}


		// 3Dデバッグ描画
		{
			//プレイヤーデバッグプリミティブ描画
			//player->RenderDebugPrimitive(rc, shapeRenderer);

			//エネミーデバッグプリミティブ描画
			//EnemyManager::Instance()
			//.RenderDebugPrimitive(rc, shapeRenderer);
		}

		// 2Dスプライト描画
		{
			/*sprite->Render(rc,
				610, 335, 0, 64.0f, 64.0f,
				0,
				1, 1, 1, 1);

			sprite_text->Render(rc,
				950, 0, 0, 120, 80, 0, 1, 1, 1, 1);*/

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

	ImGui::Begin("Camera");

	float yaw = DirectX::XMConvertToDegrees(cameraController->angle.y);
	float pitch = DirectX::XMConvertToDegrees(cameraController->angle.x);

	if (ImGui::SliderFloat("Yaw", &yaw, -180, 180))
		cameraController->angle.y = DirectX::XMConvertToRadians(yaw);

	if (ImGui::SliderFloat("Pitch", &pitch, -360, 360))
		cameraController->angle.x = DirectX::XMConvertToRadians(pitch);

	ImGui::SliderFloat(
		"Distance",
		&cameraController->distance,
		5.0f,
		40.0f
	);

	ImGui::End();


	//マス目に関するgui
	{
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

		//ImGui::Text("player ptr : %p", player);
		ImGui::Text("X %d", PlayerManager::Instance().GetPlayer()->GetPlayerX());

		ImGui::End();
	}

	//数字を自由に変更する
	{
		ImGui::Begin("Map Editor");

		// ===== 設定 =====
		const float cellSize = 50.0f;
		ImVec2 origin = ImGui::GetCursorScreenPos();
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		// ===== グリッド描画 =====
		for (int y = 0; y < grid.GRID_MAX; y++)
		{
			for (int x = 0; x < grid.GRID_MAX; x++)
			{
				ImVec2 pMin = {
					origin.x + x * cellSize,
					origin.y + y * cellSize
				};
				ImVec2 pMax = {
					pMin.x + cellSize,
					pMin.y + cellSize
				};

				int v = grid.map[y][x];

				// 背景色
				ImU32 bgColor = (v == 0)
					? IM_COL32(60, 60, 60, 255)
					: IM_COL32(180, 160, 120, 255);

				// マス描画
				drawList->AddRectFilled(pMin, pMax, bgColor, 6.0f);
				drawList->AddRect(pMin, pMax, IM_COL32(255, 255, 255, 255));

				// 数字描画
				if (v > 0)
				{
					std::string text = std::to_string(1 << v);
					ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

					ImVec2 textPos = {
						pMin.x + (cellSize - textSize.x) * 0.5f,
						pMin.y + (cellSize - textSize.y) * 0.5f
					};

					drawList->AddText(textPos, IM_COL32(0, 0, 0, 255), text.c_str());
				}
			}
		}

		// グリッド分カーソルを進める
		ImGui::Dummy(ImVec2(cellSize * grid.GRID_MAX, cellSize * grid.GRID_MAX));

		ImGui::Separator();
		ImGui::Text("Edit Values");

		// ===== 数値編集 =====
		for (int y = 0; y < grid.GRID_MAX; y++)
		{
			for (int x = 0; x < grid.GRID_MAX; x++)
			{
				ImGui::PushID(y * grid.GRID_MAX + x);
				ImGui::SetNextItemWidth(40.0f);
				ImGui::InputInt("", &grid.map[y][x]);
				ImGui::PopID();

				if (x < grid.GRID_MAX - 1)
					ImGui::SameLine();
			}
		}

		ImGui::End();
	}

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
