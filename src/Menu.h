#ifndef MENU_H
#define MENU_H
#include "SDLFramework.h"
#include "Point.h"
#include <vector>
#include "Rectangle.h"
#include "Settings.h"

//All type of menuItems that ara available
enum MenuType 
{
	NormalItem, ButtonItem, OptionItem, TextItem, SliderItem
};
//A simple option (used in an optionMenuItem)
class Options 
{
private:
	//Color of the optionText
	int _r, _g, _b;
	//position en dimensions of the option
	Rectangle _bound;
	//_hover = whether or not the cursor is in the boundingbox, so if it is on hover
	// _boundSet = whether or not the option has a defined dimension and position on the screen
	bool _hover, _boundSet;
	//String (caption) of the option
	std::string _text;
public:
	//Contructor (needed a default contructor for some function)
	Options();
	//Constructor which set the optiontext ant textcollor (default is white)
	Options(std::string text, int r=255, int g=255, int b=255);
	//Returns the red component of the text color
	int GetColorR();
	//Returns the green component of the text color
	int GetColorG();
	//Returns the blue component of the text color
	int GetColorB();
	//Returns whether or not the option has a defined dimension and position
	bool IsBoundingBox();
	//Returns the position and dimension in a rectangle
	Rectangle GetBound();
	//Returns whether or not the cursor hover the option
	bool GetHover();
	//Returns the text of the option
	std::string GetText();
	//Set a new x and y position and dimensions of the option
	void SetBound(float x, float y, float width, float height);
	//Set whether or not the cursor is on hover
	void SetHover(bool value);
	//Set the new option text
	void SetText(std::string text);
	//Set the new text color
	void SetColor(int r, int g, int b);
	//Set if the option is enabled, so it can be pressed
	bool Enabled;
};
//De basic menu item
class MenuItem
{
protected:
	//Type of the menu item
	MenuType _type;
	//Submenuitems of this item
	std::vector<MenuItem> _childs;
	//Boundingbox of the menuitem (position and dimension)
	Rectangle _bounds;
	//A custom position, if it has to be on a different location than usual
	Point2D _custom;

