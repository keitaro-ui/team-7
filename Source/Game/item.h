#pragma once

#include "Enemy.h"
#include <random>
#include "Grid.h"
#include "PlayerManager.h"

class Item : public Enemy
{
public:
	Item();
	~Item() override;

	//XVˆ—
	void Update(float elapsedTime) override;

	//•`‰æˆ—
	void Render(const RenderContext& rc, ModelRenderer* renderer) override;

	//oŒ»
	void Spawn();

	//­‚È‚¢”‚Ìbox‚ğÁ‚·
	void deleteSmallNumber();

private:
	Grid grid;
	std::unique_ptr<Model> model = nullptr;
};