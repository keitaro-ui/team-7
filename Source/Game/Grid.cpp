#include "Grid.h"
#include "Player.h"
#include "./Game/PlayerManager.h"

//方向キーでBox動かす関数
bool Grid::MoveRight()
{
	std::memset(merged, false, sizeof(merged));
	moved = false;

	for (int y = 0; y < GRID_MAX; y++)
	{
		for (int x = GRID_MAX - 2; x >= 0; x--)
		{
			if (map[y][x] != 0)
			{
				int cx = x;
				while (cx < GRID_MAX - 1)
				{
					//一個右が0なら移動させる
					if (map[y][cx + 1] == 0)
					{
						map[y][cx + 1] = map[y][cx];
						map[y][cx] = 0;
						cx++;
						moved = true;
					}
					//一個右と一緒なら合体
					else if (map[y][cx + 1] == map[y][cx]
						&& !merged[y][cx + 1])
					{
						map[y][cx + 1]++;
						map[y][cx] = 0;
						merged[y][cx + 1] = true;
						moved = true;
						break;
					}
					// 一個右が0じゃなくて、違う数字ならそのまま
					else break;
				}
			}
		}
	}
	return moved;
}

bool Grid::MoveLeft()
{
	std::memset(merged, false, sizeof(merged));
	moved = false;

	for (int y = 0; y < GRID_MAX; y++)
	{
		for (int x = 1; x < GRID_MAX; x++)
		{
			if (map[y][x] != 0)
			{
				int cx = x;
				while (cx > 0)
				{
					//一個左が0なら移動させる
					if (map[y][cx - 1] == 0)
					{
						map[y][cx - 1] = map[y][cx];
						map[y][cx] = 0;
						cx--;
						moved = true;
					}
					//一個左と一緒なら合体
					else if (map[y][cx - 1] == map[y][cx]
						&& !merged[y][cx - 1])
					{
						map[y][cx - 1]++;
						map[y][cx] = 0;
						merged[y][cx - 1] = true;
						moved = true;
						break;
					}
					// 一個左が0じゃなくて、違う数字ならそのまま
					else break;
				}
			}
		}
	}
	return moved;
}

bool Grid::MoveUp()
{
	std::memset(merged, false, sizeof(merged));
	moved = false;

	for (int x = 0; x < GRID_MAX; x++)
	{
		for (int y = 1; y < GRID_MAX; y++)
		{
			if (map[y][x] != 0)
			{
				int cy = y;
				while (cy > 0)
				{
					//一個上が0なら移動させる
					if (map[cy - 1][x] == 0)
					{
						map[cy - 1][x] = map[cy][x];
						map[cy][x] = 0;
						cy--;
						moved = true;
					}
					//一個上と一緒なら合体
					else if (map[cy - 1][x] == map[cy][x]
						&& !merged[cy - 1][x])
					{
						map[cy - 1][x]++;
						map[cy][x] = 0;
						merged[cy - 1][x] = true;
						moved = true;
						break;
					}
					// 一個上が0じゃなくて、違う数字ならそのまま
					else break;
				}
			}
		}
	}
	return moved;
}

bool Grid::MoveDown()
{
	std::memset(merged, false, sizeof(merged));
	moved = false;

	for (int x = 0; x < GRID_MAX; x++)
	{
		for (int y = GRID_MAX - 2; y >= 0; y--)
		{
			if (map[y][x] != 0)
			{
				int cy = y;
				while (cy < GRID_MAX - 1)
				{
					//一個下が0なら移動させる
					if (map[cy + 1][x] == 0)
					{
						map[cy + 1][x] = map[cy][x];
						map[cy][x] = 0;
						cy++;
						moved = true;
					}
					//一個下と一緒なら合体
					else if (map[cy + 1][x] == map[cy][x]
						&& !merged[cy + 1][x])
					{
						map[cy + 1][x]++;
						map[cy][x] = 0;
						merged[cy + 1][x] = true;
						moved = true;
						break;
					}
					// 一個下が0じゃなくて、違う数字ならそのまま
					else break;
				}
			}
		}
	}
	return moved;
}

void Grid::Spawn()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int>distX(0, GRID_MAX - 1);
	std::uniform_int_distribution<int>distY(0, GRID_MAX - 1);
	while (true)
	{
		int x = distX(gen);
		int	y = distY(gen);

		if (map[y][x]==0)
		{
			int playerX = PlayerManager::Instance().GetPlayer()->GetPlayerX();
			int playerY = PlayerManager::Instance().GetPlayer()->GetPlayerY();
			//if (pmap[playerY][playerX] != map[y][x])
			if (playerY != y&& playerX!=x)
			{
				//2の箱だす
				map[y][x] = 1;
				break;
			}
		}
	}
}

bool Grid::IsGameOver()
{
	//
	if (PlayerDie())
		return true;

	// 空きマスがあればまだ終わらない
	if (HasEmptyCell())
		return false;

	// 合体できるならまだ終わらない
	if (CanMerge())
		return false;

	// 空きなし＆合体不可 → ゲームオーバー
	return true;
}

bool Grid::PlayerDie()
{
	int playerX = PlayerManager::Instance().GetPlayer()->GetPlayerX();
	int playerY = PlayerManager::Instance().GetPlayer()->GetPlayerY();
	for (int y = 0; y < GRID_MAX; y++)
	{
		for (int x = 0; x < GRID_MAX; x++)
		{
			if (playerY == y && playerX == x)
			{
				return false;
			}
		}
	}
	return true;
}

bool Grid::HasEmptyCell()
{
	for (int y = 0; y < GRID_MAX; y++)
	{
		for (int x = 0; x < GRID_MAX; x++)
		{
			if (map[y][x] == 0)
				return true;
		}
	}
	return false;
}

bool Grid::CanMerge()
{
	for (int y = 0; y < GRID_MAX; y++)
	{
		for (int x = 0; x < GRID_MAX; x++)
		{
			int v = map[y][x];
			if (v == 0) continue;

			//右と下だけ見れば十分
			if (x + 1 < GRID_MAX && map[y][x + 1] == v) return true;
			if (y + 1 < GRID_MAX && map[y + 1][x] == v) return true;
		}
	}
	return false;
}
