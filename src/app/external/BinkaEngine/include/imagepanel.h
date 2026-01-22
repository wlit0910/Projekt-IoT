#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

#include <SDL.h>
#include "panel.h"


class ImagePanel : public Panel 
{
public:
	ImagePanel(std::string name);
	~ImagePanel();
	
	void ReadValue(const KeyValue& kv);

	virtual void Update();
	virtual void Draw();

	virtual void SetTexture(const char* texture_path);


protected:
	int texture_id;

};

#endif