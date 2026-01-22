#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SDL.h>
#include "texturemgr.h"
#include "panel.h"


class TextBox : public Panel
{
public:
	TextBox(std::string name);
	~TextBox();

	virtual void ReadValue(const KeyValue& kv);
	virtual void OnClick();
	virtual void Update();
	virtual void Draw();

	virtual void OnFocusGained();
	virtual void OnFocusLost();

	virtual void OnSizeChanged();

	virtual void OnMouseButtonPressed();
	virtual void OnMouseButtonReleased();
	virtual void OnMouseButtonReleasedOutside();

	void		 ClearTextInSelection();
	void		 CopyTextInSelection();

	void		 ResetCursorBlink();

	int			 CalculateCursorPos();

	SDL_Point    GetIndexRange();

	int			 GetCursorPos();

	std::string  GetText() const;

	void		 SetDefaultFont();
	void		 SetText(const std::string& new_text);

	void		 SetFont(const std::string& font_name, const std::string& file_path, int font_size, SDL_Color font_color, SDL_Color bg_color);

public:
	bool in_text_input;
protected:
	int  font_id;
	std::string text;
	int	 text_shift;
	std::string text_visible;
	int  cursor_pos;
	int  cursor_height;
	SDL_Rect textRect;

	int select_startpos;
	int select_endpos;
	bool in_mouse_select;

};

#endif