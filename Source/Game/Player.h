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

    void MoveGrid();

    int GetMap(int y, int x) const { return map[y][x]; }

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

    //マス目の数
    enum { GRID_MAX = 5 };

   
   
   int mapy = 0;
   int mapx = 0;

    float game_timer;
    const float coolTime = 1.0f;


    float moveSpeed = 5.0f;

    float turnSpeed = DirectX::XMConvertToRadians(720);

    float                jumpSpeed = 12.0f;

    int                    jumpCount = 0;
    int                    jumpLimit = 2;

    const int MAP_W = GRID_MAX;
    const int MAP_H = GRID_MAX;

    //マス目の設定
    int map[GRID_MAX][GRID_MAX] = { 0 };

    bool isWPush = false;
    bool isSPush = false;
    bool isAPush = false;
    bool isDPush = false;


    ProjectileManager    projectileManager;
    DirectX::XMFLOAT3 pposition = {};


    int playerX = 2;
    int playerY = 2;    
    
    float tileSize = 4.4f;
    DirectX::XMFLOAT3 startPos = { -8.8f, 0.0f, -10.8f };
    
   /* int grid1 = 0;
    int grid2 = 0;
    int grid3 = 0;
    int grid4 = 0;
    int grid5 = 0;
    int grid6 = 0;
    int grid7 = 0;
    int grid8 = 0;
    int grid9 = 0;
    int grid10 = 0;
    int grid11 = 0;
    int grid12 = 0;
    int grid13 = 0;
    int grid14 = 0;
    int grid15 = 0;
    int grid16 = 0;
    int grid17 = 0;
    int grid18 = 0;
    int grid19 = 0;
    int grid20 = 0;
    int grid21 = 0;
    int grid22 = 0;
    int grid23 = 0;
    int grid24 = 0;
    int grid25 = 0;
    */

public:
    CameraController* cameraController = nullptr;

    int GetPlayerX()const { return playerX; }
    int GetPlayerY()const { return playerY; }

    bool finish = false;
    //std::chrono::system_clock::time_point minutes;

};