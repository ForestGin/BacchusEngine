#include "Particle.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleParticleManager.h"


Particle::Particle()
{
}

Particle::~Particle()
{
}

void Particle::SetActive(float3 position, float speed, float3 direction, float rotation, float size, float life, ResourceTexture** texture, float4 color)
{
	active = true;

	this->position = position;
	this->speed = speed;
	this->rotation = rotation * DEGTORAD;
	this->size = size;
	this->life = life;
	this->texture = texture;
	this->color = color;
	this->direction = direction;

	plane = App->particle_manager->plane;
}

void Particle::Update(float dt)
{
	if (life > lifeTime)
	{
		position += (direction.Mul(speed * dt));

		CameraOrientation();

		lifeTime += dt;
	}
	else
	{
		emitterpart->particlesList.remove(this);
		App->particle_manager->activeParticles--;
		active = false;
	}

	/*ownRotation.Mul(Quat::RotateZ(rotation));*/

}

void Particle::CameraOrientation()
{

	float3 z = -App->camera->activeCam->frustum.front;
	float3 y = App->camera->activeCam->frustum.up;
	float3 x = y.Cross(z);

	ownRotation.Set(float3x3(x, y, z));

}

void Particle::Draw()
{
	if (plane)
	{
		plane->Draw(float4x4::FromTRS(position, ownRotation, float3(size)), *texture, color);
	}
}