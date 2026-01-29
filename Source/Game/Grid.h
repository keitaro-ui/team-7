#pragma once
#include <random>

class Grid
{
private:
    bool IsW = false;
    bool IsS = false;
    bool IsA = false;
    bool IsD = false;
public:
    //マス目の数
    enum { GRID_MAX = 5 };

    //移動しているかを判断
    bool moved = false;

    int map[GRID_MAX][GRID_MAX];
    bool merged[GRID_MAX][GRID_MAX];
    int pmap[GRID_MAX][GRID_MAX];

    //方向キーでBox動かす関数
    bool MoveRight();
    bool MoveLeft();
    bool MoveUp();
    bool MoveDown();

    void Spawn();
    bool IsGameOver();

    void PlayerDie(int x,int y);

    //空きマスチェック
    bool HasEmptyCell();

    //合体できるかチェック
    bool CanMerge();

    void CanSlide();

    bool GetIsW() { return IsW; }
    bool GetIsS() { return IsS; }
    bool GetIsA() { return IsA; }
    bool GetIsD() { return IsD; }
};
