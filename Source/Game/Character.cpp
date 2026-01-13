#include "Character.h"
#include "System/ShapeRenderer.h"

//行列更新処理
void Character::UpdateTransform()
{
	//スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	//回転行列を作成
	//DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x,angle.y,angle.z);
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(angle.z);
	DirectX::XMMATRIX R = Rz * Ry * Rx;
	//DirectX::XMMATRIX R = Rx;

	//位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	//3つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;

	//計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
}

//デバッグプリミティブ描画
void Character::RenderDebugPrimitive(const RenderContext & rc, ShapeRenderer * renderer)
{
	//衝突判定用のデバッグ円柱を描画
	renderer->RenderCylinder(rc, position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

//ダメージを与える
bool Character::ApplyDamage(int damage, float invincibleTime)
{
	//ダメージ0なら健康状態を変更する必要がない
	if (damage == 0) return false;
	
	//死亡している場合は健康状態を変更しない
	if (health <= 0) return false;

	//無敵時間中はダメージを与えない
	if (invincibleTimer >= 0.0f) return false;

	//無敵時間設定
	invincibleTimer = invincibleTime;

	//ダメージ処理
	health -= damage;

	//死亡通知
	if (health <= 0)
	{
		OnDead();
	}

	//ダメージ通知
	else
	{
		OnDamaged();
	}

	//健康状態を変更したらtrueを返す
	return true;
}

//移動処理
void Character::Move(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;
	position.x += vx * speed;
	position.z += vz * speed;
}

void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	//進行ベクトルがゼロベクトルの場合は処理する必要なし
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.001f) return;

	//進行ベクトルを単位ベクトル化
	vx /= length;
	vz /= length;

	//自身の回転値から前方向を求める
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	//回転角を求めるため、2つの単位ベクトルの内積を計算する
	float dot = (frontX * vx) + (frontZ * vz);

	//内積値は-1.0～1.0で表現されていて、2つの単位ベクトルの角度が
	//小さいほどに1.0に近づくという性質を利用して回転速度を調節する
	float rot = 1.0f - dot;
	if (rot > speed) rot = speed;

	//左右判定を行うために２つの単位ベクトルの外積を計算する
	float cross = (frontZ * vx) - (frontX * vz);

	//2Dの外積値が正の場合か負の場合によって左右判定を行える
	//左右判定を行うことによって左右回転を選択する
	if (cross < 0.0f)
	{
		angle.y -= rot;
	}
	else
	{
		angle.y += rot;
	}
}

//ジャンプ処理
void Character::Jump(float speed)
{
	//上方向の力を設定
	velocity.y = speed;
}

//速力更新処理
void Character::UpdateVelocity(float elapsedTime)
{
	//重力処理
	velocity.y += gravity * elapsedTime;

	//移動処理
	position.y += velocity.y * elapsedTime;

	//地面判定
	if (position.y < 0.0f)
	{
		position.y = 0.0f;
		velocity.y = 0.0f;

		//着地した
		if (!isGround)
		{
			OnLanding();
		}
		isGround = true;
	}
	else
	{
		isGround = false;
	}
}

//無敵時間更新
void Character::UpdateInvincibleTimer(float elapsedTime)
{
	if (invincibleTimer > 0.0f)
	{
		invincibleTimer -= elapsedTime;
	}
}
