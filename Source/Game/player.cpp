#include"Player.h"
#include"System/Input.h"
#include<imgui.h>
#include "Camera.h"

#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "ProjectHoming.h"
#include "../System/Graphics.h"
#include "Camera.h"
#include "System/Audio.h"
#include "random"
#include "GridManager.h"
#include "Grid.h"
//#include "PlayerManager.h"

int answer = -1, count_1, count_2, count_3, count_4;

//コンストラクタ
Player::Player()
{
	model = new Model("Data/Model/Slime/Slime.mdl");

	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;
	
	angle = { 0,0,0 };
}

//デストラクタ
Player::~Player()
{
	delete model;
}

//マウス操作用の変数
//POINT cursorPos;
//DirectX::XMFLOAT3 ndc = {};

//更新処理
void Player::Update(float elapsedTime)
{
	//Mouse& mouse = Input::Instance().GetMouse();
	////移動入力処理
	InputMove(elapsedTime);

	////ジャンプ入力処理
	//InputJump();

	//angle.x = -cameraController->getAngle().x;
	//angle.y = cameraController->getAngle().y - DirectX::XM_PIDIV2;


	//速力処理更新
	UpdateVelocity(elapsedTime);

	//プレイヤーとエネミーとの衝突処理
	CollisionPlayerVsEnemies();

	//オブジェクト行列を更新
	UpdateTransform();
	model->UpdateTransform();

	game_timer += elapsedTime;


	//mouse.Update();
}

//移動入力処理
void Player::InputMove(float elapsedTime)
{
	//進行ベクトル取得
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	float speed = 2.9f;


	/*DirectX::XMVECTOR v = DirectX::XMLoadFloat3(&moveVec);
	if (DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(v)) > 0.0f)
		v = DirectX::XMVector3Normalize(v);
	DirectX::XMStoreFloat3(&moveVec, v);*/

	DirectX::XMFLOAT3 pos =
	{
		startPos.x + playerX * tileSize,
		startPos.y,
		-(startPos.z + playerY * tileSize)  // Z反転はあなたの座標系用
	};
	//map[mapy][mapx]=map[MAP_H][MAP_W];

	MoveGrid();
	position=pos;

	//scale.x=scale.y=scale.z= 1.0f,1.0f,1.0f ;
	//model->UpdateTransform();


	//旋回処理
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

}

// 移動処理
void Player::MoveD(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;
	position.x += vx * speed;
	position.z += vz * speed;
}

//デバッグ用GUI描画
void Player::DrawDebugGUI()
{
	if (ImGui::Begin("Player"))
	{
		// トランスフォーム
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// 位置
			ImGui::DragFloat3(
				"Position",
				&position.x,
				0.01f    // 移動量
			);

			// 回転（ラジアン ⇔ 度）
			static DirectX::XMFLOAT3 angleDeg = { 0,0,0 };
			angleDeg.x = DirectX::XMConvertToDegrees(angle.x);
			angleDeg.y = DirectX::XMConvertToDegrees(angle.y);
			angleDeg.z = DirectX::XMConvertToDegrees(angle.z);

			if (ImGui::DragFloat(
				"Rotation",
				&angle.y,
				0.5f     // 回転量（度）
			))
			{
				/*angle.x = DirectX::XMConvertToRadians(angleDeg.x);
				angle.y = DirectX::XMConvertToRadians(angleDeg.y);
				angle.z = DirectX::XMConvertToRadians(angleDeg.z);*/
			}

			// スケール
			ImGui::DragFloat3(
				"Scale",
				&scale.x,
				0.01f,
				0.001f,  // 最小値（0禁止）
				100.0f
			);

			// 等方スケール（便利）
			static float uniformScale = 1.0f;
			if (ImGui::DragFloat(
				"Uniform Scale",
				&uniformScale,
				0.01f,
				0.001f,
				100.0f
			))
			{
				scale.x = scale.y = scale.z = uniformScale;
			}

			ImGui::Text("playerX : %d", playerX);
			ImGui::Text("playerY : %d", playerY);
		}
	}
	ImGui::End();

}

//描画処理
void Player::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}

//デバッグプリミティブ描画
void Player::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	//プレイヤーデバッグプリミティブ描画
	//Character::RenderDebugPrimitive(rc, renderer);

	//弾丸デバッグプリミティブ描画
	//projectileManager.RenderDebugPrimitive(rc, renderer);
}

//スティック入力値から移動ベクトルを取得
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	//入力情報を取得
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	//カメラ方向とスティックの入力値によって進行方向を計算する
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	//移動ベクトルはXZ平面に水平なベクトルになるようにする

	//カメラ右方向ベクトルをXZ平面ベクトルに変換
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);

	if (cameraRightLength > 0.0f)
	{
		//単位ベクトル化
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	//カメラ前方向をXZ単位ベクトルに変換
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);

	if (cameraFrontLength > 0.0f)
	{
		//単位ベクトル化
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	//スティックの水平入力値をカメラ右方向に反映し、
	//スティックの垂直入力値をカメラ前方向に反映し、
	//進行ベクトルを計算する
	DirectX::XMFLOAT3 vec;
	vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
	vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
	//Y軸方向には移動しない
	vec.y = 0.0f;

	return vec;
}

//プレイヤーと敵の衝突判定
void Player::CollisionPlayerVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	//全ての敵と総当たりで衝突処理
	int enemyCount = enemyManager.GetEnemyCount();

	for (int i = 0; i < enemyManager.GetEnemyCount(); i++)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		//衝突処理
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersectSphereVsSphere(
			position, radius,
			enemy->GetPosition(),
			enemy->GetRadius(),
			outPosition))
		{
			//押し出し後の位置設定
			enemy->SetPosition(outPosition);
		}
	}
}

void Player::MoveGrid()
{
	if (GetAsyncKeyState('W') & 0x8000)
	{
		angle.y = 6.3f;
		//if (GridManager::Instance().GetGrid()->GetIsW() == true)
		{
			if (!isWPush && playerY > 0)
				playerY--;
		}
			
		isWPush = true;
	}
	else isWPush = false;

	if (GetAsyncKeyState('S') & 0x8000)
	{
		angle.y = 3.15f;
		if (!isSPush && playerY < MAP_H - 1)
			playerY++;
		isSPush = true;
	}
	else isSPush = false;

	if (GetAsyncKeyState('A') & 0x8000)
	{
		angle.y = 4.75f;
		if (!isAPush && playerX > 0)
			playerX--;
		isAPush = true;
	}
	else isAPush = false;

	if (GetAsyncKeyState('D') & 0x8000)
	{
		angle.y = 1.65f;
		if (!isDPush && playerX < MAP_W - 1)
			playerX++;
		isDPush = true;
	}
	else isDPush = false;

	//map[playerY][playerX];
}

