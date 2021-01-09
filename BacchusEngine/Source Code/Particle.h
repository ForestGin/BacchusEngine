#ifndef __Particle_H__
#define __Particle_H__

#include "Math.h"
#include "ResourceTexture.h"

class ParticlePlane;
class ComponentEmitter;

class Particle
{
public:
	Particle();
	~Particle();

	void SetActive(float3 position, float speed, float3 direction, float rotation, float size, float life, ResourceTexture** texture, float4 color);

	void Update(float dt);

	void CameraOrientation();

	void Draw();

public:
	bool active = false;

	float life = 0.0f;
	float3 position = float3::zero;
	float speed = 1.0f;
	float rotation = 0.0f;
	float size = 0.0f;
	ResourceTexture** texture = nullptr;
	float4 color = float4::one;
	ComponentEmitter* emitterpart = nullptr;
	float3 direction = float3::zero;

	ParticlePlane* plane = nullptr;

	float3 deathpos = float3::zero;

private:
	math::Quat ownRotation = Quat::identity;
	float lifeTime = 0.0f;
};

#endif