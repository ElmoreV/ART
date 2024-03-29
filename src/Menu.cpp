#include "Menu.h"
#include <sstream>

Options::Options(){
	_text = "";
	_r=_g=_b=255;
	_hover=false;
	_bound = Rectangle(0,0,0,0);
	Enabled = true;
}
Options::Options(std::string text,int r, int g, int b){
	_text = text;
	_r=r;_g=g;_b=b;
	_hover=false;
	_bound = Rectangle(0,0,0,0);
	Enabled = true;
}
int Options::GetColorR(){return (Enabled)?_r:127;}
int Options::GetColorG(){return (Enabled)?_g:127;}
int Options::GetColorB(){return (Enabled)?_b:127;}
Rectangle Options::GetBound(){return _bound;}
bool Options::GetHover(){return (_hover&&Enabled)?true:false;}
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
	if(r>=0&&r<=255)_r=r;
	if(g>=0&&g<=255)_g=g;
	if(b>=0&&b<=255)_b=b;
}
bool Options::IsBoundingBox(){return _boundSet;}

ButtonMenuItem::ButtonMenuItem(std::string title , bool center, int r, int g, int b,bool(Settings::*clickEvent)()):MenuItem(title, center, r, g, b){
	
	_type = ButtonItem; 
	_selected =false;
	HoverEnabled = true;
	_headerShown = true;
	OnClick = clickEvent;
	if (clickEvent)
	{_clickEventAssigned = true;}
}

OptionMenuItem::OptionMenuItem(std::vector<Options> options, bool center, int r, int g, int b){
	_type=OptionItem;
	Text = Header = "UNDEFINED";
	_optionText = "";
	for(unsigned int i = 0; i < options.size(); i++){
		_optionText += options.at(i).GetText() + " ";
		_options.push_back(options.at(i));
	}
	_clickEventAssigned = false;
	_r=r;_g=g;_b=b;
	_selected =false;
	HoverEnabled = true;
	_headerShown = true;
	_optionSpace = " ";
	_hover = false;
	_verticalSpace = 1;
	_center = center;
}
OptionMenuItem::OptionMenuItem(std::vector<std::string> options, bool center, int r, int g, int b, bool(Settings::*optionclicks)(int id)){
	_type=OptionItem;
	Text = Header = "UNDEFINED";
	_optionText = "";
	for(unsigned int i = 0; i < options.size(); i++){
		_optionText += options.at(i) + " ";
		_options.push_back(Options(options.at(i)));
	}
	
	_r=r;_g=g;_b=b;
	_selected =false;
	HoverEnabled = true;
	_headerShown = true;
	_optionSpace = " ";
	_hover = false;
	_verticalSpace = 1;
	_center = center;
	OnOptionClick = optionclicks;
	if (optionclicks)
	{_clickEventAssigned = true;}
}

unsigned int OptionMenuItem::OptionCount(){return _options.size(); }
std::string OptionMenuItem::GetOptionText(int index){ return _options.at(index).GetText(); }
Rectangle OptionMenuItem::GetOptionBound(int index){ return _options.at(index).GetBound(); }
bool OptionMenuItem::GetOptionHover(int index) { return _options.at(index).GetHover(); }
void OptionMenuItem::SetOptionHover(int index, bool value){ _options.at(index).SetHover(value); }
void OptionMenuItem::SetOptionBound(int index, float x, float y, float w, float h){_options.at(index).SetBound(x, y, w, h); }
Options* OptionMenuItem::GetOption(int index){return &_options.at(index); }
void OptionMenuItem::SetOptionEnabled(unsigned int index, bool value){
	if(index < _options.size())
		_options.at(index).Enabled = value;
}
std::string OptionMenuItem::GetOptionSpace(){
	if(_optionSpace != "") return _optionSpace;
	return " ";
}
int OptionMenuItem::GetOptionSpaceWidth(){return _optionSpaceWidth; }
void OptionMenuItem::SetOptionSpace(std::string value){
	_optionSpace = value;
}
void OptionMenuItem::SetOptionSpaceWidth(int value){_optionSpaceWidth = value;}
TextMenuItem::TextMenuItem(std::string text, int maxLength, bool digit, bool center, int r, int g, int b,bool (Settings::*ontextchange)(std::string text)):MenuItem(text,center,r,g,b){
	_type=TextItem;
	_maxLength = maxLength;
	_digitOnly = digit;
	_selected =false;
	HoverEnabled = true;
	_headerShown = true;
	_hover = false;
	onTextChange = ontextchange;
	if (onTextChange!=0)
	{_clickEventAssigned = true;}
}

