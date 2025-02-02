#include "ComponentEmitter.h"
#include "Application.h"
#include "GameObject.h"
#include "ModuleParticleManager.h"
#include "ModuleGUI.h"
#include "ModuleSceneIntro.h"
#include "Particle.h"
#include "ResourceTexture.h"
#include "PanelInspector.h"
#include "Component.h"

ComponentEmitter::ComponentEmitter(GameObject* parent) : Component(COMPONENT_TYPE::EMITTER, parent)
{	
	parent->componentsList.push_back(this);
	App->particle_manager->emitters.push_back(this);
}

ComponentEmitter::~ComponentEmitter()
{
	App->particle_manager->emitters.remove(this);
}

bool ComponentEmitter::Start()
{
	timer.Start();
	timerBurst.Start();

	Clear();

	return true;
}

void ComponentEmitter::DrawInspector()
{

	if (ImGui::CollapsingHeader("Particle Emitter", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat("Speed", &speed, 0.1f, 0.0f, 0.0f, "%.2f"))
		{
		}

		ImGui::Separator();

		if (ImGui::DragFloat("Life", &life, 0.1f, 0.0f, 0.0f, "%.2f"))
		{
		}

		ImGui::Separator();

		if (ImGui::DragFloat3("Direction", &direction.x, 0.1f, 0.0f, 0.0f, "%.2f"))
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

		if (ImGui::DragFloat("Time between particles", &ratio, 0.1f, 0.0f, 0.0f, "%.2f"))
		{
		}

		ImGui::Separator();

		if (ImGui::DragFloat("Time between bursts", &burstRatio, 0.1f, 0.0f, 0.0f, "%.2f"))
		{
		}

		ImGui::Separator();

		if (ImGui::DragInt("Particles in burst", &particlesBurst, 1, 0, 100))
		{
		}

		ImGui::Separator();

		if (ImGui::RadioButton("Cone shape", shapeType == Cone_TYPE))
		{
			shapeType = Cone_TYPE;
		}

		if (ImGui::RadioButton("Sphere shape", shapeType == Sphere_TYPE))
		{
			shapeType = Sphere_TYPE;
		}

		if (ImGui::RadioButton("Box shape", shapeType == Box_TYPE))
		{
			shapeType = Box_TYPE;
		}

		ImGui::Separator();

		switch (shapeType)
		{
			case Cone_TYPE:

				if (ImGui::DragFloat("Cone heigh", &heigh, 0.1f, 0.0f, 0.0f, "%.2f"))
				{
					circle.pos.y = heigh;

				}

				if (ImGui::DragFloat("Cone radius", &rad, 0.1f, 0.0f, 0.0f, "%.2f"))
				{
					circle.r = rad;

				}
				break;
			case Sphere_TYPE:
				if (ImGui::DragFloat("Sphere radius", &rad, 0.1f, 0.0f, 0.0f, "%.2f"))
				{
					sphere.r = rad;

				}
				break;
			case Box_TYPE:
				if (ImGui::DragFloat3("Box size", &boxSize.x, 0.1f, 0.0f, 0.0f, "%.2f"))
				{
					cube.SetFromCenterAndSize(object->GetComponentTransform()->GetPosition(), boxSize);

				}
				break;
			default:
				break;
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
			life = 0.0f;
			burstRatio = 0.0f;
			particlesBurst = 0;
		}

	}
}

bool ComponentEmitter::Update()
{

	if (object->GetComponentTexture() != nullptr)
	{
		ComponentTexture* tex = (ComponentTexture*)object->GetComponent(COMPONENT_TYPE::TEXTURE);
		texture = tex->rTexture;
	}
	else
	{
		object->CreateComponent(COMPONENT_TYPE::TEXTURE);
		ComponentTexture* tex = (ComponentTexture*)object->GetComponent(COMPONENT_TYPE::TEXTURE);
		texture = tex->rTexture;
	}

	if (startUpdate != true)
		return false;

	if (ratio > 0.0f)
	{

		float time = timer.Read();

		if (time >= ratio)
		{
			if (App->GetEngineState() == ENGINE_STATE::PLAY)
			{
				int pos = App->particle_manager->GetLastParticle();
				ActiveParticle(pos);
				particlesList.push_back(&App->particle_manager->particles[pos]);
				App->particle_manager->particles[pos].emitterpart = this;
				App->particle_manager->activeParticles++;
				timer.Start();
			}
		}
	}

	if (burstRatio > 0.0f)
	{
		float burstTime = timerBurst.Read();

		if (burstTime >= burstRatio)
		{
			if (App->GetEngineState() == ENGINE_STATE::PLAY)
			{
				for (int i = 0; i < particlesBurst; ++i)
				{
					int pos = App->particle_manager->GetLastParticle();
					ActiveParticle(pos);
					particlesList.push_back(&App->particle_manager->particles[pos]);
					App->particle_manager->particles[pos].emitterpart = this;
					App->particle_manager->activeParticles++;
					
				}

				timerBurst.Start();
			}
		}
	}

	return true;
}

void ComponentEmitter::ActiveParticle(int pos, bool isactive, float3 startposition)
{
	float3 randompoint = float3::zero;
	float3 initialpos = float3::zero;
	float3 directionvec = float3::zero;

	switch (shapeType)
	{
	case Cone_TYPE:
		randompoint = circle.RandomPointInside(App->random);
		if (isactive == false)initialpos = object->GetComponentTransform()->GetPosition();
		else initialpos = startposition;
		randompoint += initialpos;
		directionvec = (randompoint - initialpos).Normalized();
		App->particle_manager->particles[pos].SetActive(object->GetComponentTransform()->GetPosition(), speed, directionvec, rotation, size, life, &texture, color);
		break;
	case Sphere_TYPE:
		randompoint = sphere.RandomPointInside(App->random);
		if (isactive == false)initialpos = object->GetComponentTransform()->GetPosition();
		else initialpos = startposition;
		randompoint += initialpos;
		directionvec = (randompoint - initialpos).Normalized();
		App->particle_manager->particles[pos].SetActive(randompoint, speed, directionvec, rotation, size, life, &texture, color);
		break;
	case Box_TYPE:
		randompoint = cube.RandomPointInside(App->random);
		if (isactive == true) randompoint += startposition;
		else randompoint += object->GetComponentTransform()->GetPosition();
		App->particle_manager->particles[pos].SetActive(randompoint, speed, (float3::unitY * object->GetComponentTransform()->GetQuatRotation().ToFloat3x3()).Normalized(), rotation, size, life, &texture, color);
		break;
	default:
		break;
	}
}

void ComponentEmitter::Clear()
{
	for (auto particle : particlesList)
	{
		particle->active = false;
	}
	particlesList.clear();
}

void ComponentEmitter::Save(uint GO_id, nlohmann::json& scene_file)
{
	
}
