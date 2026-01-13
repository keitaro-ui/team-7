#pragma once

#include "System/Model.h"
#include "Enemy.h"
#include "Box.h"

//スライム
class Balloon : public Enemy
{
public:
	Balloon();
	~Balloon() override;

	//更新処理
	void Update(float elapsedTime) override;

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer) override;

	Box* box=nullptr;

protected:
	//死亡したときに呼ばれる
	void OnDead() override;

private:
	Model* model = nullptr;
	
	

};
