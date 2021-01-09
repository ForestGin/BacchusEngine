#ifndef __ParticlePlane_H__
#define __ParticlePlane_H__

#include "Math.h"

class ResourceTexture;

class ParticlePlane
{
public:
	ParticlePlane();
	~ParticlePlane();

	void Draw(float4x4 matrix, ResourceTexture* texture, float4 color);

public:
	unsigned int indexID = 0u;
	unsigned int vertexID = 0u;
	unsigned int uvID = 0u;
};

#endif
