#include "Menu.h"
#include <sstream>

Options::Options(){
	_text = "";
	_r=_g=_b=255;
	_hover=false;
	_bound = Rectangle(0,0,0,0);
}
Options::Options(std::string text,int r, int g, int b){
	_text = text;
	_r=r;_g=g;_b=b;
	_hover=false;
	_bound = Rectangle(0,0,0,0);
}
int Options::GetColorR(){return _r;}
int Options::GetColorG(){return _g;}
int Options::GetColorB(){return _b;}
Rectangle Options::GetBound(){return _bound;}
bool Options::GetHover(){return _hover;}
std::string Options::GetText(){return _text;}
void Options::SetBound(float x, float y, float width, float height){
	_boundSet=true;
	if(x>=0)_bound.X=x;
	if(y>=0)_bound.Y=y;
	if(width>=0)_bound.W=width;
	if(height>=0)_bound.H=height;
}
void Options::SetHover(bool value){_hover=value;}
void Options::SetText(std::string text){_text=text;}
void Options::SetColor(int r, int g, int b){
	if(r!=-1)_r=r;
	if(g!=-1)_g=g;
	if(b!=-1)_b=b;
}
bool Options::IsBoundingBox(){return _boundSet;}

ButtonMenuItem::ButtonMenuItem(std::string title, int r, int g, int b):MenuItem(title, r, g, b){
	_type = Button;
	_clickEventAssigned = false;
}
ButtonMenuItem::ButtonMenuItem(std::string title, bool(Settings::*clickEvent)(), int r, int g, int b):MenuItem(title, r, g, b){
	OnClick = clickEvent;
	_type = Button; 
	_clickEventAssigned = true;
}

OptionMenuItem::OptionMenuItem(std::vector<Options> options, int r, int g, int b){
	_type=Option;
	_text = "";
	for(unsigned int i = 0; i < options.size(); i++){
		_text += options.at(i).GetText() + " ";
		_options.push_back(options.at(i));
	}
	_clickEventAssigned = false;
	_r=r;_g=g;_b=b;
}
OptionMenuItem::OptionMenuItem(std::vector<std::string> options, int r, int g, int b):MenuItem(){
	_type=Option;
	_text = "";
	for(unsigned int i = 0; i < options.size(); i++){
		_text += options.at(i) + " ";
		_options.push_back(Options(options.at(i)));
	}
	_clickEventAssigned = false;
	_r=r;_g=g;_b=b;
}
OptionMenuItem::OptionMenuItem(std::vector<std::string> options, bool(Settings::*optionclicks)(int id), int r, int g, int b){
	_type=Option;
	_text = "";
	for(unsigned int i = 0; i < options.size(); i++){
		_text += options.at(i) + " ";
		_options.push_back(Options(options.at(i)));
	}
	OnOptionClick = optionclicks;
	_clickEventAssigned = true;
	_r=r;_g=g;_b=b;
}

int OptionMenuItem::OptionCount(){return _options.size(); }
std::string OptionMenuItem::GetOptionText(int index){ return _options.at(index).GetText(); }
Rectangle OptionMenuItem::GetOptionBound(int index){ return _options.at(index).GetBound(); }
bool OptionMenuItem::GetOptionHover(int index) { return _options.at(index).GetHover(); }
void OptionMenuItem::SetOptionHover(int index, bool value){ _options.at(index).SetHover(value); }
void OptionMenuItem::SetOptionBound(int index, float x, float y, float w, float h){_options.at(index).SetBound(x, y, w, h); }
Options OptionMenuItem::GetOption(int index){return _options.at(index); }

TextMenuItem::TextMenuItem(std::string text, int r, int g, int b):MenuItem(text,r,g,b){
	_type=Text;
	_maxTextLength = 10;
	_digitOnly = false;
	SDL_EnableUNICODE(SDL_ENABLE);
}
TextMenuItem::TextMenuItem(std::string text, int maxLength, bool digit, int r, int g, int b):MenuItem(text,r,g,b){
	_type=Text;
	_maxTextLength = maxLength;
	_digitOnly = digit;
	SDL_EnableUNICODE(SDL_ENABLE);
}
TextMenuItem::TextMenuItem(bool (Settings::*ontextchange)(std::string text), std::string text, int maxLength, bool digit, int r, int g, int b):MenuItem(text,r,g,b){
	_type=Text;
	_maxTextLength = maxLength;
	_digitOnly = digit;
	SDL_EnableUNICODE(SDL_ENABLE);
	onTextChange = ontextchange;
}

