#include "System/Graphics.h"
#include "SceneTitle.h"
#include "System/Input.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "System/Mouse.h"
#include "../Game/Player.h"
#include "../Scene/SceneTutorial.h"
#include "Game/PlayerManager.h"
#include "../Game/EnemyManager.h"
#include"Camera.h"

//初期化
void SceneTitle::Initialize()
{
	//スプライト初期化
	sprite = new Sprite("Data/Sprite/flow.png");
    sprite2 = new Sprite("Data/Sprite/start.png");
    sprite3 = new Sprite("Data/Sprite/tutorial.png");

    ShowCursor(true);
    //extern int count_1=0,count_2=0,count_3=0,count_4=0
    titlestage = std::make_unique<Stage>();
    titlestage->SetPosition({ 0.0f, -3.0f, 3.8f });
	player = std::make_unique<Player>();
	//model = std::make_unique<Model>("Data/Model/Player/player_robot.mdl");

    //カメラコントローラー初期化
    cameraController = new CameraController();
    //cameraController->angle.y = DirectX::XMConvertToRadians(45.0f);
    cameraController->angle.x = DirectX::XMConvertToRadians(78.0f);
    cameraController->distance = 29.8f;
    //player->cameraController = cameraController;
    DirectX::XMFLOAT3 target = titlestage->GetPosition();
    cameraController->SetTarget(target);

    PlayerManager::Instance().Register(player.get());
    PlayerManager::Instance().GetPlayer()->SetProv(true);
	PlayerManager::Instance().GetPlayer()->SetPosition({ 0.0f, -3.0f, 6.8f });

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



//終了化
void SceneTitle::Finalize()
{
	//スプライト終了化
    delete sprite;
    delete sprite2;
    delete sprite3;
    ShowCursor(false);
	
}

//更新処理
void SceneTitle::Update(float elapsedTime)
{
	//ステージ更新処理
    titlestage->Update(elapsedTime);
    //プレイヤー更新処理
    player->Update(elapsedTime);

    titlestage->SetPosition({ 0.0f, -3.0f, 3.8f });

    GetCursorPos(&cursorPos);
    HWND hwnd = GetForegroundWindow();
    ScreenToClient(hwnd, &cursorPos);

    Mouse& mouse = Input::Instance().GetMouse();

    //model->PlayAnimation(2, true);

    //左クリックで画面遷移
    if (GetAsyncKeyState('w') || GetAsyncKeyState(VK_UP))
    {
        state = 1;
    }

    if (GetAsyncKeyState('s') || GetAsyncKeyState(VK_DOWN))
    {
        state = 2;
	}

    if (GetAsyncKeyState(VK_RETURN))
    {
        if (state == 1 || state==0)
        {
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
        }
        if (state == 2)
        {
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTutorial));
        }
    }


    //スタート
    if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        if (cursorPos.x >= 505 && cursorPos.x <= 765)
        {
            if (cursorPos.y >= 520 && cursorPos.y <= 585)
            {
                SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
            }
        }
    }
    //チュートリアル
    if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        if (cursorPos.x >= 505 && cursorPos.x <= 765)
        {
            if (cursorPos.y >= 600 && cursorPos.y <= 670)
            {
                SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTutorial));
            }
        }
    }
}

//描画処理
void SceneTitle::Render()
{
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

    {
        //ステージ描画
        titlestage->UpdateTransform();
        titlestage->Render(rc, modelRenderer);
        player->Render(rc, modelRenderer);

        EnemyManager::Instance().Render(rc, modelRenderer);

        player->RenderDebugPrimitive(rc, shapeRenderer);

    }
    //2Dスプライト描画
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        sprite->Render(rc,
            0, -300, 0, screenWidth, screenHeight+200,
            0,
            1, 1, 1, 1);
            //スタートとチュートリアルの描画と拡大
        if (state == 0)
        {
            //拡大
            sprite2->Render(rc,
                60, 25, 0, 1200, 700,
                0,
                1, 1, 1, 1);
            sprite3->Render(rc,
                150, 100, 0, 1000, 600,
                0,
                1, 1, 1, 1);
        }
        
        else if (state == 1)
        {
            //拡大
            sprite2->Render(rc,
                60, 25, 0, 1200, 700,
                0,
                1, 1, 1, 1);
            sprite3->Render(rc,
                150, 100, 0, 1000, 600,
                0,
                1, 1, 1, 1);
        }
       
        else if (state == 2)
        {
            //拡大
            sprite3->Render(rc,
                60, 20, 0, 1200, 700,
                0,
                1, 1, 1, 1);
            sprite2->Render(rc,
                150, 100, 0, 1000, 600,
                0,
                1, 1, 1, 1);
        }
        else
        {
            sprite2->Render(rc,
                150, 100, 0, 1000, 600,
                0,
                1, 1, 1, 1);
            sprite3->Render(rc,
                150, 100, 0, 1000, 600,
                0,
                1, 1, 1, 1);
        }
        
        //if (cursorPos.x >= 505 && cursorPos.x <= 765)
        //{

        //    //スタート
        //    if (cursorPos.y >= 520 && cursorPos.y <= 585 )
        //    {
        //        //拡大
        //        sprite2->Render(rc,
        //            60, 25, 0, 1200, 700,
        //            0,
        //            1, 1, 1, 1);
        //    }
        //    else
        //    {
        //        sprite2->Render(rc,
        //            150, 100, 0, 1000, 600,
        //            0,
        //            1, 1, 1, 1);
        //    }

        //    //チュートリアル
        //    if (cursorPos.y >= 600 && cursorPos.y <= 670 )
        //    {
        //        //拡大
        //        sprite3->Render(rc,
        //            60, 20, 0, 1200, 700,
        //            0,
        //            1, 1, 1, 1);
        //    }
        //    else
        //    {
        //        sprite3->Render(rc,
        //            150, 100, 0, 1000, 600,
        //            0,
        //            1, 1, 1, 1);
        //    }
        //}
        //else
        //{
        //    //通常時の描画
        //    sprite2->Render(rc,
        //        150, 100, 0, 1000, 600,
        //        0,
        //        1, 1, 1, 1);

        //    sprite3->Render(rc,
        //        150, 100, 0, 1000, 600,
        //        0,
        //        1, 1, 1, 1);
        //}


    }
   
}

//GUI描画
void SceneTitle::DrawGUI()
{

}