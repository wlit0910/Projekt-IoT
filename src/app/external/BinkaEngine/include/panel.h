#ifndef PANEL_H
#define PANEL_H

#include <SDL.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "util.h"
#include "cmd.h"

//TODO: Add panel interface class IPanel

class Panel
{
public:
	Panel(std::string name);
	virtual ~Panel();

	virtual void LoadSettings(const char* script);
	virtual void ReadValue(const KeyValue& kv);
	static Panel* CreateControl(const std::string& className);

	bool IsCursorOn();

	virtual void OnClick();

	virtual void Update();
	void		 PostUpdate();
	virtual void Draw();
	virtual void DebugDrawOutline();

	virtual void DrawBorder();

	virtual bool ShouldDraw();

	virtual void UpdateClipRect();

	virtual void DeletePanel();

	std::string GetPanelName() const;
	const char* GetPanelName();

	virtual Panel* GetChildByName(const std::string& name);
	void IncrementVisibleChild();
	bool HasVisibleChild();

	SDL_Rect GetRect(); //In global space
	SDL_Rect& GetClipRect();

	int  GetWidth();
	int  GetHeight();
	int  GetZOrder();

	void SetPos(int x, int y);
	void SetPosAbs(int x, int y);
	void ShiftPos(int x_shift, int y_shift);
	virtual void SetZOrder(int z);
	void SetSize(int w, int h);
	void SetMinSize(int w, int h);
	void SetWidth(int w);
	void SetHeight(int h);
	void SetBackgroundColor(SDL_Color color);
	void CenterOnParent();
	void SetVisible(bool visible);
	bool IsVisible();
	bool IsInteractive();
	virtual void OnBecomeVisible();

	virtual void OnSizeChanged();

	virtual void OnMouseButtonPressed();
	virtual void OnMouseButtonReleased();
	virtual void OnMouseButtonReleasedOutside();
	virtual void OnMousePressedOutside();
	virtual void OnFocusGained();
	virtual void OnFocusLost();

	virtual void OnCommand(const std::string& command);
	
	const SDL_Point& GetPos();
	const SDL_Point& GetAbsPos();
	const SDL_Point& GetSize();
	const SDL_Point& GetMinSize();

	SDL_Point GetRelativeMousePos();

	void UpdateAbsPos();

	virtual std::vector<Panel*>::iterator FindChildIterator(Panel* child);

	virtual void SetParent(Panel* new_parent);
	virtual void MoveToFront();
	virtual void OnChildMoveToFront();
	Panel* GetParent();

	virtual void AddChild(Panel* child);
	virtual void RemoveChild(Panel* child);
	virtual void DeleteChild(Panel* child);

	virtual void AddChildToList(Panel* child);
	virtual void RemoveChildFromList(Panel* child);

public:
	bool is_interactive; //Will move to front when clicked
	bool best_child_found;
	bool children_list_needs_update;
	bool panel_active;
	bool in_focus;
	bool should_draw_background;
	bool should_draw_border;
	bool mark_for_delete;
	bool is_visible;
	bool force_draw;

protected:
	std::string name;

	SDL_Point pos; //Position within parent
	SDL_Point abs_pos; //Position within whole program window
	SDL_Point size; //Width height
	SDL_Point min_size;

	SDL_Rect  clip_rect;
	bool	  area_found;

	SDL_Color background_color;


	Panel* parent; //Pointer to parent panel. For root panel it's NULL.

	Panel* child_focus;

	std::vector<Panel*> children; //Children list in rendering order. Panel on top is last.
	std::vector<Panel*> updated_children; //Exact copy of children, used when we need to sort children vector, but we're doing something with it
	
	int z_order;

	bool panel_pressed;
	bool mouse_click_ready;

	bool list_cleanup;

private:
	bool in_update;
	bool in_draw;
	int visible_child_count;

};

#endif