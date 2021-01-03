#ifndef __ModuleParticleManager_H__
#define __ModuleParticleManager_H__

#include "Module.h"
#include "ComponentEmitter.h"
#include "Particle.h"
#include <list>
#include "ParticlePlane.h"

#define MAX_PARTICLES 10000

class ModuleParticleManager : public Module
{
public:
	ModuleParticleManager(Application* app, bool start_enabled = true);
	~ModuleParticleManager();

	bool Start();

	int GetLastParticle();
	update_status Update();

	bool Draw();

public:
	std::list<ComponentEmitter*> emitters;
	Particle particles[MAX_PARTICLES];

	int lastUsedParticle = 0;
	int activeParticles = 0;

	ParticlePlane* plane = nullptr;
};

#endif