SliderMenuItem::SliderMenuItem(int width, int height, bool center, int r, int g, int b,bool (*onvaluechange)(float value,void* pRev),void* pRev){
	_bounds.W = (float)width;
	_bounds.H = (float)height;
	_type = SliderItem;
	_maxLength = width;
	_r=r;_g=g;_b=b;	
	_status = width;	
	_selected =false;
	HoverEnabled = false;
	_headerShown = true;
	_hover = false;
	_verticalSpace = 1;
	_center = center;
	onValueChange=onvaluechange;
	onValueChangeRef=pRev;
	if (onValueChange)
	{_clickEventAssigned = true;}
}
float SliderMenuItem::GetRatio(){
	float status = (((float)_status)/((float)_maxLength));
	if(status<0)status=0;
	else if(status>1.0f)status=1.0f;
	return status;
}
void SliderMenuItem::SetStatus(int status){
	_status = status;
}

MenuItem::MenuItem(){
	Text = Header = "UNDEFINED";
	IsClickable = false;
	_type = NormalItem;
	_clickEventAssigned = false;
	_r = _g = _b = 255;
	_optionBoundSet=false;
	_customSet = false;
	HoverEnabled = false;
	_digitOnly=false;
	_headerShown = true;
	_optionSpace = " ";
	_hover = false;
	_verticalSpace = 1;
	_center = false;
}
MenuItem::MenuItem(std::string title, bool center, int r, int g, int b){
	Text = Header = title;
	IsClickable = false;
	_type = NormalItem;
	_clickEventAssigned = false;
	_r=r;_g=g;_b=b;
	_optionBoundSet=false;
	_customSet = false;
	HoverEnabled = false;
	_digitOnly=false;
	_headerShown = true;
	_optionSpace = " ";
	_hover = false;
	_verticalSpace = 1;
	_center = center;
}
void MenuItem::AddChild(std::string title, bool center, int r, int g, int b){
	if(_type == NormalItem) IsClickable = true;
	_childs.push_back(MenuItem(title, center, r, g, b));
	HoverEnabled = true;
}
void MenuItem::AddChild(MenuItem* item){
	if(_type == NormalItem) IsClickable = true;
	_childs.push_back(*item);
	HoverEnabled = true;
}
void MenuItem::AddButtonChild(std::string title, bool center, int r, int g, int b, bool(Settings::*onclick)()){
	if(_type == NormalItem)IsClickable=true;
	_childs.push_back(ButtonMenuItem(title, center, r, g, b, onclick));
	HoverEnabled = true;
}
void MenuItem::AddOptionChild(std::vector<std::string> options, std::string optionSpace, bool center, int r, int g, int b, bool(Settings::*optionclicks)(int id)){
	if(_type == NormalItem)IsClickable=true;
	OptionMenuItem item(options, center, r, g, b, optionclicks);
	item.SetOptionSpace(optionSpace);
	_childs.push_back(item);
	HoverEnabled = true;
}
void MenuItem::AddTextChild(std::string title, int maxLength, bool digit, bool center, int r, int g, int b, bool (Settings::*onTextChange)(std::string text)){
	if(_type == NormalItem)IsClickable=true;
	_childs.push_back(TextMenuItem(title, maxLength, digit, center, r, g, b,onTextChange));
	HoverEnabled = true;
}
void MenuItem::AddSliderChild(int width, int height, bool center, int r, int g, int b,bool (*onValueChange)(float value,void* pRev),void* pRev){
	if(_type == NormalItem)IsClickable=true;
	_childs.push_back(SliderMenuItem(width, height, center, r, g, b,onValueChange,pRev));
	HoverEnabled = true;
}

