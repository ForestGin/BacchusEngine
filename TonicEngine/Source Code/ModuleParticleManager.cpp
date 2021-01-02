#include "ModuleParticleManager.h"



ModuleParticleManager::ModuleParticleManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleParticleManager::~ModuleParticleManager()
{
}

int ModuleParticleManager::GetLastParticle()
{
	for (int i = lastUsedParticle; i < MAX_PARTICLES; i++) {
		if (particles[i].life < 0) {
			lastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < lastUsedParticle; i++) {
		if (particles[i].life < 0) {
			lastUsedParticle = i;
			return i;
		}
	}

	return 0;
}