#include "System/Graphics.h"
#include "SceneResult.h"
#include "System/Input.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include <imgui.h>
#include "SceneTitle.h"
#include"Camera.h"

//初期化
void SceneResult::Initialize()
{
	beforeScore = Grid::Instance().GetScore();
	if (Grid::Instance().bestScore < beforeScore)
	{
		Grid::Instance().bestScore = beforeScore;
	}

	//スプライト初期化
	sprite = std::make_unique<Sprite>("Data/Sprite/bestScore.png");
	sprite_number = std::make_unique<Sprite>("Data/Sprite/number.png");
	sprite_txt = std::make_unique<Sprite>("Data/Sprite/backTitle.png");
	//プレイヤー初期化
	player = std::make_unique<Player2>();
	player2 = std::make_unique<Player2>();

	player->SetPosition({ -8.0f, -12.0f, 6.8f });
	player2->SetPosition({ 8.0f, -12.0f, 6.8f });
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0.5f, 19, 30),//視点
		DirectX::XMFLOAT3(0, 0, 0),//注視点
		DirectX::XMFLOAT3(0, 1, 0)//上方向
	);
	//BGM
	SoundManager::Instance().GetSound(SoundList::resultOverBGM)->Play(true, 0.02f);
}

//終了化
void SceneResult::Finalize()
{
	ShowCursor(true);
	Grid::Instance().SetScore(0);
	//BGM
	SoundManager::Instance().GetSound(SoundList::resultOverBGM)->Stop();
}

//更新処理
void SceneResult::Update(float elapsedTime)
{
	player->Update(elapsedTime);
	player2->Update(elapsedTime);

	player->SetPosition({ -9.0f, playerY, 6.8f });
	player2->SetPosition({ 9.0f,playerY, 6.8f });

	GamePad& gamePad = Input::Instance().GetGamePad();

	//なにかボタンを押したらゲームシーンへ切り替え
	const GamePadButton anyButton =
		GamePad::BTN_START;

	if (gamePad.GetButtonDown() & anyButton)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}

	UpdateCursorToggle();
}

//描画処理
void SceneResult::Render()
{
	//Graphics& graphics = Graphics::Instance();
	//ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	//RenderState* renderState = graphics.GetRenderState();
	//ModelRenderer* modelRenderer = graphics.GetModelRenderer();

	////描画準備
	//RenderContext rc;
	//rc.deviceContext = dc;
	//rc.renderState = graphics.GetRenderState();

	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderState = graphics.GetRenderState();

	//描画準備
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();

	ShapeRenderer* shapeRenderer = graphics.GetShapeRenderer();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();

	//カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 描画準備
	rc.deviceContext = dc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f };	// ライト方向（下方向）
	rc.renderState = graphics.GetRenderState();

	//2Dスプライト描画
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		sprite->Render(rc,
			0, 0, 0, screenWidth, screenHeight,
			0,
			1, 1, 1, 1);
		sprite_txt->Render(rc,
			0, 0, 0, screenWidth, screenHeight,
			0,
			1, 1, 1, 1);

		//今回のスコアの表示
		{
			int temp = beforeScore;
			int digits[10];
			int digitCount = 0;

			if (temp == 0)
			{
				digits[digitCount++] = 0;
			}
			else
			{
				while (temp > 0)
				{
					digits[digitCount++] = temp % 10;
					temp /= 10;
				}
			}


			float startX = numberPosX;
			float startY = numberPosY;

			const float digitW = 32.0f * 2.0f;

			const float spacing = digitW * 0.85f;
			float totalWidth = digitCount * spacing;
			float startDrawX = startX - totalWidth * 0.5f;

			for (int i = 0; i < digitCount; i++)
			{
				//int num = digits[i];
				int num = digits[digitCount - 1 - i];
				sprite_number->Render(rc,
					startDrawX + spacing * i,  // ← ここ重要
					startY,
					0,
					digitW, digitW,
					372.5f * num, 0,
					372.5f, 514,
					0,
					1, 1, 1, 1);
			}
		}

		//最高スコアの表示
		{
			int temp = Grid::Instance().bestScore;
			int digits[10];
			int digitCount = 0;

			if (temp == 0)
			{
				digits[digitCount++] = 0;
			}
			else
			{
				while (temp > 0)
				{
					digits[digitCount++] = temp % 10;
					temp /= 10;
				}
			}

			float startX = NumberPosX;
			float startY = NumberPosY;

			const float digitW = 32.0f * 2.0f;

			const float spacing = digitW * 0.85f;
			float totalWidth = digitCount * spacing;
			float startDrawX = startX - totalWidth * 0.5f;

			for (int i = 0; i < digitCount; i++)
			{
				int num = digits[digitCount - 1 - i];
				sprite_number->Render(rc,
					startDrawX + spacing * i,
					startY,
					0,
					32 * 2, 32 * 2,
					372.5f * num, 0,
					372.5f, 514,
					0,
					1, 1, 1, 1);
			}
		}
	}

	player->Render(rc, modelRenderer);
	player2->Render(rc, modelRenderer);
}

//GUI描画
void SceneResult::DrawGUI()
{
	ImGui::Begin("Score");
	ImGui::Text("GetScore : %d", Grid::Instance().GetScore());
	ImGui::Text("beforeScore : %d", beforeScore);
	ImGui::Text("bestScore : %d", Grid::Instance().bestScore);

	ImGui::Begin("Number Position");
	ImGui::DragFloat2("Pos", &numberPosX, 1.0f);
	ImGui::End();
	ImGui::Begin("Number Position best");
	ImGui::DragFloat2("Pos", &NumberPosX, 1.0f);
	ImGui::End();

	ImGui::Begin("Player Y Control");
	ImGui::DragFloat("Y Position", &playerY, 0.1f, -50.0f, 50.0f, "%.2f");
	ImGui::End();
	ImGui::End();
}

void SceneResult::UpdateCursorToggle()
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
