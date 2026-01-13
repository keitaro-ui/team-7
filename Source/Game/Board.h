#pragma once

#include "System/Model.h"
#include "Enemy.h"

//スライム
class Board : public Enemy
{
public:
	Board();
	~Board() override;

	//更新処理
	void Update(float elapsedTime) override;

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer) override;

	void CreateModel();

protected:
	//死亡したときに呼ばれる
	void OnDead() override;

private:
	
};