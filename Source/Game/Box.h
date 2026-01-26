#pragma once

#include "System/Model.h"
#include "Enemy.h"

class Box : public Enemy
{
public:
	Box(int num);
	~Box() override;

	//XVˆ—
	void Update(float elapsedTime) override;

	//•`‰æˆ—
	void Render(const RenderContext& rc, ModelRenderer* renderer) override;

private:
	Model* model = nullptr;
	float radius = 0.0f;
	int num = 0;
};
