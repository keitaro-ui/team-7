#pragma once

#include <DirectXMath.h>

//カメラコントローラー
class CameraController
{
public:
	void Initialize();

	//更新処理
	void Update(float elapsedTime);

	//ターゲット位置設定
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }
	DirectX::XMFLOAT3 eye;
	DirectX::XMFLOAT3 dir = {};

	const DirectX::XMFLOAT3& getAngle() const{ return angle; };

	void MouseCamera(float elapsedTime);

	DirectX::XMFLOAT3		target = { 0, 0, 0 }; //注視点
	DirectX::XMFLOAT3		angle = { 0, 0, 0 }; //回転角度
	float distance = 10.0f;
private:
	float					rollSpeed = DirectX::XMConvertToRadians(90); //回転速度
	float					range = 10.0f; //距離
	
	float					maxAngleX = DirectX::XMConvertToRadians(30);
	float					minAngleX = DirectX::XMConvertToRadians(-30);

	float					maxAngleY = DirectX::XMConvertToRadians(45);
	float					minAngleY = DirectX::XMConvertToRadians(-45);
};
