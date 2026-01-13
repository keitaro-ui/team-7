#include "Collision.h"

//球と球の交差判定
bool Collision::IntersectSphereVsSphere(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	DirectX::XMFLOAT3& outPositionB)
{
	//A→Bの単位ベクトルを算出
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);	//XMFLOAT3をXMVectorに
	DirectX::XMVECTOR PositionB	= DirectX::XMLoadFloat3(&positionB);	//変換するのがXMLoadFloat3
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);	//ベクトルの長さ
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);	
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	//距離判定
	float range = radiusA + radiusB;
	if (lengthSq > range * range)
	{
		return false;
	}

	//AがBを押し出す
	Vec = DirectX::XMVector3Normalize(Vec);
	Vec = DirectX::XMVectorScale(Vec, range);
	PositionB = DirectX::XMVectorAdd(PositionA, Vec);

	DirectX::XMStoreFloat3(&outPositionB, PositionB);

	return true;
}

bool Collision::IntersectSphereVsCylinder(
	const DirectX::XMFLOAT3& spherePosition,
	float sphereRadius,
	const DirectX::XMFLOAT3& cylinderPosition, 
	float cylinderRadius, 
	float cylinderHeight,
	DirectX::XMFLOAT3& outCylinderPosition)
{
	//高さチェック
	if (spherePosition.y + sphereRadius < cylinderPosition.y) return false;
	if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderHeight) return false;

	//XZ平面での範囲チェック
	float vx = cylinderPosition.x - spherePosition.x;
	float vz = cylinderPosition.z - spherePosition.z;
	float range = sphereRadius + cylinderRadius;
	float distXZ = sqrtf(vx * vx + vz * vz);
	if (distXZ > range) return false;

	//球が円柱を押し出す
	vx /= distXZ;
	vz /= distXZ;
	outCylinderPosition.x = spherePosition.x + (vx * range);
	outCylinderPosition.y = spherePosition.y;
	outCylinderPosition.z = spherePosition.z + (vz * range);

	return true;
}
