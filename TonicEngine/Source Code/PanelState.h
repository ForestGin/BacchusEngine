#ifndef __PANELSTATE_H__
#define __PANELSTATE_H__

#include "Application.h"
#include "PanelManager.h"
#include "TextureImporter.h"

class PanelState : public PanelManager
{
public:
	PanelState();
	~PanelState();

	bool Start();
	bool Draw();

	bool editing = true;

	Texture move;
	Texture rot;
	Texture scale;
};

#endif
