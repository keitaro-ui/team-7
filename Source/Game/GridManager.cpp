#include "GridManager.h"

GridManager::GridManager()
{
	
}

void GridManager::Update(float elapsedTime)
{
	
}

void GridManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{

}

void GridManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
}

int GridManager::GetData(int x, int y)
{
	return grid->map[y][x];
}