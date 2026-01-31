#pragma once
#include <random>
#include<memory>

class Grid
{
private:


public:
    Grid();
    //bool isMoving = false;
    struct BoxAnimeData
    {
        int num;
        int x, y;
        int oldX, oldY;
    };
    std::vector<BoxAnimeData> boxAnimeData;

    int score = 0;

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

    int Score(int c);

    //空きマスチェック
    bool HasEmptyCell();

    //合体できるかチェック
    bool CanMerge();

    void CanSlideW();
};
