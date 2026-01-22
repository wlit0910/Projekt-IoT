#ifndef CONTEXTMENUBUTTON_H
#define CONTEXTMENUBUTTON_H

#include "button.h"
#include "contextmenu.h"

class ContextMenuButton : public Button
{
public:
	ContextMenuButton(std::string name);
	//~ContextMenuButton();

	void OnClick();
	virtual void Update();
	virtual void OnFocusGained();
	virtual void OnFocusLost();

	void AddContextMenu(Panel* context_menu);

	void OnMousePressedOutside();
	void OnMouseButtonPressed();
	void OnMouseButtonReleased();
	void OnMouseButtonReleasedOutside();

public:
	Panel* context_menu;
	bool   last_menu_state; //True if we opened it false if we closed it.


};

#endif