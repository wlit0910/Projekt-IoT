#ifndef GRABBAR_H
#define GRABBAR_H

#include "button.h"


class GrabBar : public Button
{
public:
	GrabBar(std::string name);
	//~GrabBar();

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