int MenuItem::HandleEvent(SDL_Event sEvent, Settings* setting){
	bool onClickItem = false;
	if(sEvent.type == SDL_MOUSEBUTTONDOWN){
		if(_bounds.Contains(sEvent.button.x, sEvent.button.y)) return -1;
		else {
			for(unsigned int i = 0; i < _childs.size(); i++){
				MenuItem* child = &_childs.at(i);
				Rectangle rec = *child->GetBoundingBox();
				if(child->GetType() == SliderItem){ rec.X -= 5; rec.W += 10; }
				if(rec.Contains(sEvent.button.x, sEvent.button.y)){
					onClickItem = true;
					child->SetSelect(true);
					
						switch(child->GetType()){
						case NormalItem:
							if(child->IsClickable) return i;
							break;
						case ButtonItem:
							if(child->IsEventAssigned())
								if((setting->*child->OnClick)())
									return i;
							break;
						case OptionItem:
							if(child->IsEventAssigned()){
								OptionMenuItem* oItem = (OptionMenuItem*)child;
								float offsetX = rec.X;
								for(unsigned int j = 0; j < oItem->OptionCount(); j++){
									if(offsetX <= sEvent.button.x && offsetX + oItem->GetOptionBound(j).W >= sEvent.button.x){
										if(oItem->GetOption(j)->Enabled){
											if((setting->*child->OnOptionClick)(j))
												return i;
										}
									}
									offsetX += oItem->GetOptionBound(j).W + oItem->GetOptionSpaceWidth();
								}
							}
							break;
					}
				}
				else child->SetSelect(false);
			}
		}
		return -2;
	}
	else if(sEvent.type == SDL_MOUSEBUTTONUP){
		for(unsigned int i = 0; i < _childs.size(); i++){
			if(_childs.at(i).GetType() == SliderItem){
				SliderMenuItem* item = (SliderMenuItem*)&_childs.at(i);
				item->SetSelect(false);
			}
		}
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
				if(_childs.at(i).GetType() == SliderItem && _childs.at(i).IsSelected()){
						SliderMenuItem* item = (SliderMenuItem*)&_childs.at(i);
						item->SetStatus(sEvent.button.x - (int)item->GetBoundingBox()->X);
						if(item->IsEventAssigned())
							if(item->onValueChange(item->GetRatio(),item->onValueChangeRef))
									return -3;
				}
				else if(_childs.at(i).GetBoundingBox()->Contains(sEvent.button.x, sEvent.button.y) && _childs.at(i).GetType() != SliderItem){
					_childs.at(i).SetHover(true);
					if(_childs.at(i).GetType() == OptionItem){
						OptionMenuItem* oItem = (OptionMenuItem*)&_childs.at(i);
						float offsetX = oItem->GetBoundingBox()->X;
						for(unsigned int j = 0; j < oItem->OptionCount(); j++){
							if(offsetX <= sEvent.button.x && offsetX + oItem->GetOptionBound(j).W >= sEvent.button.x)
								oItem->SetOptionHover(j, true);
							else
								oItem->SetOptionHover(j, false);
							offsetX += oItem->GetOptionBound(j).W + oItem->GetOptionSpaceWidth();
						}
				}
				}
				else _childs.at(i).SetHover(false);
			}
		}

	}
	else if(sEvent.type == SDL_KEYDOWN){
		for(unsigned int i = 0; i < _childs.size(); i++){
			MenuItem* item = &_childs.at(i);
			if(item->IsSelected() && item->GetType() == TextItem){
				TextMenuItem* tItem = (TextMenuItem*)&item;
				std::string str = item->GetText();
				SDL_Keycode key = sEvent.key.keysym.scancode;
				if(key == SDL_SCANCODE_BACKSPACE && item->Text.size() > 0){
					if (str.size () > 0){
						str.resize (str.size () - 1);
					}
					if(str.size() == 0) str = " ";
						item->Text = str;
				}
				else if( key == SDL_SCANCODE_RETURN){
					item->SetSelect(false);
				}
				if(item->IsEventAssigned())
					if((setting->*item->onTextChange)(str))
						return i;
			}
		}
	}else if(sEvent.type == SDL_TEXTINPUT){
		for(unsigned int i = 0; i < _childs.size(); i++){
			MenuItem* item = &_childs.at(i);
			if(item->IsSelected() && item->GetType() == TextItem){
				TextMenuItem* tItem = (TextMenuItem*)&item;
				std::string str = item->GetText();
				std::string key = sEvent.text.text;

				if(str.size() < (Uint32)item->GetMaxLength()){
					char v = (char)key[0];
					if(!item->IsDigitOnly() || (item->IsDigitOnly() && key[0] >= 48 && key[0] <= 57)){
						if(str != " ") str += v;
						else str = v;
						item->Text = str;
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
void MenuItem::Draw(Window screen, Font& font, Point2D offset){
	float y = offset.Y;
	float x = offset.X;

	Surface render;
	if(_headerShown){
		font.SetColor(_r, _g, _b);
		if(Header != "") render.RenderText(font, Header); 
		else render.RenderText(font, " ");
		if(_center) x = ((float)screen.GetWidth() / 2) - ((float)render.GetWidth() / 2);
		render.Draw(screen, (int)x, (int)y);
		SetBoundingBox(x, y, (float)render.GetWidth(), (float)render.GetHeight());
		y += render.GetHeight();
		if(IsHover()) screen.DrawLine((int)x, (int)y, (int)(x + render.GetWidth()), (int)y, _r, _g, _b);
		y += _verticalSpace; 
		x+=5;
	}

	for(unsigned int i = 0; i < _childs.size(); i++){
		MenuItem* item = &_childs.at(i);
		if(item->GetType() != SliderItem && item->GetType() != OptionItem){
			font.SetColor(item->GetColorR(), item->GetColorG(), item->GetColorB());
			render.RenderText(font, item->GetText());
			if(_center) x = (float)screen.GetWidth() * 0.5f - (float)render.GetWidth() * 0.5f;
			if(item->IsCustomPosition())
				item->SetBoundingBox(item->GetCustomPosition().X, item->GetCustomPosition().Y, (float)render.GetWidth(), (float)render.GetHeight());
			else
				item->SetBoundingBox(x, y, (float)render.GetWidth(), (float)render.GetHeight());
			render.Draw(screen, (Uint32)item->GetBoundingBox()->X, (Uint32)item->GetBoundingBox()->Y);
			y += render.GetHeight() + _verticalSpace; 
		}
		else if(item->GetType() == OptionItem){
			render.RenderText(font, item->GetText());
			OptionMenuItem* oItem = (OptionMenuItem*)item;
			if(_center) x = (float)screen.GetWidth() * 0.5f - (float)render.GetWidth() * 0.5f;
			if(item->IsCustomPosition())
				item->SetBoundingBox(item->GetCustomPosition().X, item->GetCustomPosition().Y, (float)render.GetWidth(), (float)render.GetHeight());
			else
				item->SetBoundingBox(x, y, (float)render.GetWidth(), (float)render.GetHeight());
			float tempX = item->GetBoundingBox()->X;
			for(unsigned int i = 0; i < oItem->OptionCount(); i++){
				Options* option = oItem->GetOption(i);
				font.SetColor(option->GetColorR(), option->GetColorG(), option->GetColorB());
				render.RenderText(font, option->GetText());
				render.Draw(screen, (Uint32)tempX, (Uint32)item->GetBoundingBox()->Y);
				tempX += render.GetWidth();
				
				font.SetColor(item->GetColorR(), item->GetColorG(), item->GetColorB());
				render.RenderText(font, oItem->GetOptionSpace());
				render.Draw(screen, (Uint32)tempX, (Uint32)item->GetBoundingBox()->Y);
				tempX += render.GetWidth();
				oItem->SetOptionSpaceWidth(render.GetWidth());
			}
		}
		else {
			SliderMenuItem* sItem = (SliderMenuItem*)item;
			Rectangle* bound = sItem->GetBoundingBox();
			if(_center) x = screen.GetWidth() / 2 - bound->W / 2;
			if(sItem->IsCustomPosition())
				sItem->SetBoundingBox(sItem->GetCustomPosition().X, sItem->GetCustomPosition().Y, -1, -1);
			else
				sItem->SetBoundingBox(x, y, -1, -1);
			screen.DrawLine((int)bound->X, (int)(bound->Y + bound->H/2), (int)(bound->X + bound->W), (int)(bound->Y + bound->H/2), sItem->GetColorR(), sItem->GetColorG(), sItem->GetColorB());
			float w = (sItem->GetRatio())*sItem->GetMaxLength();
			float h = (1-sItem->GetRatio())*(bound->H/4);
			screen.DrawFilledRect((int)(bound->X + w - 5), (int)(bound->Y+h), (int)(bound->X + w + 5), (int)(bound->Y + bound->H - h), sItem->GetColorR(), sItem->GetColorG(), sItem->GetColorB());
			y += bound->H+_verticalSpace;
		}

		
		if(item->GetType() == OptionItem){
			OptionMenuItem* oItem = (OptionMenuItem*)item;
			render.RenderText(font, oItem->GetOptionSpace()); int spaceWidth = render.GetWidth(); oItem->SetOptionSpaceWidth(spaceWidth);
			if(!_optionBoundSet){
				_optionBoundSet=true;
				float offsetX = item->GetBoundingBox()->X;
				for(unsigned int j = 0; j < oItem->OptionCount(); j++){
					font.SetColor(oItem->GetOption(j)->GetColorR(), oItem->GetOption(j)->GetColorG(), oItem->GetOption(j)->GetColorB()); 
					render.RenderText(font, oItem->GetOptionText(j));
					oItem->SetOptionBound(j, offsetX, item->GetBoundingBox()->Y, (float)render.GetWidth(), (float)render.GetHeight());
					offsetX += oItem->GetOptionBound(j).W + spaceWidth;
				}
			}
			if(item->IsHover() && item->HoverEnabled){
				float offsetX = item->GetBoundingBox()->X;
				for(unsigned int j = 0; j < oItem->OptionCount(); j++){
					if(oItem->GetOptionHover(j)){
						screen.DrawLine((int)offsetX, (int)(item->GetBoundingBox()->Y + item->GetBoundingBox()->H), 
							(int)(offsetX + oItem->GetOptionBound(j).W), (int)(item->GetBoundingBox()->Y + item->GetBoundingBox()->H), oItem->GetOption(j)->GetColorR(), oItem->GetOption(j)->GetColorG(), oItem->GetOption(j)->GetColorB());
						break;
					}
					offsetX += oItem->GetOptionBound(j).W + spaceWidth;
				}
			}
		}
		else if(item->IsHover() && item->HoverEnabled){
			Rectangle* b(item->GetBoundingBox());
			screen.DrawLine((int)b->X, (int)(b->Y+b->H), (int)(b->X + b->W), (int)(b->Y+b->H), item->GetColorR(), item->GetColorG(), item->GetColorB());
		}
	}
	render.Free();
};
void MenuItem::ShowHeader(bool value){ _headerShown = value; }
void MenuItem::SetVerticalSpace(int value){_verticalSpace = value; }
void MenuItem::SetCenter(bool value){_center = value; }
bool MenuItem::IsSelected(){return _selected;}
std::string MenuItem::GetText(){
	if(_type==OptionItem) { Text = "";
		for(unsigned int i = 0; i < _options.size(); i++){
			Text += _options.at(i).GetText() + _optionSpace;
		}
	}
	if(Text != "") return Text; 
	return " "; 
}
int MenuItem::GetMaxLength(){return _maxLength;}
bool MenuItem::IsDigitOnly(){return _digitOnly;}
void MenuItem::SetSelect(bool value){_selected=value;}
MenuItem* MenuItem::GetChild(unsigned int index){
	if(index<_childs.size()){
		return &_childs.at(index);
	}
	else return false;
}
MenuType MenuItem::GetType() { return _type; }
Rectangle* MenuItem::GetBoundingBox(){return &_bounds;}
bool MenuItem::IsEventAssigned(){return _clickEventAssigned; }
bool MenuItem::IsHover(){return _hover;}
int MenuItem::CountChild(){ return _childs.size(); }
int MenuItem::GetColorR(){return _r;}
int MenuItem::GetColorG(){return _g;}
int MenuItem::GetColorB(){return _b;}
bool MenuItem::IsCustomPosition(){return _customSet; }
Point2D MenuItem::GetCustomPosition(){return _custom; }
void MenuItem::SetHover(bool value){_hover =value;}
void MenuItem::SetBoundingBox(float x, float y, float width, float height){
	if(x>=0)_bounds.X = x;
	if(y>=0)_bounds.Y = y;
	if(width>=0)_bounds.W = width;
	if(height>=0)_bounds.H = height;
}
void MenuItem::SetColor(int r, int g, int b) { 
	if(r>=0&&r<=255)_r=r;
	if(g>=0&&g<=255)_g=g;
	if(b>=0&&b<=255)_b=b;
}
void MenuItem::SetCustomPosition(float x, float y){ _customSet=true;_custom = Point2D(x, y); }

Menu::Menu(std::string text, Settings* setting, int r, int g, int b){
	_mainItem = MenuItem(text, false, r, g, b);
	_currentItem = &_mainItem;
	_mainItem.SetColor(r, g, b);
	_setting = setting;
}
void Menu::Open(Window screen, Font& font, Point2D offset){
	if(_itemTracker.size() == 0) _currentItem = &_mainItem;
	_currentItem->Draw(screen, font,offset);
}
MenuItem* Menu::GetChild(unsigned int index){
	return _mainItem.GetChild(index);
}

void Menu::AddChild(std::string text, bool center, int r, int g, int b){
	_mainItem.AddChild(text, center, r, g, b);
}
void Menu::AddChild(MenuItem* item){
	_mainItem.AddChild(item);
}

void Menu::AddButtonChild(std::string title, bool center, int r, int g, int b,bool(Settings::*onclick)()){
	_mainItem.AddButtonChild(title , center, r, g, b,onclick);
}
void Menu::AddOptionChild(std::vector<std::string> options, std::string optionSpace, bool center, int r, int g, int b, bool(Settings::*optionclicks)(int id)){
	_mainItem.AddOptionChild(options, optionSpace, center, r, g, b, optionclicks);
}

void Menu::AddTextChild(std::string title, int maxLength, bool digit, bool center, int r, int g, int b,bool (Settings::*onTextChange)(std::string text)){
	_mainItem.AddTextChild(title, maxLength, digit, center, r, g, b,onTextChange);
}
void Menu::AddSliderChild(int width, int height, bool center, int r, int g, int b,bool (*onValueChange)(float value,void* pRev),void* pRev){
	_mainItem.AddSliderChild(width, height, center, r, g, b,onValueChange,pRev);
}
void Menu::SetVerticalSpace(int space){
		_mainItem.SetVerticalSpace(space);
	}
void  Menu::SetColor(unsigned int r, unsigned int g, unsigned int b){
	if(r>255)r=255;
	if(g>255)g=255;
	if(b>255)b=255;
	_mainItem.SetColor(r, g, b);
}
void  Menu::ShowHeader(bool value){ _mainItem.ShowHeader(value); }
void  Menu::SetCenter(bool value){_mainItem.SetCenter(value); }
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