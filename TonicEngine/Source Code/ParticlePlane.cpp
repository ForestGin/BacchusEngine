#include "ParticlePlane.h"
#include "glew/include/GL/glew.h"
#include "ResourceTexture.h"

ParticlePlane::ParticlePlane()
{
	float indicesQuad[]
	{
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f,
	};

	glGenBuffers(1, (GLuint*)&(indexID));
	glBindBuffer(GL_ARRAY_BUFFER, indexID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, indicesQuad, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int vertices[]
	{
		0, 1, 2,
		1, 3, 2,
	};

	glGenBuffers(1, (GLuint*)&(vertexID));
	glBindBuffer(GL_ARRAY_BUFFER, vertexID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * 6, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	float text[]
	{
		0, 0,
		1, 0,
		0, 1,
		1, 1,
	};

	glGenBuffers(1, (GLuint*)&(uvID));
	glBindBuffer(GL_ARRAY_BUFFER, uvID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, text, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

ParticlePlane::~ParticlePlane()
{

	glDeleteBuffers(1, (GLuint*)&(indexID));
	glDeleteBuffers(1, (GLuint*)&(vertexID));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticlePlane::Draw(float4x4 matrix, ResourceTexture* texture, float4 color)
{
	glPushMatrix();
	glMultMatrixf(matrix.ptr());

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, indexID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexID);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (texture != nullptr)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, uvID);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		glEnable(GL_ALPHA_TEST);

		glBindTexture(GL_TEXTURE_2D, texture->tex.id);
	}
	glColor4f(color.x, color.y, color.z, color.w);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}