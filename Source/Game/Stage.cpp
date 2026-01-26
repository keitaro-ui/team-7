#include "Stage.h"
#include <imgui.h>

//コンストラクタ
Stage::Stage()
{
	//ステージモデルを読み込み
	model = new Model("Data/Model/Stage/stage.mdl");
	scale.x = scale.y = scale.z = 1.2f;

}

Stage::~Stage()
{
	//ステージモデルを破棄
	delete model;
}

//更新処理
void Stage::Update(float elapsedTime)
{
	DrawDebugGUI();

	UpdateTransform();

	model->UpdateTransform();
}

//デバッグ用GUI描画
void Stage::DrawDebugGUI()
{
	if (ImGui::Begin("Stage"))
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

			if (ImGui::DragFloat3(
				"Rotation",
				&angleDeg.x,
				0.5f     // 回転量（度）
			))
			{
				angle.x = DirectX::XMConvertToRadians(angleDeg.x);
				angle.y = DirectX::XMConvertToRadians(angleDeg.y);
				angle.z = DirectX::XMConvertToRadians(angleDeg.z);
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
void Stage::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	//DirectX::XMStoreFloat4x4(&transform, DirectX::XMMatrixIdentity());

	//レンダラモデルに描画してもらう
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}
