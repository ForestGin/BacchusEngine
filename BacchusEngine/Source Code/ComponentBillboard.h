#ifndef __C_BILLBOARD_H__
#define __C_BILLBOARD_H__

#include "Component.h"


//class GameObject;

enum class BILLBOARD_TYPE
{
	NONE = -1,

	SCREEN,
	WORLD,
	AXIAL
};

enum class BILLBOARD_AXIS
{
	X,
	Y,
	Z
};

class ComponentBillboard : public Component
{
public:
	ComponentBillboard(GameObject* GO = nullptr);
	~ComponentBillboard();

	void DrawInspector();

	bool Update();


public:

	BILLBOARD_TYPE billboardtype = BILLBOARD_TYPE::NONE;
	BILLBOARD_AXIS billboardaxis = BILLBOARD_AXIS::X;
};

#endif
