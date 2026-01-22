#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include "button.h"


class ToggleButton : public Button
{
public:
	ToggleButton(std::string name);
	//~ToggleButton();

	void OnClick();
	
	void Update();
	void Draw();

	bool IsOn();
	void SetState(bool on);
	void Toggle();

private:
	bool is_on;

};

#endif