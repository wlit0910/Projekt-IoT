#ifndef RICHTEXTBOX_H
#define RICHTEXTBOX_H

#include <SDL.h>
#include "texturemgr.h"
#include "panel.h"


class RichTextBox : public Panel
{
public:
	RichTextBox(std::string name);
	~RichTextBox();

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

	SDL_Point	 CalculateCursorPos();

	SDL_Point    GetIndexRange();

	SDL_Point    GetIndexRangeForLine(int line);

	size_t		 GetMaxLength();

	int			 GetVisibleLineCount();
	int			 GetLastVisibleLine();

	int			 GetCursorPos();

	std::string  GetText() const;

	void		 SetDefaultFont();
	void		 SetText(const std::string& new_text);
	void		 AppendText(const std::string& new_text);
	void		 ClearText();
	void		 ClearOldLines();
	void		 ScrollToLastLine();

	void		 SetReadOnly(bool read_only);

	void		 SetFont(const std::string& font_name, const std::string& file_path, int font_size, SDL_Color font_color, SDL_Color bg_color);
	void		 SetFontSize(int size);
	void		 SetFontColor(SDL_Color font_color);
	int			 GetFontSize();


	struct TextLines
	{
		std::string text;
		std::string text_visible;
		int  cursor_pos;
	};

public:
	bool in_text_input;
protected:
	bool is_readonly;

	int  font_id;
	int  cursor_height;
	SDL_Rect textRect_base;

	std::vector<TextLines> text_lines;
	int current_line;
	int line_shift;

	int	 text_shift; //This is shared for all lines
	
	SDL_Point select_startpos;
	SDL_Point select_endpos;
	bool in_mouse_select;
};

#endif