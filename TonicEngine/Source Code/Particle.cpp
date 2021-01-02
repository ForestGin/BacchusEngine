#include "Particle.h"
#include "Globals.h"

Particle::Particle()
{
}

Particle::~Particle()
{
}

void Particle::SetActive(float3 position, float3 speed, float2 rotation, float size, float life, ResourceTexture** texture, float4 color)
{
	active = true;

	this->position = position;
	this->speed = speed;
	this->rotation = rotation * DEGTORAD;
	this->size = size;
	this->life = life;
	this->texture = texture;
	this->color = color;
}