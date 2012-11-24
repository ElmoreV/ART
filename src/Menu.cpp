#include "Menu.h"
#include <sstream>
ButtonMenuItem::ButtonMenuItem():MenuItem(){
	_type = Button;
	_clickEventAssigned = false; 
}
ButtonMenuItem::ButtonMenuItem(std::string title):MenuItem(title){
	_type = Button;
	_clickEventAssigned = false;
}
ButtonMenuItem::ButtonMenuItem(std::string title, bool(Settings::*clickEvent)()):MenuItem(title){
	OnClick = clickEvent;
	_type = Button; 
	_clickEventAssigned = true;
}

OptionMenuItem::OptionMenuItem(std::vector<std::string> options):MenuItem(){
	_type=Option;
	_text = "";
	for(unsigned int i = 0; i < options.size(); i++){
		_text += options.at(i) + " ";
		_options.push_back(options.at(i));
		_optionHover.push_back(false);
	}
	_clickEventAssigned = false;
}
OptionMenuItem::OptionMenuItem(std::vector<std::string> options, bool(Settings::*optionclicks)(int id)){
	_type=Option;
	_text = "";
	for(unsigned int i = 0; i < options.size(); i++){
		_text += options.at(i) + " ";
		_options.push_back(options.at(i));
		_optionHover.push_back(false);
	}
	OnOptionClick = optionclicks;
	_clickEventAssigned = true;
}
void OptionMenuItem::SetOptionWidth(int width){ _optionWidths.push_back(width); }
int OptionMenuItem::OptionCount(){return _options.size(); }
int OptionMenuItem::OptionWidthCount(){ return _optionWidths.size(); }
std::string OptionMenuItem::GetOptionText(int index){ return _options.at(index); }
int OptionMenuItem::GetOptionWidth(int index){ return _optionWidths.at(index); }
void OptionMenuItem::SetOptionHover(int index, bool value){ _optionHover.at(index) = value; }
bool OptionMenuItem::GetOptionHover(int index) { return _optionHover.at(index); }

MenuItem::MenuItem(){
	_text = _header = "UNDEFINED";
	_clickable = false;
	_type = Normal;
	_clickEventAssigned = false;
}
MenuItem::MenuItem(std::string title){
	_text = _header = title;
	_clickable = false;
	_type = Normal;
	_clickEventAssigned = false;
}
void MenuItem::AddChild(std::string title){
	if(_type == Normal) _clickable = true;
	_childs.push_back(MenuItem(title));
}
void MenuItem::AddChild(MenuItem* item){
	if(_type == Normal) _clickable = true;
	_childs.push_back(*item);
}
void MenuItem::AddButtonChild(std::string title){
	if(_type == Normal)_clickable=true;
	_childs.push_back(ButtonMenuItem(title));
}
void MenuItem::AddButtonChild(std::string title, bool(Settings::*onclick)()){
	if(_type == Normal)_clickable=true;
	_childs.push_back(ButtonMenuItem(title, onclick));
}
void MenuItem::AddOptionChild(std::vector<std::string> options){
	if(_type == Normal)_clickable=true;
	_childs.push_back(OptionMenuItem(options));
}
void MenuItem::AddOptionChild(std::vector<std::string> options, bool(Settings::*optionclicks)(int id)){
	if(_type == Normal)_clickable=true;
	_childs.push_back(OptionMenuItem(options, optionclicks));
}

