#ifndef MENU_H
#define MENU_H
#include "SDLFramework.h"
#include "Point.h"
#include <vector>
#include "Rectangle.h"
#include "Settings.h"

enum MenuType 
{
	Normal, Button, Option
};
class MenuItem
{
protected:
	MenuType _type;
	std::string _text;
	std::string _header;
	std::vector<MenuItem> _childs;
	Rectangle _bounds;
	
	bool (Settings::*OnClick)(); //ButtonItem
	bool (Settings::*OnOptionClick)(int id); //OptionItem
	std::vector<std::string> _options; //OptionItem
	std::vector<int> _optionWidths; //OptionItem
	std::vector<bool> _optionHover; //OptionItem
	
	bool _clickEventAssigned;
	bool _hover;
	bool _clickable;
public:
	MenuItem();
	MenuItem(std::string title);
	
	void AddChild(std::string title);
	void AddChild(MenuItem* item);
	void AddButtonChild(std::string title);
	void AddButtonChild(std::string title, bool(Settings::*onclick)());
	void AddOptionChild(std::vector<std::string> options);
	void AddOptionChild(std::vector<std::string> options, bool(Settings::*optionclicks)(int id));

	int HandleEvent(SDL_Event sEvent, Settings* setting);
	void Draw(WindowSurface screen, Font font, Point2D offset);

	void SetBoundingBox(float X, float Y, float Width, float Height);
	void SetHover(bool value);
	void SetText(std::string text);
	void SetHeader(std::string text);
	void SetClickable(bool value);
	
	Rectangle* GetBoundingBox();
	std::string GetText();
	std::string GetHeader();
	MenuType GetType();
	MenuItem* GetChild(unsigned int index);
	int CountChild();
	bool IsEventAssigned();
	bool IsHover();
	bool Clickabe();
};
class ButtonMenuItem : public MenuItem
{
public:
	ButtonMenuItem();
	ButtonMenuItem(std::string title);
	ButtonMenuItem(std::string title, bool(Settings::*clickEvent)());
};
class OptionMenuItem : public MenuItem
{
public:
	OptionMenuItem(std::vector<std::string> options);
	OptionMenuItem(std::vector<std::string> options, bool(Settings::*optionclicks)(int id));
	void SetOptionWidth(int width);
	void SetOptionHover(int index, bool value);
	
	int OptionCount();
	int OptionWidthCount();
	
	std::string GetOptionText(int index);
	int GetOptionWidth(int index);
	bool GetOptionHover(int index);
};
class Menu
{
private:
	Settings* _setting;
	std::vector<Uint16> _itemTracker;
	MenuItem _mainItem;
	MenuItem* _currentItem;
	Font _font;
public:
	Menu(std::string text, Settings* setting);
	void Open(WindowSurface screen, Point2D offset);
	MenuItem* GetChild(unsigned int index);

	void AddChild(std::string text);
	void AddChild(MenuItem* item);
	void AddButtonChild(std::string title);
	void AddButtonChild(std::string title, bool(Settings::*onclick)());
	void AddOptionChild(std::vector<std::string> options);
	void AddOptionChild(std::vector<std::string> options, bool(Settings::*optionclicks)(int id));

	void HandleEvent(SDL_Event sEvent);
	void Reset();
	void Back(unsigned int index = 1);
};
#endif