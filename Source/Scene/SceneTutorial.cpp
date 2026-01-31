#include "System/Graphics.h"
#include"System/Input.h"
#include "SceneTutorial.h"
#include "Camera.h"
#include "../Game/EnemyManager.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"


// 初期化
void SceneTutorial::Initialize()
{
	spr = std::make_unique<Sprite>("Data/Sprite/tutoSprite.png");

	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 7, 30),//視点
		DirectX::XMFLOAT3(0, 0, 0),//注視点
		DirectX::XMFLOAT3(0, 1, 0)//上方向
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),//視野角
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),//画面アスペクト比
		0.1f,//クリップ距離（近）
		1000.0f//クリップ距離（遠）
	);
}

// 終了化
void SceneTutorial::Finalize()
{

}

// 更新処理
void SceneTutorial::Update(float elapsedTime)
{
	if (GetAsyncKeyState(' ') & 0x8000)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
	}
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

	//カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	spr->Render(rc,
		0, 0, 0,
		1280, 720,
		0,
		1, 1, 1, 1);
}

// GUI描画
void SceneTutorial::DrawGUI()
{
	//プレーヤーデバッグ処理
	//player->DrawDebugGUI();
}
