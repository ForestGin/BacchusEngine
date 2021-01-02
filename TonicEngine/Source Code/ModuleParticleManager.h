#ifndef __ModuleParticleManager_H__
#define __ModuleParticleManager_H__

#include "Module.h"
#include "ComponentEmitter.h"
#include "Particle.h"
#include <list>

#define MAX_PARTICLES 10000

class ModuleParticleManager : public Module
{
public:
	ModuleParticleManager(Application* app, bool start_enabled = true);
	~ModuleParticleManager();

	int GetLastParticle();

public:
	std::list<ComponentEmitter*> emitters;
	Particle particles[MAX_PARTICLES];

	int lastUsedParticle = 0;
};

#endif