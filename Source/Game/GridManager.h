#pragma once

#include <set>
#include<vector>
#include"./Game/Grid.h"
#include <ProjectileManager.h>

//エネミーマネージャー
class GridManager
{
private:
	GridManager() {}
	~GridManager() {}

public:
	//唯一のインスタンス取得
	static GridManager& Instance()
	{
		static GridManager instance;
		return instance;
	}

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	//デバッグプリミティブ描画
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	void Register(Grid* g) { grid = g; }

	Grid* GetGrid() { return grid; }
	
private:
	Grid* grid;
};