MenuItem::MenuItem(){
	_text = _header = "UNDEFINED";
	_clickable = false;
	_type = Normal;
	_clickEventAssigned = false;
	_r = _g = _b = 255;
	_optionBoundSet=false;
	_customSet = false;
}
MenuItem::MenuItem(std::string title, int r, int g, int b){
	_text = _header = title;
	_clickable = false;
	_type = Normal;
	_clickEventAssigned = false;
	_r=r;_g=g;_b=b;
	_optionBoundSet=false;
	_customSet = false;
}
void MenuItem::AddChild(std::string title, int r, int g, int b){
	if(_type == Normal) _clickable = true;
	_childs.push_back(MenuItem(title, r, g, b));
}
void MenuItem::AddChild(MenuItem* item){
	if(_type == Normal) _clickable = true;
	_childs.push_back(*item);
}
void MenuItem::AddButtonChild(std::string title, int r, int g, int b){
	if(_type == Normal)_clickable=true;
	_childs.push_back(ButtonMenuItem(title, r, g, b));
}
void MenuItem::AddButtonChild(std::string title, bool(Settings::*onclick)(), int r, int g, int b){
	if(_type == Normal)_clickable=true;
	_childs.push_back(ButtonMenuItem(title, onclick, r, g, b));
}
void MenuItem::AddOptionChild(std::vector<std::string> options, int r, int g, int b){
	if(_type == Normal)_clickable=true;
	_childs.push_back(OptionMenuItem(options, r, g, b));
}
void MenuItem::AddOptionChild(std::vector<std::string> options, bool(Settings::*optionclicks)(int id), int r, int g, int b){
	if(_type == Normal)_clickable=true;
	_childs.push_back(OptionMenuItem(options, optionclicks, r, g, b));
}
void MenuItem::AddTextChild(std::string title, int maxLength, bool digit, int r, int g, int b){
	if(_type == Normal)_clickable=true;
	_childs.push_back(TextMenuItem(title, maxLength, digit, r, g, b));
}
void MenuItem::AddTextChild(bool (Settings::*onTextChange)(std::string text), std::string title, int maxLength, bool digit, int r, int g, int b){
	if(_type == Normal)_clickable=true;
	_childs.push_back(TextMenuItem(onTextChange, title, maxLength, digit, r, g, b));
}


