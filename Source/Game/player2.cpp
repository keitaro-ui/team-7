#include"Player2.h"
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
#include "SoundManager.h"
#include "Scene/SceneTitle.h"
//#include "PlayerManager.h"

//コンストラクタ
Player2::Player2()
{
	model = new Model("Data/Model/Player/player_robot.mdl");
	model2 = new Model("Data/Model/Player/player_robot.mdl");

	//Idle
	model->PlayAnimation(1, true);
	model2->PlayAnimation(1, true);


	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.071f;

	angle = { 0,0,0 };

}

//デストラクタ
Player2::~Player2()
{
	delete model;
	delete model2;
}


//更新処理
void Player2::Update(float elapsedTime)
{
	//速力処理更新
	UpdateVelocity(elapsedTime);


	//オブジェクト行列を更新
	UpdateTransform();
	model->UpdateAnimation(elapsedTime);
	model->UpdateTransform();

	model2->UpdateAnimation(elapsedTime);
	model2->UpdateTransform();

	game_timer += elapsedTime;

	DrawDebugGUI();
}



//デバッグ用GUI描画
void Player2::DrawDebugGUI()
{
	if (ImGui::Begin("Player2"))
	{
		// トランスフォーム
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// 位置
			ImGui::DragFloat3(
				"Position",
				&position.x,
				0.01f,          // 移動量
				-1000.0f,       // 最小値
				1000.0f,        // 最大値
				"%.3f"
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

		}
	}
	ImGui::End();

}

//描画処理
void Player2::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);
	renderer->Render(rc, transform, model2, ShaderId::Lambert);
}

//デバッグプリミティブ描画
void Player2::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	//プレイヤーデバッグプリミティブ描画
	//Character::RenderDebugPrimitive(rc, renderer);

	//弾丸デバッグプリミティブ描画
	//projectileManager.RenderDebugPrimitive(rc, renderer);
}




