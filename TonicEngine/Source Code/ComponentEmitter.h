#ifndef __ComponentEmitter_H__
#define __ComponentEmitter_H__

#include "Component.h"
#include "Timer.h"
#include "Math.h"
#include <list>

class Particle;
class ResourceTexture;

class ComponentEmitter : public Component
{
public:
	ComponentEmitter(GameObject* parent);
	~ComponentEmitter();

	void Inspector();

	bool Update();

	void Save(uint GO_id, nlohmann::json& scene_file);

public:
	Timer timer;

	float ratio = 0.f;

	float life = 0.0f;
	float3 position = float3::zero;
	float3 speed = float3::zero;
	float2 rotation = float2::zero;
	float size = 0.0f;
	ResourceTexture* texture = nullptr;
	float4 color = float4::one;

	std::list<Particle*> particlesList;
};

#endif