int MenuItem::HandleEvent(SDL_Event sEvent, Settings* setting){
	bool onClickItem = false;
	if(sEvent.type == SDL_MOUSEBUTTONDOWN){
		if(_bounds.Contains(sEvent.button.x, sEvent.button.y)) return -1;
		else {
			for(unsigned int i = 0; i < _childs.size(); i++){
				MenuItem child = _childs.at(i);
				Rectangle rec = *child.GetBoundingBox();
				if(rec.Contains(sEvent.button.x, sEvent.button.y)){
					onClickItem = true;
						switch(child.GetType()){
						case Normal:
							if(child.Clickabe()) return i;
							break;
						case Button:
							if(child.IsEventAssigned())
								if((setting->*child.OnClick)())
									return i;
							break;
						case Option:
							if(child.IsEventAssigned()){
								OptionMenuItem* oItem = (OptionMenuItem*)&child;
								float offsetX = rec.X;
								for(int j = 0; j < oItem->OptionCount(); j++){
									if(offsetX <= sEvent.button.x && offsetX + oItem->GetOptionWidth(j) >= sEvent.button.x){
										if((setting->*child.OnOptionClick)(j))
											return i;
									}
									offsetX += oItem->GetOptionWidth(j);
								}
							}
							break;
					}
				}
			}
		}
		return -2;
	}
	else if(sEvent.type == SDL_MOUSEMOTION || onClickItem){
		if(_bounds.Contains(sEvent.button.x, sEvent.button.y)){
			_hover = true;
			for(unsigned int i = 0; i < _childs.size(); i++){
				_childs.at(i).SetHover(false);
			}
		}
		else{
			_hover=false;
			for(unsigned int i = 0; i < _childs.size(); i++){
				if(_childs.at(i).GetBoundingBox()->Contains(sEvent.button.x, sEvent.button.y)){
					_childs.at(i).SetHover(true);
					if(_childs.at(i).GetType() == Option){
						OptionMenuItem* oItem = (OptionMenuItem*)&_childs.at(i);
						float offsetX = oItem->GetBoundingBox()->X;
						for(int j = 0; j < oItem->OptionCount(); j++){
							if(offsetX <= sEvent.button.x && offsetX + oItem->GetOptionWidth(j) >= sEvent.button.x)
								oItem->SetOptionHover(j, true);
							else
								oItem->SetOptionHover(j, false);
							offsetX += oItem->GetOptionWidth(j);
						}
					}
				}
				else _childs.at(i).SetHover(false);
			}
		}

	}
	return -3;
}
bool MenuItem::Clickabe() { return _clickable; }
MenuType MenuItem::GetType() { return _type; }
Rectangle* MenuItem::GetBoundingBox(){return &_bounds;}
int MenuItem::CountChild(){
	return _childs.size();
}
MenuItem* MenuItem::GetChild(unsigned int index){
	if(index<_childs.size()){
		return &_childs.at(index);
	}
	else return false;
}
void MenuItem::Draw(WindowSurface screen, Font font, Point2D offset){
	float y = offset.Y;
	float x = offset.X;

	Surface render; render.RenderText(font, _header); render.Draw(screen, (int)x, (int)y);
	SetBoundingBox(x, y, (float)render.GetWidth(), (float)render.GetHeight());
	y += render.GetHeight(); 
	if(IsHover()) screen.DrawLine((int)x, (int)y, (int)(x + render.GetWidth()), (int)y, 255, 255, 255);
	x+=5;

	for(unsigned int i = 0; i < _childs.size(); i++){
		MenuItem* item = &_childs.at(i);
		render.RenderText(font, " ");
		int spaceWidth = render.GetWidth();
		render.RenderText(font, item->GetText());
		item->SetBoundingBox(x, y, (float)render.GetWidth(), (float)render.GetHeight());
		render.Draw(screen, (int)x, (int)y);y += render.GetHeight() + 1; 
		if(item->GetType() == Option){
			OptionMenuItem* oItem = (OptionMenuItem*)item;
			if(oItem->OptionWidthCount() == 0){
				for(int j = 0; j < oItem->OptionCount(); j++){
					render.RenderText(font, oItem->GetOptionText(j));
					oItem->SetOptionWidth((int)(render.GetWidth() + spaceWidth));
				}
			}
			if(item->IsHover()){
				float offsetX = x;
				for(int j = 0; j < oItem->OptionCount(); j++){
					if(oItem->GetOptionHover(j)){
						screen.DrawLine((int)offsetX, (int)y, (int)(offsetX + oItem->GetOptionWidth(j)), (int)y, 255, 255, 255);
						break;
					}
					offsetX += oItem->GetOptionWidth(j);
				}
			}
		}
		else if(item->IsHover())
			screen.DrawLine((int)x, (int)y, (int)(x + render.GetWidth()), (int)y, 255, 255, 255);
	}
};
void MenuItem::SetBoundingBox(float x, float y, float width, float height){
	_bounds.X = x;
	_bounds.Y = y;
	_bounds.W = width;
	_bounds.H = height;
}
std::string MenuItem::GetText(){return _text;}
bool MenuItem::IsEventAssigned(){return _clickEventAssigned; }
void MenuItem::SetHover(bool value){_hover =value;}
bool MenuItem::IsHover(){return _hover;}
void MenuItem::SetText(std::string text){if(_type != Option) _text = text; }
void MenuItem::SetHeader(std::string text){_header = text; }
std::string MenuItem::GetHeader(){return _header;}
void MenuItem::SetClickable(bool value){_clickable = value; }

Menu::Menu(std::string text, Settings* setting){
	_mainItem = MenuItem(text);
	_currentItem = &_mainItem;
	_font.OpenFont("Another.ttf");
	_font.SetColor(255, 255, 255);
	_setting = setting;
}
void Menu::Open(WindowSurface screen, Point2D offset){
	if(_itemTracker.size() == 0) _currentItem = &_mainItem;
	_currentItem->Draw(screen, _font, offset);
}
MenuItem* Menu::GetChild(unsigned int index){
	return _mainItem.GetChild(index);
}
void Menu::AddChild(std::string text){
	_mainItem.AddChild(text);
}
void Menu::AddChild(MenuItem* item){
	_mainItem.AddChild(item);
}
void Menu::AddButtonChild(std::string title){
	_mainItem.AddButtonChild(title);
}
void Menu::AddButtonChild(std::string title, bool(Settings::*onclick)()){
	_mainItem.AddButtonChild(title, onclick);
}
void Menu::AddOptionChild(std::vector<std::string> options){
	_mainItem.AddOptionChild(options);
}
void Menu::AddOptionChild(std::vector<std::string> options, bool(Settings::*optionclicks)(int id)){
	_mainItem.AddOptionChild(options, optionclicks);
}
void Menu::HandleEvent(SDL_Event sEvent){
	int id = _currentItem->HandleEvent(sEvent, _setting);
	if(id == -3 || id == -2) return;
	else if(id == -1)
	{
		if(_itemTracker.size() > 0) Back();
	}
	else
	{
		_itemTracker.push_back(id);
		_currentItem = _currentItem->GetChild(id);
	}
}
void Menu::Reset(){
	_itemTracker.clear();
	_currentItem = &_mainItem;
}
void Menu::Back(unsigned int index){
	for(unsigned int i = 0; i < index; i++){
		if(_itemTracker.size() > 0)
			_itemTracker.pop_back();
	}
	_currentItem = &_mainItem;
	for(unsigned int i = 0; i < _itemTracker.size(); i++)
		_currentItem = _currentItem->GetChild(_itemTracker.at(i));
}