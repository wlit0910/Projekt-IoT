#ifndef ROOTPANEL_H
#define ROOTPANEL_H

#include "panel.h"
#include "layerpanel.h"


class RootPanel : public Panel
{
public:
	RootPanel(std::string name);
	//~RootPanel();

	void OnClick();
	
	void Update();

	void DeletePanel();
	void DeleteRootPanel();

	void OnMouseButtonPressed();
	void OnMouseButtonReleased();
	void OnMouseButtonReleasedOutside();

	virtual Panel* GetLayerPanel(Uint32 layerFlag);
	virtual void EnableLayer(Uint32 layer);
	virtual void DisableLayer(Uint32 layer);
	virtual void ToggleLayer(Uint32 layer);

private:
	bool PanelNeedCursor();

private:
	LayerPanel* layerHUD;
	LayerPanel* layerGameUI;
	LayerPanel* layerMenus;
	LayerPanel* layerTools;
	LayerPanel* layerToolsOverlay;

};

extern RootPanel* g_rootPanel;


void Init_Panels();
void Update_Panels();
void Draw_Panels();
void Shutdown_Panels();

#endif