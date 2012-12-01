#ifndef MENU_H
#define MENU_H
#include "SDLFramework.h"
#include "Point.h"
#include <vector>
#include "Rectangle.h"
#include "Settings.h"

enum MenuType 
{
	NormalItem, ButtonItem, OptionItem, TextItem, SliderItem
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
	bool Enabled;
};
class MenuItem
{
protected:
	MenuType _type;
	std::vector<MenuItem> _childs;
	Rectangle _bounds;
	Point2D _custom;
	std::string _optionText;
	int _r, _g, _b, _maxLength, _status;
	
	bool (Settings::*OnClick)(); //ButtonItem
	bool (Settings::*OnOptionClick)(int id); //OptionItem
	bool (Settings::*onTextChange)(std::string text); //TextItem
	bool (*onValueChange)(float percentage,void* ref); //SliderItem
	void* onValueChangeRef;
	std::vector<Options> _options;
	bool _optionBoundSet, _clickEventAssigned, _hover, _customSet, _digitOnly, _selected, _headerShown, _center;
	std::string _optionSpace;
	int _optionSpaceWidth, _verticalSpace;
public:
	MenuItem();
	MenuItem(std::string title, bool center=false, int r=255, int g=255, int b=255);
	
	void AddChild(MenuItem* item);
	void AddChild(std::string title, bool center=false, int r=255, int g=255, int b=255);
	
	void AddButtonChild(std::string title, bool center=false, int r=255, int g=255, int b=255);
	void AddButtonChild(std::string title, bool(Settings::*onclick)(), bool center=false, int r=255, int g=255, int b=255);
	
	void AddOptionChild(std::vector<std::string> options, std::string optionSpace="", bool center=false, int r=255, int g=255, int b=255);
	void AddOptionChild(std::vector<std::string> options, bool(Settings::*optionclicks)(int id), std::string optionSpace="", bool center=false, int r=255, int g=255, int b=255);
	
	void AddTextChild(std::string title="", int maxLength=0, bool digit=false, bool center=false, int r=255, int g=255, int b=255);
	void AddTextChild(bool (Settings::*onTextChange)(std::string text), std::string title="", int maxLength=0, bool digit=false, bool center=false, int r=255, int g=255, int b=255);
	
	void AddSliderChild( int width, int height, bool center=false, int r=255, int g=255, int b=255,bool (*onValueChange)(float value,void* pRev)=0,void* pRev=0);
	
	int HandleEvent(SDL_Event sEvent, Settings* setting);
	void Draw(WindowSurface screen, Font font, Point2D offset=Point2D());

	bool HoverEnabled, IsClickable;
	std::string Text, Header;

	void SetBoundingBox(float X, float Y, float Width, float Height);
	void SetHover(bool value);
	void SetColor(int r, int g, int b);
	std::string GetText();
	bool IsCustomPosition();
	bool IsSelected();
	void SetSelect(bool value);

	Rectangle* GetBoundingBox();
	int GetColorR();
	int GetColorG();
	int GetColorB();
	Point2D GetCustomPosition();
	MenuType GetType();
	MenuItem* GetChild(unsigned int index);
	int CountChild();
	bool IsEventAssigned();
	bool IsHover();
	void SetCustomPosition(float x, float y);
	bool IsDigitOnly();
	int GetMaxLength();
	void ShowHeader(bool value){
		_headerShown = value;
	}
	void SetVerticalSpace(int value){_verticalSpace = value; }
	void SetCenter(bool value){_center = value; }
};
class ButtonMenuItem : public MenuItem
{
public:
	ButtonMenuItem(std::string title = "", bool center=false, int r=255, int g=255, int b=255);
	ButtonMenuItem(std::string title, bool(Settings::*clickEvent)(), bool center=false, int r=255, int g=255, int b=255);
};
class OptionMenuItem : public MenuItem
{
public:
	OptionMenuItem(std::vector<Options> options, bool center=false, int r=255, int g=255, int b=255);
	OptionMenuItem(std::vector<std::string> options, bool center=false, int r=255, int g=255, int b=255);
	OptionMenuItem(std::vector<std::string> options, bool(Settings::*optionclicks)(int id), bool center=false, int r=255, int g=255, int b=255);
	void SetOptionBound(int index, float x, float y, float w, float h);
	void SetOptionHover(int index, bool value);
	
	unsigned int OptionCount();
	
	std::string GetOptionText(int index);
	Rectangle GetOptionBound(int index);
	bool GetOptionHover(int index);
	Options* GetOption(int index);
	void SetOptionEnabled(unsigned int index, bool value){
		if(index < _options.size())
			_options.at(index).Enabled = value;
	}
	std::string GetOptionSpace(){
		if(_optionSpace != "") return _optionSpace;
		return " ";
	}
	int GetOptionSpaceWidth(){return _optionSpaceWidth; }
	void SetOptionSpace(std::string value){
		_optionSpace = value;
	}
	void SetOptionSpaceWidth(int value){_optionSpaceWidth = value;}
};
class TextMenuItem : public MenuItem
{
public:
	TextMenuItem(std::string title = "", bool center=false, int r=255, int g=255, int b=255);
	TextMenuItem(std::string title, int maxLength=0, bool digit=false, bool center=false, int r=255, int g=255, int b=255);
	TextMenuItem(bool (Settings::*onTextChange)(std::string text), std::string title="", int maxLength=0, bool digit=false, bool center=false, int r=255, int g=255, int b=255);
};
class SliderMenuItem : public MenuItem
{
public:
	SliderMenuItem(int width, int height, bool center=false, int r=255, int g=255, int b=255,bool (*onValueChange)(float value,void* pRev)=0,void* pRev=0);
	float GetPercentage();
	void SetStatus(int status);
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
	void Open(WindowSurface screen, Point2D offset = Point2D());
	MenuItem* GetChild(unsigned int index);

	void SetVerticalSpace(int space){
		_mainItem.SetVerticalSpace(space);
	}
	void SetColor(unsigned int r, unsigned int g, unsigned int b){
		if(r>255)r=255;
		if(g>255)g=255;
		if(b>255)b=255;
		_font.SetColor(r, g, b);
	}
	void ShowHeader(bool value){ _mainItem.ShowHeader(value); }
	void SetCenter(bool value){_mainItem.SetCenter(value); }
	void AddChild(MenuItem* item);
	void AddChild(std::string text,bool center=false, int r=255, int g=255, int b=255);
	
	void AddButtonChild(std::string title, bool center=false, int r=255, int g=255, int b=255);
	void AddButtonChild(std::string title, bool(Settings::*onclick)(), bool center=false, int r=255, int g=255, int b=255);
	
	void AddOptionChild(std::vector<std::string> options, std::string optionSpace="", bool center=false, int r=255, int g=255, int b=255);
	void AddOptionChild(std::vector<std::string> options, bool(Settings::*optionclicks)(int id), std::string optionSpace="", bool center=false, int r=255, int g=255, int b=255);
	
	void AddTextChild(std::string title="", int maxLength=10, bool digit=false, bool center=false, int r=255, int g=255, int b=255);
	void AddTextChild(bool (Settings::*onTextChange)(std::string text), std::string title="", int maxLength=10, bool digit=false, bool center=false, int r=255, int g=255, int b=255);
	
	void AddSliderChild(int width, int height, bool center=false, int r=255, int g=255, int b=255,bool (*onValueChange)(float value,void* pRev)=0,void* pRev=0);

	void HandleEvent(SDL_Event sEvent);
	void Reset();
	void Back(unsigned int index = 1);
};
#endif