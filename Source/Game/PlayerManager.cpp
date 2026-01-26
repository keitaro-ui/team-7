#include "PlayerManager.h"

void PlayerManager::Update(float elapsedTime)
{
	player->Update(elapsedTime);
}

void PlayerManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	player->Render(rc, renderer);
}

void PlayerManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
}