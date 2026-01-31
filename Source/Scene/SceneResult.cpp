#include "System/Graphics.h"
#include "SceneResult.h"
#include "System/Input.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include <imgui.h>
#include "SceneTitle.h"

//初期化
void SceneResult::Initialize()
{
	beforeScore = Grid::Instance().GetScore();
	if (Grid::Instance().bestScore < beforeScore)
	{
		Grid::Instance().bestScore = beforeScore;
	}

	//スプライト初期化
	sprite = std::make_unique<Sprite>("Data/Sprite/result.png");
	sprite_number = std::make_unique<Sprite>("Data/Sprite/number.png");
}

//終了化
void SceneResult::Finalize()
{
	ShowCursor(true);
	Grid::Instance().SetScore(0);
}

//更新処理
void SceneResult::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//なにかボタンを押したらゲームシーンへ切り替え
	const GamePadButton anyButton =
		GamePad::BTN_START;

	if (gamePad.GetButtonDown() & anyButton)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}
}

//描画処理
void SceneResult::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderState = graphics.GetRenderState();

	//描画準備
	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = graphics.GetRenderState();

	//2Dスプライト描画
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		sprite->Render(rc,
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

			float startX = 1100.0f;
			float startY = 10.0f;

			for (int i = 0; i < digitCount; i++)
			{
				int num = digits[i];
				sprite_number->Render(rc,
					startX - (24 * 2 * i),
					startY,
					0,
					32 * 2, 32 * 2,
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

			float startX = 100.0f;
			float startY = 10.0f;

			for (int i = 0; i < digitCount; i++)
			{
				int num = digits[i];
				sprite_number->Render(rc,
					startX - (24 * 2 * i),
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
}

//GUI描画
void SceneResult::DrawGUI()
{
	ImGui::Begin("Score");
	ImGui::Text("GetScore : %d", Grid::Instance().GetScore());
	ImGui::Text("beforeScore : %d", beforeScore);
	ImGui::Text("bestScore : %d", Grid::Instance().bestScore);
	ImGui::End();
}
