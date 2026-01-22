#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include "label.h"


class Button : public Label 
{
public:
	Button(std::string name);
	//~Button();

	virtual void ReadValue(const KeyValue& kv);

	virtual void OnMouseButtonPressed();
	virtual void OnMouseButtonReleased();
	virtual void OnMouseButtonReleasedOutside();
	virtual void OnClick();
	
	virtual void Update();
	virtual void Draw();
	virtual void DrawBorder();

	void SetCommandTarget(Panel* target);

	void SetPressedColor(SDL_Color color);
	void SetRegularColor(SDL_Color color);

	void SetPressedTexture(const char* texture_path);
	void SetReleasedTexture(const char* texture_path);


public:
	std::string globalCommand;
	std::string panelCommand;

protected:
	int tex_released_id;
	int tex_pressed_id;

	Panel* command_target;

private:
	SDL_Color pressed_color;
	SDL_Color regular_color;

};

#endif