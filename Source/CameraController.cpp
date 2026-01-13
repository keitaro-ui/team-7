#include"System/Input.h"
#include"CameraController.h"
#include"Camera.h"
#include<System/Graphics.h>
#include "imgui.h"

void CameraController::Initialize()
{

}

//更新処理
void CameraController::Update(float elapsedTime)
{
	/*GamePad& gamePad = Input::Instance().GetGamePad();*/
	Mouse& mouse = Input::Instance().GetMouse();

	CameraController::MouseCamera(elapsedTime);

	//float mouseDeltaPosX = mouse.GetPositionX() - mouse.GetOldPositionX();
	//float mouseDeltaPosY = mouse.GetPositionY() - mouse.GetOldPositionY();
	float mouseDeltaPosX = mouse.GetDeltaPositionX();
	float mouseDeltaPosY = mouse.GetDeltaPositionY();

	float movePower = 0.01f;

	float ax = mouseDeltaPosX * movePower;
	float ay = mouseDeltaPosY * movePower;
	
	angle.x -= ay * 0.2f;
	angle.y += ax * 0.2f;


	//X軸のカメラ回転を制限
	if (angle.x < minAngleX)
	{
		angle.x = minAngleX;
	}
	if (angle.x > maxAngleX)
	{
		angle.x = maxAngleX;
	}

	if (angle.y < minAngleY)
	{
		angle.y = minAngleY;
	}
	if (angle.y > maxAngleY)
	{
		angle.y = maxAngleY;
	}


	//Y軸の回転値を-3.14～3.14に収まるようにする
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}

	//カメラ回転値を回転行列に変換
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//回転行列から前方向ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	//注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
	
	eye.x = target.x - front.x;// *range;
	eye.y = target.y + front.y;// *range;
	eye.z = target.z - front.z;// *range;

	//向いている方向のベクトルを計算で出す
	dir.x = target.x - eye.x;
	dir.y = target.y - eye.y;
	dir.z = target.z - eye.z;
	float len = sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	dir.x /= len;
	dir.y /= len;
	dir.z /= len;

	//カメラの視点と注視点を設定
	Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));

	//SetCursorPos(Graphics::Instance().GetScreenWidth() / 2.0f, Graphics::Instance().GetScreenHeight() / 2.0f);
	//mouse.Update();
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

