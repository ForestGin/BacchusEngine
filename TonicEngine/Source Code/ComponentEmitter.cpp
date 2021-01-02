#include "ComponentEmitter.h"
#include "Application.h"
#include "GameObject.h"
#include "ModuleParticleManager.h"
#include "Particle.h"

ComponentEmitter::ComponentEmitter(GameObject* parent) : Component(COMPONENT_TYPE::EMITTER, parent)
{
	type = COMPONENT_TYPE::EMITTER;
	parent->componentsList.push_back(this);
	
	App->particle_manager->emitters.push_back(this);
}

ComponentEmitter::~ComponentEmitter()
{
}

void ComponentEmitter::Inspector()
{
}

bool ComponentEmitter::Update()
{
	float time = timer.Read();

	if (ratio > 0.0f)
	{
		if (time >= ratio)
		{
			if (App->GetEngineState() == ENGINE_STATE::PLAY)
			{
				int pos = App->particle_manager->GetLastParticle();
				App->particle_manager->particles[pos].SetActive(position, speed, rotation, size, life, &texture, color);
				particlesList.push_back(&App->particle_manager->particles[pos]);
			}
		}
	}

	return true;
}

void ComponentEmitter::Save(uint GO_id, nlohmann::json& scene_file)
{
}
