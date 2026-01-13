#include "EnemyManager.h"
#include "Collision.h"


//エネミー登録
void EnemyManager::Register(Enemy* enemy)
{
	enemies.emplace_back(enemy);
}

//エネミー削除
void EnemyManager::Remove(Enemy* enemy)
{
	//破棄リストに追加
	removes.insert(enemy);
}

//更新処理
void EnemyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}

	//破棄処理
	//　enemiesの範囲for文中でerase()すると不具合が発生してしまうため、
	//　更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する。
	for (Enemy* enemy : removes)
	{
		//std::vectorから要素を削除する場合はイテレータで削除しなければならない
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
		if (it != enemies.end())
		{
			enemies.erase(it);
		}

		//削除
		delete enemy;
	}
	//破棄リストのクリア
	removes.clear();

	//敵同士の衝突処理
	CollisionEnemyVsEnemies();
}

//描画処理
void EnemyManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Render(rc, renderer);
	}
}

//デバッグプリミティブ描画
void EnemyManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	for (Enemy* enemy : enemies)
	{
		renderer->RenderSphere(rc, enemy->GetPosition(), enemy->GetRadius(), DirectX::XMFLOAT4(0, 0, 0, 1));
	}
}

//エネミー同士の衝突処理
void EnemyManager::CollisionEnemyVsEnemies()
{
	size_t enemyCount = enemies.size();
	for (int i = 0; i < enemyCount; i++)
	{
		Enemy* enemyA = enemies.at(i);
		for (int j = i + 1; j < enemyCount; j++)
		{
			Enemy* enemyB = enemies.at(j);

			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersectSphereVsSphere(
				enemyA->GetPosition(),
				enemyA->GetRadius(),
				enemyB->GetPosition(),
				enemyB->GetRadius(),
				outPosition
			))
			{
				enemyB->SetPosition(outPosition);
			}
		}
	}
}

//エネミー全削除
void EnemyManager::Clear()
{
	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}
