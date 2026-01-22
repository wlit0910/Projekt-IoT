#ifndef FORM_H
#define FORM_H

#include "panel.h"
#include "grabbar.h"
#include "button.h"


class Form : public Panel
{
public:
	Form(std::string name);
	//~Form();

	virtual void OnSizeChanged();

	void SetFormName(const std::string& text);

protected:
	GrabBar* grab_bar;
	Button*  button_close;

};

#endif