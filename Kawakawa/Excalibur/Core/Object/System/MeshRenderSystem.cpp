#include "MeshRenderSystem.h"

int Excalibur::MeshRenderSystem::Initialize() noexcept
{
	return 0;
}

void Excalibur::MeshRenderSystem::Finalize() noexcept
{
}

void Excalibur::MeshRenderSystem::Tick() noexcept
{
}

void Excalibur::MeshRenderSystem::AddComponent(MeshRenderComponent* comp)
{
	mComponents.insert(comp);
}

void Excalibur::MeshRenderSystem::DeleteComponent(MeshRenderComponent* comp)
{
	mComponents.erase(comp);
}


void Excalibur::MeshRenderSystem::Render()
{
	if (!IsActive())
		return;

	for (auto comp : mComponents) {
		if (comp->IsVisible()) {
			comp->Render();
		}
	}
}