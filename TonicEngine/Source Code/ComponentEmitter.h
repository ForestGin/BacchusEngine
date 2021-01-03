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

	void DrawInspector();

	bool Update();

	void Save(uint GO_id, nlohmann::json& scene_file);

public:
	Timer timer;

	float ratio = 0.f;

	float life = 0.0f;
	float3 position = float3::zero;
	float speed = 1.0f;
	float rotation = 0.0f;
	float size = 1.0f;
	ResourceTexture* texture = nullptr;
	float4 color = float4::one;
	std::string texPath;
	float3 direction;

	std::list<Particle*> particlesList;
};

#endif