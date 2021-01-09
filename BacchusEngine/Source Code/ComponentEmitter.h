#ifndef __ComponentEmitter_H__
#define __ComponentEmitter_H__

#include "Component.h"
#include "Timer.h"
#include "Math.h"
#include <list>

class Particle;
class ResourceTexture;

enum Shape_TYPE {

	Cone_TYPE = -1,
	Sphere_TYPE,
	Box_TYPE

};

class ComponentEmitter : public Component
{
public:
	ComponentEmitter(GameObject* parent);
	~ComponentEmitter();

	bool Start();

	void DrawInspector();

	bool Update();

	void ActiveParticle(int pos, bool isactive = false, float3 startposition = float3::zero);

	void Clear();

	void Save(uint GO_id, nlohmann::json& scene_file);

public:
	Timer timer;
	Timer timerBurst;

	float ratio = 0.f;
	float burstRatio = 0.f;

	int particlesBurst = 0;

	float life = 0.0f;
	float speed = 1.0f;
	float rotation = 0.0f;
	float size = 1.0f;
	ResourceTexture* texture = nullptr;
	float4 color = float4::one;
	std::string texPath;
	float3 direction = float3::unitY;

	bool startUpdate = true;

	std::list<Particle*> particlesList;

	AABB cube = AABB(float3(-1, -1, -1), float3(1, 1, 1));
	float3 boxSize = float3(2, 2, 2);

	Sphere sphere = Sphere(float3(0, 0, 0), 1);
	Circle circle = Circle(float3(0, 0, 0), float3::unitY, 1);
	float rad = 0.0f;
	float heigh = 0.0f;

	bool active = true;

	Shape_TYPE shapeType = Shape_TYPE::Cone_TYPE;
};

#endif