	//These variables had to be in the baseclass, because otherwise they could not fit in a vector of MenuItems (pointer issues)
	//Text of the menuitem if it is an optionMenuItem
	std::string _optionText;
	//(r,g,b) = textcolor
	//MaxLength is used in textMenuItem, to define the maximum amount of character in it
	//Status is used in slider and text, to define if the item is selected
	int _r, _g, _b, _maxLength, _status;
	//Event that occured if != 0 and if the event occured
	bool (Settings::*OnClick)(); //ButtonItem, if cursor clicks on the item
	bool (Settings::*OnOptionClick)(int id); //OptionItem, if cursor clicks on an option, also gives the id of the option
	bool (Settings::*onTextChange)(std::string text); //TextItem, if the text changes
	bool (*onValueChange)(float ratio,void* ref); //SliderItem, if the slider moves, so it changes its value
	void* onValueChangeRef; //callback, to call the special event from the base class
	//List of all the options in a optionMenuItem
	std::vector<Options> _options;
	//_evenAssigned = if event*!=0
	//_hover = whether or not the cursor is on hover
	//_customSet = whether or not there is a different position for this item
	//_digitOnly = whether or not the textItem can only contain numbers
	//_selected = whether or not this item is clicked
	//_headerShown = whether or not this item must be shown if it is the parent
	//_center = whether or not this item must be centered
	bool _optionBoundSet, _clickEventAssigned, _hover, _customSet, _digitOnly, _selected, _headerShown, _center;
	//String of the space between the option (OptionMenuItem)
	std::string _optionSpace;
	//_optionSpaceWidth = amount of pixels between the options, necessary to define if cursor clicks on a item, and hover
	//_verticalSpace = amount of pixels between the menuitems
	int _optionSpaceWidth, _verticalSpace;
public:
	//The base contructor
	MenuItem();
	//The constructor which set the text of the menu item, if it is centered or not, and the text color
	MenuItem(std::string title, bool center=false, int r=255, int g=255, int b=255);
	//Adds an already existing menuItem to this parent
	void AddChild(MenuItem* item);
	//Adds a normal menuItem to this parent
	void AddChild(std::string title, bool center=false, int r=255, int g=255, int b=255);
	//Adds a button menuItem to this parent, you can also set the onclick event
	void AddButtonChild(std::string title, bool center=false, int r=255, int g=255, int b=255,bool(Settings::*onclick)()=0);
	//Adds a option menuItem to this parent, you can also set the onOptionClick event
	//Also you need to give the string that will be the space between the option
	void AddOptionChild(std::vector<std::string> options , std::string optionSpace="", bool center=false, int r=255, int g=255, int b=255,bool(Settings::*optionclicks)(int id)=0);
	//Adds a text menuItem to this parent, with the maximum amount of character, whether or not is is digit only
	//Also you can set the onTextChange event
	void AddTextChild(std::string title="", int maxLength=0, bool digit=false, bool center=false, int r=255, int g=255, int b=255,bool (Settings::*onTextChange)(std::string text)=0);
	//Adds a slider item to this parent, with the dimensions of the slider, and the onvaluechange event
	void AddSliderChild( int width, int height, bool center=false, int r=255, int g=255, int b=255,bool (*onValueChange)(float value,void* pRev)=0,void* pRev=0);
	//Handles all the events, if this item in shown (check if cursor press a menuItem, changes some text, etc
	int HandleEvent(SDL_Event sEvent, Settings* setting);
	//Renders all the MenuItems with the given font on the screen
	//Offset is the place to start drawing, if it is not a centered menuItem
	void Draw(Window screen, Font& font, Point2D offset=Point2D());
	//HoverEnebled = If this menuItem can be hovered
	//IsClickable = If this menuItem can be clicked
	bool HoverEnabled, IsClickable;
	//Text = Text of this menuItem if it is a submenu
	//Header = Text of this menuitem if it is the parent
	std::string Text, Header;
	//Set the position and the dimensions of this menuItem
	void SetBoundingBox(float X, float Y, float Width, float Height);
	//Set whether or not the cursor hovers this item
	void SetHover(bool value);
	//Set the textcolor of this item
	void SetColor(int r, int g, int b);
	//Returns the text of this item
	std::string GetText();
	//Returns whether or not there is a special position for this item
	bool IsCustomPosition();
	//Returns if this item is selected (if cursor pressed on this item)
	bool IsSelected();
	//Set if this item is selected
	void SetSelect(bool value);
	//Returns the position and dimensions of this item
	Rectangle* GetBoundingBox();
	//Return the Red component of the text color
	int GetColorR();
	//Return the Green component of the text color
	int GetColorG();
	//Return the Blue component of the text color
	int GetColorB();
	//Returns the spacial position of this item, if set
	Point2D GetCustomPosition();
	//Returns the type of this item
	MenuType GetType();
	//Returns the pointer of a subitem from the list at the given index
	MenuItem* GetChild(unsigned int index);
	//Returns the amount of subitems
	int CountChild();
	//Returns if this item has an assigned even
	bool IsEventAssigned();
	//Returns if this item is hoverd
	bool IsHover();
	//Set a custom position to this item
	void SetCustomPosition(float x, float y);
	//Returns whether or not this item is digit only
	bool IsDigitOnly();
	//Returns the maximum length of this item
	int GetMaxLength();
	//Set if the header must be shown this is a parent item
	void ShowHeader(bool value);
	//Set the value of the space between the item
	void SetVerticalSpace(int value);
	//Sets whether or not this item must be centered
	void SetCenter(bool value);
};
//Button MenuItem
class ButtonMenuItem : public MenuItem
{
public:
	//Constructor
	ButtonMenuItem(std::string title = "", bool center=false, int r=255, int g=255, int b=255, bool(Settings::*clickEvent)()=0);
};
//Button MenuItem
class OptionMenuItem : public MenuItem
{
public:
	//Constructor, with a list of all the options
	OptionMenuItem(std::vector<Options> options, bool center=false, int r=255, int g=255, int b=255);
	//Constructor, with a list of all the options,and the event
	OptionMenuItem(std::vector<std::string> options, bool center=false, int r=255, int g=255, int b=255,bool(Settings::*optionclicks)(int id)=0);
	//Set the bounding box of the option at index
	void SetOptionBound(int index, float x, float y, float w, float h);
	//Set if the given option at index is hovered
	void SetOptionHover(int index, bool value);
	//Returns the amount of options
	unsigned int OptionCount();
	//Returns the optiontext of the option at index
	std::string GetOptionText(int index);
	//Returns the position and dimensions of the option at index
	Rectangle GetOptionBound(int index);
	//Returns whether or not the option at index is on hover
	bool GetOptionHover(int index);
	//Return a pointer of the option at index in the list
	Options* GetOption(int index);
	//Set the option index , if it is enabled, so clickable
	void SetOptionEnabled(unsigned int index, bool value);
	//Return the spacestring between the options
	std::string GetOptionSpace();
	//Return the with of the space between options
	int GetOptionSpaceWidth();
	//Set the optionspace between the options
	void SetOptionSpace(std::string value);
	//Sets the spacewith between the options
	void SetOptionSpaceWidth(int value);
};
//The Text MenuItem
class TextMenuItem : public MenuItem
{
public:
	//Constructor
	TextMenuItem(std::string title="", int maxLength=10, bool digit=false, bool center=false, int r=255, int g=255, int b=255,bool (Settings::*onTextChange)(std::string text)=0);
};
//The Slider MenuItem
class SliderMenuItem : public MenuItem
{
public:
	//Contructor, with the dimension, and the event
	SliderMenuItem(int width, int height, bool center=false, int r=255, int g=255, int b=255,bool (*onValueChange)(float value,void* pRev)=0,void* pRev=0);
	//Returns the value of the slider 0-1
	float GetRatio();
	//Set the status of the slider, clicked or not?
	void SetStatus(int status);
};
//The Menu, the menuitemhandler
class Menu
{
private:
	//pointer to the settings, to where the events are
	Settings* _setting;
	//List of all parents
	std::vector<Uint16> _itemTracker;
	//The mainitem, where all other items are
	MenuItem _mainItem;
	//The current selected item
	MenuItem* _currentItem;
	//Font _font;
public:
	//Constructor, which set the mainItem
	Menu(std::string text, Settings* setting, int r=255, int g=255, int b=255);
	//Renders the currentItem on the screen
	void Open(Window screen, Font& font, Point2D offset = Point2D());
	//Returns the pointer of the menuitem at index
	MenuItem* GetChild(unsigned int index);
	//Set the vertical space between the items
	void SetVerticalSpace(int space);
	//Set the textcolor of this item
	void SetColor(unsigned int r, unsigned int g, unsigned int b);
	//Sets whether or not this header must be shown
	void ShowHeader(bool value);
	//Sets whether or not this item is centerd
	void SetCenter(bool value);
	//Adds an existing item to mainItem
	void AddChild(MenuItem* item);
	//Adds an normal item to mainItem
	void AddChild(std::string text,bool center=false, int r=255, int g=255, int b=255);
	//Adds an button item to mainItem
	void AddButtonChild(std::string title, bool center=false, int r=255, int g=255, int b=255, bool(Settings::*onclick)()=0);
	//Adds an option item to mainItem
	void AddOptionChild(std::vector<std::string> options, std::string optionSpace="", bool center=false, int r=255, int g=255, int b=255, bool(Settings::*optionclicks)(int id)=0);
	//Adds an text item to mainItem
	void AddTextChild(std::string title="", int maxLength=10, bool digit=false, bool center=false, int r=255, int g=255, int b=255,bool (Settings::*onTextChange)(std::string text)=0);
	//Adds an slider item to mainItem
	void AddSliderChild(int width, int height, bool center=false, int r=255, int g=255, int b=255,bool (*onValueChange)(float value,void* pRev)=0,void* pRev=0);
	//Handles all events of the currentItem, that is shown
	void HandleEvent(SDL_Event sEvent);
	//Returns to the mainItem
	void Reset();
	//Goes back one level, so to the parent of the current item
	void Back(unsigned int index = 1);
};
#endif