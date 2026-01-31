#include "item.h"

Item::Item()
{
	model = std::make_unique<Model>("Data/Model/Boad/Box.mdl");
}

Item::~Item()
{
}

void Item::Update(float elapsedTime)
{
}

void Item::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model.get(), ShaderId::Lambert);
}

void Item::Spawn()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int>distX(0, Grid::Instance().GRID_MAX - 1);
	std::uniform_int_distribution<int>distY(0, Grid::Instance().GRID_MAX - 1);
	while (true)
	{
		int x = distX(gen);
		int	y = distY(gen);

		if (Grid::Instance().map[y][x] == 0)
		{
			int playerX = PlayerManager::Instance().GetPlayer()->GetPlayerX();
			int playerY = PlayerManager::Instance().GetPlayer()->GetPlayerY();

			if (playerY != y && playerX != x)
			{
				
				break;
			}
		}
	}
}

void Item::deleteSmallNumber()
{
	for (int x = 0; x < Grid::Instance().GRID_MAX; x++)
	{
		for (int y = 1; y < Grid::Instance().GRID_MAX; y++)
		{
			//8ˆÈ‰º‚È‚çÁ‚·
			if (Grid::Instance().map[y][x] < 4)
			{
				Grid::Instance().map[y][x] = 0;
			}
		}
	}
}
