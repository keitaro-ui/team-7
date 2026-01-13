#pragma once

#include "System/Model.h"
#include "Enemy.h"

//スライム
class Box : public Enemy
{
public:
	Box();
	~Box() override;

	//更新処理
	void Update(float elapsedTime) override;

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer) override;

protected:
	//死亡したときに呼ばれる
	void OnDead() override;

private:
	Model* model = nullptr;

};
