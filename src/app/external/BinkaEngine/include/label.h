#ifndef LABEL_H
#define LABEL_H

#include <SDL.h>
#include "texturemgr.h"
#include "panel.h"


class Label : public Panel 
{
public:
	Label(std::string name);
	~Label();
	
	virtual void Update();
	virtual void Draw();

	virtual void ReadValue(const KeyValue& kv);

	virtual void OnSizeChanged();

	void		 SetText(const std::string& new_text);
	void		 SetDefaultFont();
	void		 SetFont(const std::string& font_name, const std::string& file_path, int font_size, SDL_Color font_color, SDL_Color bg_color);
	void		 SetFontSize(int size);

	void		 SetDynamic(bool state);
	bool		 IsDynamic();

public:
	bool scale_with_font;
	bool center_text;

protected:
	bool is_dynamic; //If label value changes frequently, we don't use buffer texture. TODO: This is disabled for now
	int  font_id;
	std::string text;
	Texture text_buff_texture;
	SDL_Rect textRect;

};

#endif