#pragma once

#include <set>
#include<vector>
#include"Enemy.h"

//エネミーマネージャー
class EnemyManager
{
private:
	EnemyManager() {}
	~EnemyManager() {}

	//エネミー同士の衝突処理
	void CollisionEnemyVsEnemies();

public:
	//唯一のインスタンス取得
	static EnemyManager& Instance()
	{
		static EnemyManager instance;
		return instance;
	}

	//エネミー登録
	void Register(Enemy* enemy);

	//エネミー削除
	void Remove(Enemy* enemy);

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	//デバッグプリミティブ描画
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	//エネミー数取得
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

	//エネミー取得
	Enemy* GetEnemy(int index) { return enemies.at(index); }

	//エネミー全削除
	void Clear();

private:
	std::vector<Enemy*>		enemies;

	std::set<Enemy*>		removes;
};
