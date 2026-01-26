#pragma once

#include "System/ModelRenderer.h"
#include "Character.h"
#include <ProjectileManager.h>
#include "CameraController.h"
#include "System/AudioSource.h"

//プレイヤー
class Player : public Character
{
public:
    Player();
    ~Player() override;

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render(const RenderContext& rc, ModelRenderer* renderer);

    //デバッグ用GUI描画
    void DrawDebugGUI();

    //デバッグプリミティブ描画
    void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer) override;




private:
    Model* model = nullptr;

    //移動入力処理
    void InputMove(float elapsedTime);

    void MoveD(float elapsedTime, float vx, float vz, float speed);

    DirectX::XMFLOAT3 dir = {};

    //スティック入力値から移動ベクトルを取得
    DirectX::XMFLOAT3 GetMoveVec() const;

    //プレイヤーとエネミーとの衝突処理
    void CollisionPlayerVsEnemies();


    float moveSpeed = 5.0f;

    float turnSpeed = DirectX::XMConvertToRadians(720);

    float                jumpSpeed = 12.0f;

    int                    jumpCount = 0;
    int                    jumpLimit = 2;



    ProjectileManager    projectileManager;
    DirectX::XMFLOAT3 pposition = {};


public:
    CameraController* cameraController = nullptr;

    bool finish = false;
    //std::chrono::system_clock::time_point minutes;

};