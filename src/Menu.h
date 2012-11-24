#ifndef MENU_H
#define MENU_H
#include "SDLFramework.h"
#include "Point.h"
#include <vector>
#include "Rectangle.h"
#include "Settings.h"

enum MenuType 
{
	Normal, Button, Option, Text, Slider
};
class Options 
{
private:
	int _r, _g, _b;
	Rectangle _bound;
	bool _hover, _boundSet;
	std::string _text;
public:
	Options();
	Options(std::string text, int r=255, int g=255, int b=255);
	int GetColorR();
	int GetColorG();
	int GetColorB();
	bool IsBoundingBox();
	Rectangle GetBound();
	bool GetHover();
	std::string GetText();
	void SetBound(float x, float y, float width, float height);
	void SetHover(bool value);
	void SetText(std::string text);
	void SetColor(int r, int g, int b);
};
class MenuItem
{
protected:
	MenuType _type;
	std::string _text;
	std::string _header;
	std::vector<MenuItem> _childs;
	Rectangle _bounds;
	Point2D _custom;

	int _r, _g, _b, _maxTextLength;
	
	bool (Settings::*OnClick)(); //ButtonItem
	bool (Settings::*OnOptionClick)(int id); //OptionItem
	bool (Settings::*onTextChange)(std::string text); //TextItem
	std::vector<Options> _options;
	bool _optionBoundSet, _clickEventAssigned, _hover, _customSet, _clickable, _digitOnly, _selected;
public:
	MenuItem();
	MenuItem(std::string title, int r=255, int g=255, int b=255);
	
	void AddChild(MenuItem* item);
	void AddChild(std::string title, int r=255, int g=255, int b=255);
	void AddButtonChild(std::string title, int r=255, int g=255, int b=255);
	void AddButtonChild(std::string title, bool(Settings::*onclick)(), int r=255, int g=255, int b=255);
	void AddOptionChild(std::vector<std::string> options, int r=255, int g=255, int b=255);
	void AddOptionChild(std::vector<std::string> options, bool(Settings::*optionclicks)(int id), int r=255, int g=255, int b=255);
	void AddTextChild(std::string title="", int maxLength=0, bool digit=false, int r=255, int g=255, int b=255);
	void AddTextChild(bool (Settings::*onTextChange)(std::string text), std::string title="", int maxLength=0, bool digit=false, int r=255, int g=255, int b=255);
	bool IsCustomPosition();

	int HandleEvent(SDL_Event sEvent, Settings* setting);
	void Draw(WindowSurface screen, Font font, Point2D offset);

	void SetBoundingBox(float X, float Y, float Width, float Height);
	void SetHover(bool value);
	void SetText(std::string text);
	void SetHeader(std::string text);
	void SetClickable(bool value);
	void SetColor(int r, int g, int b);
	
	int GetMaxLength();
	bool IsSelected();
	void SetSelect(bool value);
	bool IsDigitOnly();

	Rectangle* GetBoundingBox();
	std::string GetText();
	std::string GetHeader();
	int GetColorR();
	int GetColorG();
	int GetColorB();
	Point2D GetCustomPosition();
	MenuType GetType();
	MenuItem* GetChild(unsigned int index);
	int CountChild();
	bool IsEventAssigned();
	bool IsHover();
	bool Clickabe();
	void SetCustomPosition(float x, float y);
};
class ButtonMenuItem : public MenuItem
{
public:
	ButtonMenuItem(std::string title = "", int r=255, int g=255, int b=255);
	ButtonMenuItem(std::string title, bool(Settings::*clickEvent)(), int r=255, int g=255, int b=255);
};
class OptionMenuItem : public MenuItem
{
public:
	OptionMenuItem(std::vector<Options> options, int r=255, int g=255, int b=255);
	OptionMenuItem(std::vector<std::string> options, int r=255, int g=255, int b=255);
	OptionMenuItem(std::vector<std::string> options, bool(Settings::*optionclicks)(int id), int r=255, int g=255, int b=255);
	void SetOptionBound(int index, float x, float y, float w, float h);
	void SetOptionHover(int index, bool value);
	
	int OptionCount();
	
	std::string GetOptionText(int index);
	Rectangle GetOptionBound(int index);
	bool GetOptionHover(int index);
	Options GetOption(int index);
};
class TextMenuItem : public MenuItem
{
public:
	TextMenuItem(std::string title = "", int r=255, int g=255, int b=255);
	TextMenuItem(std::string title="", int maxLength=0, bool digit=false, int r=255, int g=255, int b=255);
	TextMenuItem(bool (Settings::*onTextChange)(std::string text), std::string title="", int maxLength=0, bool digit=false, int r=255, int g=255, int b=255);
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
	Menu(std::string text, Settings* setting, int r=255, int g=255, int b=255);
	void Open(WindowSurface screen, Point2D offset);
	MenuItem* GetChild(unsigned int index);
	
	void AddChild(MenuItem* item);
	void AddChild(std::string text, int r=255, int g=255, int b=255);
	void AddButtonChild(std::string title, int r=255, int g=255, int b=255);
	void AddButtonChild(std::string title, bool(Settings::*onclick)(), int r=255, int g=255, int b=255);
	void AddOptionChild(std::vector<std::string> options, int r=255, int g=255, int b=255);
	void AddOptionChild(std::vector<std::string> options, bool(Settings::*optionclicks)(int id), int r=255, int g=255, int b=255);
	void AddTextChild(std::string title="", int maxLength=0, bool digit=false, int r=255, int g=255, int b=255);
	void AddTextChild(bool (Settings::*onTextChange)(std::string text), std::string title="", int maxLength=0, bool digit=false, int r=255, int g=255, int b=255);
	
	void HandleEvent(SDL_Event sEvent);
	void Reset();
	void Back(unsigned int index = 1);
};
#endif