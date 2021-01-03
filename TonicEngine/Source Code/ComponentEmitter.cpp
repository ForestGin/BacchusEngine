#include "ComponentEmitter.h"
#include "Application.h"
#include "GameObject.h"
#include "ModuleParticleManager.h"
#include "ModuleGUI.h"
#include "ModuleSceneIntro.h"
#include "Particle.h"
#include "ResourceTexture.h"

ComponentEmitter::ComponentEmitter(GameObject* parent) : Component(COMPONENT_TYPE::EMITTER, parent)
{	
	parent->componentsList.push_back(this);
	App->particle_manager->emitters.push_back(this);
}

ComponentEmitter::~ComponentEmitter()
{
}

void ComponentEmitter::DrawInspector()
{

	if (ImGui::CollapsingHeader("Particle Emitter", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat("Speed", &speed, 0.1f, 0.0f, 0.0f, "%.2f"))
		{
		}

		ImGui::Separator();

		if (ImGui::DragFloat("Rotation##Particles", &rotation, 0.1f, 0.0f, 0.0f, "%.2f"))
		{
		}

		ImGui::Separator();

		if (ImGui::DragFloat("Size", &size, 0.1f, 0.0f, 0.0f, "%.2f"))
		{
		}

		ImGui::Separator();

		if (ImGui::DragFloat("Ratio", &ratio, 0.1f, 0.0f, 0.0f, "%.2f"))
		{
		}

		ImGui::Separator();

		if (texture)
		{
			ImGui::Text("%s", texPath.c_str());
			ImGui::Image((ImTextureID*)texture->tex.id, ImVec2(310, 310), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
		}
		

		ImGui::Separator();

		ImGui::ColorPicker4("Color", &color.x);


		ImGui::Separator();

		if (ImGui::Button("Reset##Particles"))
		{
			speed = 1.0f;
			rotation = 0.0f;
			size = 1.0f;
			ratio = 0.0f;
		}

	}
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
				float3 up = float3::unitY;

				int pos = App->particle_manager->GetLastParticle();
				App->particle_manager->particles[pos].SetActive(position, speed, (float3::unitY * object->GetComponentTransform()->GetQuatRotation().ToFloat3x3()).Normalized(), rotation, size, life, &texture, color);
				particlesList.push_back(&App->particle_manager->particles[pos]);
				App->particle_manager->particles[pos].emitterpart = this;
				App->particle_manager->activeParticles++;
			}
		}
	}

	return true;
}

void ComponentEmitter::Save(uint GO_id, nlohmann::json& scene_file)
{
	
}
