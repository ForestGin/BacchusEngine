#ifndef __Particle_H__
#define __Particle_H__

#include "Math.h"
#include "ResourceTexture.h"

class Particle
{
public:
	Particle();
	~Particle();

	void SetActive(float3 position, float3 speed, float2 rotation, float size, float life, ResourceTexture** texture, float4 color);

public:
	bool active = false;

	float life = 0.0f;
	float3 position = float3::zero;
	float3 speed = float3::zero;
	float2 rotation = float2::zero;
	float size = 0.0f;
	ResourceTexture** texture = nullptr;
	float4 color = float4::one;
};

#endif