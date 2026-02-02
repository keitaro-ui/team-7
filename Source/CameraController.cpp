#include"System/Input.h"
#include"CameraController.h"
#include"Camera.h"
#include<System/Graphics.h>
#include "imgui.h"

void CameraController::Initialize()
{
	target = { 0.4f,-2.6f,3.0f };
	distance = 30.2f;
	//angle = { 1.361f,0 };
}

void CameraController::Update(float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();

	float mouseDeltaX = mouse.GetDeltaPositionX();
	float mouseDeltaY = mouse.GetDeltaPositionY();

	// マウスで回転（Y軸のみ）
	//angle.y += mouseDeltaX * 0.002f;

	// Y回転だけの行列
	DirectX::XMMATRIX rotX = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX rotY = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX rot = rotX * rotY;

	//// 後ろ方向ベクトル
	//DirectX::XMVECTOR back =
	//	DirectX::XMVector3TransformCoord(
	//		DirectX::XMVectorSet(0, 0, -distance, 0),
	//		rotY
	//	);

	//DirectX::XMFLOAT3 backDir;
	//DirectX::XMStoreFloat3(&backDir, back);

	//eye.x = target.x + backDir.x;
	//eye.y = target.y + height;
	//eye.z = target.z + backDir.z;

	//// わずかに前を見る
	///*target.x = eye.x;
	//target.y = eye.y - 10.0f;
	//target.z = eye.z + 2.0f; */

	//Camera::Instance().SetLookAt(
	//	eye,
	//	target,
	//	DirectX::XMFLOAT3(0, 1, 0)
	//);
	DirectX::XMVECTOR offset =
		DirectX::XMVector3TransformCoord(
			DirectX::XMVectorSet(0, 0, -distance, 0),
			rot
		);

	DirectX::XMFLOAT3 off;
	DirectX::XMStoreFloat3(&off, offset);

	eye.x = target.x + off.x;
	eye.y = target.y + off.y;
	eye.z = target.z + off.z;

	Camera::Instance().SetLookAt(
		eye,
		target,
		DirectX::XMFLOAT3(0, 1, 0)
	);

	//ImGui::Begin("Camera Position");

	//// カメラの中心（これを動かすと位置が変わる）
	//ImGui::DragFloat3("Target", &target.x, 0.1f, -100.0f, 100.0f);

	//// 距離（ズーム）
	//ImGui::DragFloat("Distance", &distance, 0.1f, 1.0f, 200.0f);

	//// 回転も触れるように
	//ImGui::DragFloat2("Angle", &angle.x, 0.01f, -DirectX::XM_PI, DirectX::XM_PI);

	//ImGui::End();
	target = { 0.4f,-2.6f,3.0f };
	distance = 30.2f;
}


void CameraController::MouseCamera(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	Mouse& mouse = Input::Instance().GetMouse();

	// IMGUIのマウス入力値を使ってカメラ操作する
	ImGuiIO io = ImGui::GetIO();

	// マウスカーソルの移動量を求める
	float moveX = io.MouseDelta.x * 0.02f;
	float moveY = io.MouseDelta.y * 0.02f;

	// カメラ平行移動
	if (io.MouseDown[ImGuiMouseButton_Middle])
	{
		float mousePosX = io.MousePos.x;
		float mousePosY = io.MousePos.y;

		DirectX::XMFLOAT2 mousePos = { mousePosX,mousePosY };
		//target += {mousePos.x, 0, mousePos.y};
	}

	// マウス左ボタン押下中
	if (io.MouseDown[ImGuiMouseButton_Left])
	{
		// Y軸回転
		angle.y += moveX * 0.5f;
		if (angle.y > DirectX::XM_PI)
		{
			angle.y -= DirectX::XM_2PI;
		}
		else if (angle.y < -DirectX::XM_PI)
		{
			angle.y += DirectX::XM_2PI;
		}
		// X軸回転
		angle.x += moveY * 0.5f;
		if (angle.x > DirectX::XM_PI)
		{
			angle.x -= DirectX::XM_2PI;
		}
		else if (angle.x < -DirectX::XM_PI)
		{
			angle.x += DirectX::XM_2PI;
		}
	}

	// マウスホイール
	else if (io.MouseWheel != 0)
	{
		// ズーム
		range -= io.MouseWheel * range * 0.1f;
	}
}