int MenuItem::HandleEvent(SDL_Event sEvent, Settings* setting){
	bool onClickItem = false;
	if(sEvent.type == SDL_MOUSEBUTTONDOWN){
		if(_bounds.Contains(sEvent.button.x, sEvent.button.y)) return -1;
		else {
			for(unsigned int i = 0; i < _childs.size(); i++){
				MenuItem* child = &_childs.at(i);
				Rectangle rec = *child->GetBoundingBox();
				if(rec.Contains(sEvent.button.x, sEvent.button.y)){
					onClickItem = true;
					child->SetSelect(true);
					
						switch(child->GetType()){
						case Normal:
							if(child->Clickabe()) return i;
							break;
						case Button:
							if(child->IsEventAssigned())
								if((setting->*child->OnClick)())
									return i;
							break;
						case Option:
							if(child->IsEventAssigned()){
								OptionMenuItem* oItem = (OptionMenuItem*)child;
								float offsetX = rec.X;
								for(int j = 0; j < oItem->OptionCount(); j++){
									if(offsetX <= sEvent.button.x && offsetX + oItem->GetOptionBound(j).W >= sEvent.button.x){
										if((setting->*child->OnOptionClick)(j))
											return i;
									}
									offsetX += oItem->GetOptionBound(j).W + 7;
								}
							}
							break;
						case Slider:
							break;
					}
				}
				else child->SetSelect(false);
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
							if(offsetX <= sEvent.button.x && offsetX + oItem->GetOptionBound(j).W >= sEvent.button.x)
								oItem->SetOptionHover(j, true);
							else
								oItem->SetOptionHover(j, false);
							offsetX += oItem->GetOptionBound(j).W + 7;
						}
					}
				}
				else _childs.at(i).SetHover(false);
			}
		}

	}
	if(sEvent.type == SDL_KEYDOWN){
		for(unsigned int i = 0; i < _childs.size(); i++){
			MenuItem* item = &_childs.at(i);
			if(item->IsSelected() && item->GetType() == Text){
				std::string str = item->GetText();
				SDLKey key = sEvent.key.keysym.sym;
				if(key == SDLK_BACKSPACE){
					if (str.size () > 0){
						str.resize (str.size () - 1);
					}
					if(str.size() == 0) str = " ";
						item->SetText(str);
				}
				else if(key == SDLK_KP_ENTER || key == SDLK_RETURN){
					item->SetSelect(false);
				}
				else {
					if(str.size() < item->GetMaxLength()){
						char v = sEvent.key.keysym.unicode;
						if(!item->IsDigitOnly() || (item->IsDigitOnly() && sEvent.key.keysym.unicode >= 48 && sEvent.key.keysym.unicode <= 57)){
							if(str != " ") str += v;
							else str = v;
							item->SetText(str);
						}
					}
				}
				if(item->IsEventAssigned())
					if((setting->*item->onTextChange)(str))
							return i;
			}
		}
	}
	return -3;
}
void MenuItem::Draw(WindowSurface screen, Font font, Point2D offset){
	float y = offset.Y;
	float x = offset.X;

	Surface render; 
	font.SetColor(_r, _g, _b);
	render.RenderText(font, _header); render.Draw(screen, (int)x, (int)y);
	SetBoundingBox(x, y, (float)render.GetWidth(), (float)render.GetHeight());
	y += render.GetHeight(); 
	if(IsHover()) screen.DrawLine((int)x, (int)y, (int)(x + render.GetWidth()), (int)y, _r, _g, _b);
	x+=5;

	for(unsigned int i = 0; i < _childs.size(); i++){
		MenuItem* item = &_childs.at(i);
		render.RenderText(font, " "); int spaceWidth = render.GetWidth();
		font.SetColor(item->GetColorR(), item->GetColorG(), item->GetColorB());
		render.RenderText(font, item->GetText());
		if(item->IsCustomPosition()){
			item->SetBoundingBox(item->GetCustomPosition().X, item->GetCustomPosition().Y, (float)render.GetWidth(), (float)render.GetHeight());
		}
		else{
			item->SetBoundingBox(x, y, (float)render.GetWidth(), (float)render.GetHeight());
		}

		render.Draw(screen, item->GetBoundingBox()->X, item->GetBoundingBox()->Y);y += render.GetHeight() + 1; 
		
		if(item->GetType() == Option){
			OptionMenuItem* oItem = (OptionMenuItem*)item;
			if(!_optionBoundSet){
				_optionBoundSet=true;
				float offsetX = item->GetBoundingBox()->X;
				for(int j = 0; j < oItem->OptionCount(); j++){
					font.SetColor(oItem->GetOption(j).GetColorR(), oItem->GetOption(j).GetColorG(), oItem->GetOption(j).GetColorB()); 
					render.RenderText(font, oItem->GetOptionText(j));
					oItem->SetOptionBound(j, offsetX, item->GetBoundingBox()->Y, render.GetWidth(), render.GetHeight());
					offsetX += oItem->GetOptionBound(j).W + spaceWidth;
				}
			}
			if(item->IsHover()){
				float offsetX = item->GetBoundingBox()->X;
				for(int j = 0; j < oItem->OptionCount(); j++){
					if(oItem->GetOptionHover(j)){
						screen.DrawLine((int)offsetX, (int)(item->GetBoundingBox()->Y + item->GetBoundingBox()->H), 
							(int)(offsetX + oItem->GetOptionBound(j).W), (int)(item->GetBoundingBox()->Y + item->GetBoundingBox()->H), oItem->GetOption(j).GetColorR(), oItem->GetOption(j).GetColorG(), oItem->GetOption(j).GetColorB());
						break;
					}
					offsetX += oItem->GetOptionBound(j).W + spaceWidth;
				}
			}
		}
		else if(item->IsHover()){
			Rectangle* b(item->GetBoundingBox());
			screen.DrawLine((int)b->X, (int)(b->Y+b->H), (int)(b->X + b->W), (int)(b->Y+b->H), item->GetColorR(), item->GetColorG(), item->GetColorB());
		}
	}
};

