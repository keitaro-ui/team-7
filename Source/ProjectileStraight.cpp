#include "ProjectileStraight.h"

//コンストラクタ
ProjectileStraight::ProjectileStraight(ProjectileManager* manager) : Projectile(manager)
{
	model = new Model("Data/Model/Bullet/bullet.mdl");

	//表示サイズを調整
	scale.x = scale.y = scale.z = 0.02f;
}

//デストラクタ
ProjectileStraight::~ProjectileStraight()
{
	delete model;
}

//更新処理
void ProjectileStraight::Update(float elapsedTime)
{
	//寿命処理
	lifeTimer -= elapsedTime;
	if (lifeTimer <= 0.0f)
	{
		//自分を削除
		Destroy();
	}

	//移動
	float speed = this->speed * elapsedTime;
	position.x += direction.x * speed;
	position.y += direction.y * speed;
	position.z += direction.z * speed;

	//オブジェクト行列を更新
	UpdateTransform();
	//DirectX::XMMATRIX myMatrix = DirectX::XMLoadFloat4x4(&transform);
	//DirectX::XMMATRIX parentMatrix = DirectX::XMLoadFloat4x4(&parentTransform);
	//DirectX::XMStoreFloat4x4(&transform, myMatrix * parentMatrix);

	//モデル行列更新
	model->UpdateTransform();
}

//描画処理
void ProjectileStraight::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}

//発射
void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT4X4 parentTransform)
{
	this->direction = direction;
	this->parentTransform = parentTransform;
}

void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3 position)
{
	this->direction = direction;
	this->position = position;
}
