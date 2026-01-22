#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

#include "panel.h"
#include "contextmenubutton.h"

class ContextMenu : public Panel
{
public:
	ContextMenu(std::string name);
	//~ContextMenu();

	virtual void OnFocusGained();
	virtual void OnFocusLost();

	virtual void AddChild(Panel* child);

	virtual void OnCommand(const std::string& command);

	void		 ResizeButtons();

private:
	int button_count;

};

#endif