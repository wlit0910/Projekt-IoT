#ifndef LAYERPANEL_H
#define LAYERPANEL_H

#include "panel.h"

//Layer Flags
#define LAYER_HUD           (1 << 0)
#define LAYER_GAMEUI        (1 << 1)
#define LAYER_MENUS         (1 << 2)
#define LAYER_TOOLS         (1 << 3)
#define LAYER_TOOLS_OVERLAY (1 << 4)


class LayerPanel : public Panel
{
public:
	LayerPanel(std::string name);
	//~LayerPanel();

	void OnClick();
	void OnMouseButtonPressed();
	void OnChildMoveToFront();
	
	void Update();

public:
	bool is_background_nonsolid;

private:
	bool move_to_front;

};


#endif