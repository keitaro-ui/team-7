#pragma once


#include <DirectXMath.h>
#include "System/ShapeRenderer.h"

//キャラクター
class Character
{
public:
	Character(){}
	virtual ~Character(){}

	//行列更新処理
	void UpdateTransform();

	//位置取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	//位置設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
	
	//回転取得
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	//回転設定
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	//スケール取得
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	//スケール設定
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	//半径取得
	float GetRadius() const { return radius; }

	//高さ取得
	float GetHeight() const { return height; }

	//デバッグプリミティブ描画
	virtual void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	//接地判定
	bool IsGround() const { return isGround; }

	//ダメージを与える
	bool ApplyDamage(int damage, float invincibleTime);

protected:
	DirectX::XMFLOAT3		position = { 0,1,0 };
	DirectX::XMFLOAT3		angle = { 0,0,0 };
	DirectX::XMFLOAT3		scale = { 1,1,1 };
	DirectX::XMFLOAT4X4		transform =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	float radius = 0.5f;
	float height = 2.0f;

	//移動処理
	void Move(float elapsedTime, float vx, float vz, float speed);

	//旋回処理
	void Turn(float elapsedTime, float vx, float vz, float speed);

	//ジャンプ処理
	void Jump(float speed);

	//速力処理更新
	void UpdateVelocity(float elapsedTime);

	//無敵時間更新
	void UpdateInvincibleTimer(float elapsedTime);

	//着地した時に呼ばれる
	virtual void OnLanding() {}

	//ダメージ受けたら呼ばれる
	virtual void OnDamaged() {}

	//死亡したら呼ばれる
	virtual void OnDead() {}

protected:
	float				gravity = 0.0f;
	DirectX::XMFLOAT3	velocity = { 0, 0, 0 };

	bool				isGround = false;

	int					health = 1;
	float				invincibleTimer = 1.0f;
};
