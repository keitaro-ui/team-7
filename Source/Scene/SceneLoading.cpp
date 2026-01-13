#include "System/Graphics.h"
#include "System/Input.h"
#include "SceneManager.h"
#include "SceneLoading.h"

void SceneLoading::Initialize()
{
	//スプライト初期化
	sprite = new Sprite("Data/Sprite/レティクル.png");

	//スレッド開始
	thread = new std::thread(LoadingThread, this);
}

void SceneLoading::Finalize()
{
	//スプライト終了化
	if (sprite != nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}

	//スレッド終了化
	if (thread != nullptr)
	{
		thread->join();
		delete thread;
		thread = nullptr;
	}
}

void SceneLoading::Update(float elapsedTime)
{
	constexpr float speed = 180;
	angle += speed * elapsedTime;

	//次のシーンが完了したらシーンを切り替える
	if (nextScene->IsReady())
	{
		SceneManager::Instance().ChangeScene(nextScene);
		nextScene = nullptr;
	}
}

void SceneLoading::Render()
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
		//画面右下にローディングアイコン描画
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float spriteWidth = 256;
		float spriteHeight = 256;
		float positionX = screenWidth - spriteWidth;
		float positionY = screenHeight - spriteHeight;

		sprite->Render(rc,
			positionX, positionY, 0, spriteWidth, spriteHeight,
			angle,
			1, 1, 1, 1);
	}
}

void SceneLoading::DrawGUI()
{

}

//ローディングスレッド
void SceneLoading::LoadingThread(SceneLoading* scene)
{
	//COM関連の初期化でスレッド毎に呼ぶ必要がある
	CoInitialize(nullptr);

	//次のシーンの初期化を行う
	scene->nextScene->Initialize();

	//スレッドが終わる前にCOM関連の終了化
	CoUninitialize();

	//次のシーンの準備完了設定
	scene->nextScene->SetReady();
}