int MenuItem::GetMaxLength(){return _maxTextLength; }
bool MenuItem::IsSelected(){return _selected;}
void MenuItem::SetSelect(bool value){_selected=value;}
bool MenuItem::IsDigitOnly(){return _digitOnly;}
MenuItem* MenuItem::GetChild(unsigned int index){
	if(index<_childs.size()){
		return &_childs.at(index);
	}
	else return false;
}
MenuType MenuItem::GetType() { return _type; }
Rectangle* MenuItem::GetBoundingBox(){return &_bounds;}
std::string MenuItem::GetText(){return _text;}
std::string MenuItem::GetHeader(){return _header;}
bool MenuItem::IsEventAssigned(){return _clickEventAssigned; }
bool MenuItem::IsHover(){return _hover;}
bool MenuItem::Clickabe() { return _clickable; }
int MenuItem::CountChild(){ return _childs.size(); }
int MenuItem::GetColorR(){return _r;}
int MenuItem::GetColorG(){return _g;}
int MenuItem::GetColorB(){return _b;}
bool MenuItem::IsCustomPosition(){return _customSet; }
Point2D MenuItem::GetCustomPosition(){return _custom; }
void MenuItem::SetHover(bool value){_hover =value;}
void MenuItem::SetText(std::string text){if(_type != Option) _text = text; }
void MenuItem::SetHeader(std::string text){_header = text; }
void MenuItem::SetClickable(bool value){_clickable = value; }
void MenuItem::SetBoundingBox(float x, float y, float width, float height){
	if(x>=0)_bounds.X = x;
	if(y>=0)_bounds.Y = y;
	if(width>=0)_bounds.W = width;
	if(height>=0)_bounds.H = height;
}
void MenuItem::SetColor(int r, int g, int b) { _r = r; _g = g; _b = b; }
void MenuItem::SetCustomPosition(float x, float y){ _customSet=true;_custom = Point2D(x, y); }

Menu::Menu(std::string text, Settings* setting, int r, int g, int b){
	_mainItem = MenuItem(text, r, g, b);
	_currentItem = &_mainItem;
	_font.OpenFont("Another.ttf");
	_font.SetColor(r, g, b);
	_setting = setting;
}
void Menu::Open(WindowSurface screen, Point2D offset){
	if(_itemTracker.size() == 0) _currentItem = &_mainItem;
	_currentItem->Draw(screen, _font, offset);
}
MenuItem* Menu::GetChild(unsigned int index){
	return _mainItem.GetChild(index);
}

void Menu::AddChild(std::string text, int r, int g, int b){
	_mainItem.AddChild(text, r, g, b);
}
void Menu::AddChild(MenuItem* item){
	_mainItem.AddChild(item);
}
void Menu::AddButtonChild(std::string title, int r, int g, int b){
	_mainItem.AddButtonChild(title, r, g, b);
}
void Menu::AddButtonChild(std::string title, bool(Settings::*onclick)(), int r, int g, int b){
	_mainItem.AddButtonChild(title, onclick, r, g, b);
}
void Menu::AddOptionChild(std::vector<std::string> options, int r, int g, int b){
	_mainItem.AddOptionChild(options, r, g, b);
}
void Menu::AddOptionChild(std::vector<std::string> options, bool(Settings::*optionclicks)(int id), int r, int g, int b){
	_mainItem.AddOptionChild(options, optionclicks,r,g,b);
}
void Menu::AddTextChild(std::string title, int maxLength, bool digit, int r, int g, int b){
	_mainItem.AddTextChild(title, maxLength, digit, r, g, b);
}
void Menu::AddTextChild(bool (Settings::*onTextChange)(std::string text), std::string title, int maxLength, bool digit, int r, int g, int b){
	_mainItem.AddTextChild(onTextChange, title, maxLength, digit, r, g, b);
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