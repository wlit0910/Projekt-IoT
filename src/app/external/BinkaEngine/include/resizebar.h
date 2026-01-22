#ifndef RESIZEBAR_H
#define RESIZEBAR_H

#include "button.h"


class ResizeBar : public Button
{
public:
	ResizeBar(std::string name);
	//~ResizeBar();

	void OnMouseButtonPressed();
	void OnMouseButtonReleased();
	void OnMouseButtonReleasedOutside();
	void OnClick();

	void Update();
	void Draw();


private:
	SDL_Point grab_point;
	bool grabbed;

};

#endif