#include "Object.h"
//Constructor
Object::Object(std::string filename, float X, float Y, int spriteX, int spriteY)
{
	_surface.LoadImage(filename.c_str());
	_position.X = X; _position.Y = Y;
	_velocity.X = 0.0f; _velocity.Y = 0.0f;
	_spriteX = spriteX; _spriteY = spriteY;
	_spriteDimension.X = _surface->w / (float)_spriteX;
	_spriteDimension.Y = _surface->h / (float)_spriteY;
	_frame = 9;
	_showing = true;
	_buttonUp=false; _buttonDown=false; _buttonLeft=false; _buttonRight=false;
}
//Get a rectangle of the position and the dimentions
SDL_Rect Object::GetBound()
{
	SDL_Rect bound = {(Sint16)_position.X, (Sint16)_position.Y, (Sint16)_spriteDimension.X, (Sint16)_spriteDimension.Y};
	return bound;
}
//Get a rectangle of the clipimage that is showed on the screen
SDL_Rect Object::GetFrame()
{
	SDL_Rect frames;
	frames.x = (Sint16)((_frame % _spriteX) * _spriteDimension.X); //Get X on the surface
	frames.y = (Sint16)((_frame / _spriteY) * _spriteDimension.Y); //Get Y on the surface
	frames.w = (Sint16)_spriteDimension.X; //Clip width
	frames.h = (Sint16)_spriteDimension.Y; //Clip Height
	return frames;
}
//Draws the surface on the screen
void Object::Draw(WindowSurface screen)
{
	if((_spriteX == 1) && (_spriteY == 1))//Check if it is the whole surface
		_surface.Draw(screen, (Sint16)_position.X, (Sint16)_position.Y, NULL);//Draws whole surface
	else 
		_surface.Draw(screen, (Uint16)_position.X, (Uint16)_position.Y, &GetFrame()); //Draws only framepart on the screen
}
//Convert the collision rectangle to the rectangle on the surface
SDL_Rect Object::ConvertToImagePosition(SDL_Rect rect)
{
    SDL_Rect normalized;
	normalized.x = rect.x - (Sint16)_position.X;//Get X position of the collision on the surface
	normalized.y = rect.y - (Sint16)_position.Y;//Get Y position of the collision on the surface
    normalized.w = rect.w;
    normalized.h = rect.h;
     
    return normalized;
}
//Get the boundingbox if the two rectangles intersect
SDL_Rect Object::GetBoundingBox(SDL_Rect boundsA, SDL_Rect boundsB)
{
	//Calculate collision 
    int x1 = Maximum(boundsA.x, boundsB.x);
    int y1 = Maximum(boundsA.y, boundsB.y);
    int x2 = Minimum(boundsA.x + boundsA.w, boundsB.x + boundsB.w);
    int y2 = Minimum(boundsA.y + boundsA.h, boundsB.y + boundsB.h);
    //Get width and height of collisionpart
    int width = x2 - x1; int height = y2 - y1;
	//Return collisionpart
    if(width > 0 && height > 0) 
	{ 
		SDL_Rect intersect = {x1, y1, width, height}; 
		return intersect; 
	}
	//If there is no collision
    else  
	{ 
		SDL_Rect intersect = {0, 0, 0, 0}; 
		return intersect;
    }
};
//Check if there is collision
bool Object::CheckCollision(Object objB)
{
	//Get boundingbox
	SDL_Rect collisionRect = GetBoundingBox(GetBound(), objB.GetBound());
	//Check if there is no collision
    if(collisionRect.w == 0 && collisionRect.h == 0)
        return false;
	//Get the boundingbox on the two surfaces
	SDL_Rect normalA = ConvertToImagePosition(collisionRect);
    SDL_Rect normalB = objB.ConvertToImagePosition(collisionRect);
	
	//Check if two no-transpirant pixel collide
    for(int y = 0; y < collisionRect.h; y++)
	{
        for(int x = 0; x < collisionRect.w; x++)
		{
			//Return true if two non-transpirant pixels collide
			if(GetAlphaXY(this, normalA.x + x, normalA.y + y) && GetAlphaXY(&objB, normalB.x + x, normalB.y + y))
                return true;
		}
	}
    return false;
};
//Return if selected pixel on the surface is transpirant or not
bool Object::GetAlphaXY(Object* obj, int x, int y)
{
	int bpp = obj->_surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)obj->_surface->pixels + y * obj->_surface->pitch + x * bpp;
    Uint32 pixelColor;
     
    switch(bpp)
    {
        case(1):
            pixelColor = *p;
            break;
        case(2):
            pixelColor = *(Uint16*)p;
            break;
        case(3):
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                pixelColor = p[0] << 16 | p[1] << 8 | p[2];
            else
                pixelColor = p[0] | p[1] << 8 | p[2] << 16;
            break;
        case(4):
            pixelColor = *(Uint32*)p;
            break;
    }
     
    Uint8 red, green, blue, alpha;
    SDL_GetRGBA(pixelColor, obj->_surface->format, &red, &green, &blue, &alpha);
	int r=0, b=0, g=0;
	obj->_surface.GetMaskColor(r, g, b);
	//If colorKey == null
	if(b == -1){return alpha > 200;}
	else
	{
		if(red == r && green == g && blue == b){return false;}
		return true;
	}
		
};
//Changes the sprite to the new sprite with the rgb data for the mask color
bool Object::ChangeSprite(std::string filename, int r, int g, int b )
{
	if(_surface != 0) { return _surface.LoadImage(filename.c_str(), r, g, b); }
	else return false;
}
//Changes the object position
void Object::SetPosition(float X, float Y){_position.X = X;_position.Y = Y;}
//Changes the object velocity
void Object::SetVelocity(float X, float Y){_velocity.X = X;_velocity.Y = Y;}
//Set the clipnumber that will be showed (like a book, from left to right, and then from top to bottom)
void Object::SetFrame(int frame)
{
	int maxFrame=_spriteX*_spriteY;
	_frame = frame;
	if(_frame<0){_frame=0;}
	else if(_frame > maxFrame){_frame=maxFrame;}
}
//Set the colorkey (mask) of the surface
bool Object::MaskColor(int r, int g, int b)
{
	if(_surface!=0){return _surface.MaskColor(r, g, b);}
	else{return false;}
}
//Releases resources of the class
void Object::Free(){_surface.Free();}
//Keep up if arrowbuttons are pressed
void Object::HandleEvent(SDL_Event sEvent)
{
	Uint8* keystates = SDL_GetKeyState(NULL);
	if(keystates[SDLK_DOWN]) _buttonDown = true; else _buttonDown = false;
	if(keystates[SDLK_UP]) _buttonUp = true; else _buttonUp = false;
	if(keystates[SDLK_LEFT]) _buttonLeft = true; else _buttonLeft = false;
	if(keystates[SDLK_RIGHT]) _buttonRight = true; else _buttonRight = false;
}

Player::Player(std::string filename, float X, float Y, int interval, int spriteX, int spriteY):Object(filename, X, Y, spriteX, spriteY)
{
	_maxVelocity = 250;
	_interval = interval;
	if(_interval <= 0) _interval = 1;
	_countInterval = 0;
	_animationState = 0;
	_lastFrame = 1;
};
void Player::SetVelocity(float X, float Y){_velocity.X = X;_velocity.Y = 0;_maxVelocity=(int)Y;}
void Player::Update(Map map1, long lastTick){
	//Update animation image for the player
	_frame = _lastFrame;
	if((_buttonLeft || _buttonRight) && (_countInterval > _interval)){
		_countInterval = 0;
		_animationState++;
		if(_animationState > 2)
			_animationState = 0;
	}
	_countInterval++;
	
	//Timedifference, important for time-based movement
	float timeDiff=lastTick<0?1:(clock()-lastTick)/1000.0f;
	//For the jump
	if(_velocity.Y == 0) 
		_velocity.Y++;
	else if(_velocity.Y < _maxVelocity) 
		_velocity.Y+=_maxVelocity*timeDiff;
	//Initiate jump if button up is pressed
	if(_buttonUp && _jumpEnable) {
		_velocity.Y = (float)-_maxVelocity; 
		_jumpEnable = false; 
	}
	//Handles walking and collision
	HandleCollision(map1, 800, 600, timeDiff);

	//Prevents the player from walking out of screen
	if(_position.X - map1.GetMapPosition().X < 0) 
		_position.X = map1.GetMapPosition().X ;
	else if(_position.X + _spriteDimension.X - map1.GetMapPosition().X > 800) 
		_position.X = 800 - _spriteDimension.X + map1.GetMapPosition().X ;
	if(_position.Y -  map1.GetMapPosition().Y< 0) {
		_position.Y = map1.GetMapPosition().Y;
		_velocity.Y = 0;
	}
	else if(_position.Y + _spriteDimension.Y -  map1.GetMapPosition().Y > 600) {
		_jumpEnable = true;
		_position.Y = 600 - _spriteDimension.Y +  map1.GetMapPosition().Y;
	}
	
}
void Player::Draw(WindowSurface screen)
{
	//Draws the player on the screen
	_surface.Draw(screen, (Sint16)_position.X, (Sint16)_position.Y, &GetFrame());
}
void Player::HandleCollision(Map map, int screenWidth, int screenHeight, float timeDiff){
	if(timeDiff*_velocity.Y <= map.GetTileDimension().Y && timeDiff*_velocity.X<=map.GetTileDimension().X){
		int Y;
		if(_velocity.Y < 0){
			//The new y position after walking if up
			Y = (int)((_position.Y + _velocity.Y*timeDiff) / map.GetTileDimension().Y);
		}
		else {
			//The new y position after walking if down
			Y = (int)((_position.Y + _velocity.Y*timeDiff + _spriteDimension.Y) / map.GetTileDimension().Y);
		}
		//The left edge x coord
		int X1 = (int)(_position.X / map.GetTileDimension().X);
		//The right edge x coord
		int X2 = (int)((_position.X + _spriteDimension.X) / map.GetTileDimension().X);
		Point2D left((float)X1, (float)Y);
		Point2D right((float)X2, (float)Y);
		//IF both edges dont hit anything, the player can walk freely upwards or downwards
		int charTypeLeft = map.GetCharType(left);
		int charTypeRight = map.GetCharType(right);

		if(charTypeLeft < 2 && charTypeRight < 2){
			_position.Y += _velocity.Y*timeDiff;

		}
		//The player hit a block, so now it can only walk the difference between player and the block
		else {
			if(_velocity.Y < 0){
				//double yDiff = abs(_position.Y - ((Y+1) * map.GetTileDimension().Y));
				_position.Y = (Y+1)*map.GetTileDimension().Y;
				_velocity.Y = 0;
			}
			else if(_velocity.Y > 0) {
				//double yDiff = abs(_position.Y + _spriteDimension.Y - (Y *map.GetTileDimension().Y));
				//_position.Y = (Y*map.GetTileDimension().Y) - _spriteDimension.Y;
				if(charTypeLeft == 2 || charTypeRight == 2){
					_position.Y = (Y*map.GetTileDimension().Y) - _spriteDimension.Y;
					_velocity.Y = 50;
					_jumpEnable = true;
				}
				else if(charTypeLeft == 3 || charTypeRight == 3){
					
					//_position.Y += _velocity.Y*timeDiff;
					TileData tl = map.GetTileData((int)left.X, (int)left.Y);
					TileData tr = map.GetTileData((int)right.X, (int)right.Y);
					int yl1, yl2; tl.GetSlope(yl1, yl2);
					int yr1, yr2; tr.GetSlope(yr1, yr2);
					//on top of 1 or 2 slopes / or \ or /\ 
					if((charTypeRight == charTypeLeft && yl2 != yr1 && (int)left.X != (int)right.X) || // if 2 of the same slopes to each other // or \\ 
						(charTypeLeft == 3 && yl2>yl1 &&  (charTypeRight != 3 || (charTypeRight == 3 && yr1>yr2))) || 
						(charTypeRight == 3 && yr1>yr2&&(charTypeLeft!=3 || (charTypeLeft==3&&yl2>yl1)))){
						int h = yl2>yr1?yl2:yr1;
						float newpos = ((Y+1)*map.GetTileDimension().Y) - _spriteDimension.Y - h;
						if(newpos >= _position.Y+_velocity.Y*timeDiff)
							_position.Y += _velocity.Y*timeDiff;
						else {
							_position.Y = newpos;
							_velocity.Y = 50;
							_jumpEnable = true;
						}
					}
					else if((charTypeLeft == 3 && yl1>yl2) || (charTypeRight == 3 && yr1<yr2)){ //Downwards
						float newpos;
						//on top of 2 downward slopes \/
						if((charTypeLeft == 3 && yl1>yl2)&&(charTypeRight == 3 && yr1<yr2)){
							float hl = map.GetSlopeHeight(Point2D(_position.X, _position.Y+_spriteDimension.Y + _velocity.Y*timeDiff));
							float hr = map.GetSlopeHeight(Point2D(_position.X+_spriteDimension.X, _position.Y+_spriteDimension.Y + _velocity.Y*timeDiff));
							newpos = hl<hr?(Y*map.GetTileDimension().Y) + hl:(Y*map.GetTileDimension().Y) + hr;
						}
						else {
							//normal slope \ or /
							float h = (charTypeLeft == 3 && yl1>yl2)?map.GetSlopeHeight(Point2D(_position.X, _position.Y+_spriteDimension.Y + _velocity.Y*timeDiff)):map.GetSlopeHeight(Point2D(_position.X+_spriteDimension.Y, _position.Y+_spriteDimension.Y + _velocity.Y*timeDiff));
							newpos = (Y*map.GetTileDimension().Y) + h;
						}
						//Set the new position
						if(newpos - _spriteDimension.Y >= _position.Y+_velocity.Y*timeDiff)
							_position.Y += _velocity.Y*timeDiff;
						else {
							_position.Y = newpos - _spriteDimension.Y;
							_velocity.Y = 50;
							_jumpEnable = true;
						}
					}
				}
			}
		}
		if(_buttonLeft || _buttonRight){
			int X;
			if(_buttonLeft){
				//For animation
				_frame=5+_animationState;_lastFrame = 4;
				//The new x position after walking if to left
				X = (int)((_position.X - _velocity.X*timeDiff) / map.GetTileDimension().X);
			}
			else {
				//For animation
				_frame=9+_animationState;_lastFrame = 8;
				//The new x position after walking if to right
				X = (int)((_position.X + _velocity.X*timeDiff + _spriteDimension.X) / map.GetTileDimension().X);
			}
			int Y1 = (int)(_position.Y / map.GetTileDimension().Y); // Top edge of sprite
			int Y2 = (int)((_position.Y + _spriteDimension.Y - 1) / map.GetTileDimension().Y); //bottom edge of sprite
			Point2D top((float)X, (float)Y1);
			Point2D bot((float)X, (float)Y2);
			int charTypeTop = map.GetCharType(top);
			int charTypeBot = map.GetCharType(bot);
			//IF both edge don't hit anything, the player can freely move to left or right
			if(charTypeTop < 2 && charTypeBot < 2){
				if(_buttonLeft){_position.X -= _velocity.X*timeDiff;}
				else {_position.X += _velocity.X*timeDiff;}
			}
			//If it is a slope
			else if(charTypeBot == 3){
				//velocity defference
				float dx = _buttonLeft?_position.X - _velocity.X * timeDiff:_position.X+_velocity.X*timeDiff;
				int X1 = (int)(dx/map.GetTileDimension().X);
				int X2 = (int)((dx + _spriteDimension.X)/map.GetTileDimension().X);
				//Get tile data
				TileData slopeLeft = map.GetTileData(X1, Y2);
				TileData slopeRight = map.GetTileData(X2, Y2);
				int ly1, ly2, ry1, ry2;
				slopeLeft.GetSlope(ly1, ly2);
				slopeRight.GetSlope(ry1, ry2);
				if(ly1==ly2==1) {ly1=ly2=0; }
				if(ry1==ry2==1) {ry1=ry2=0; }
				float hl = map.GetHeightAtPosition(Point2D(dx, _position.Y));
				float hr = map.GetHeightAtPosition(Point2D(dx+_spriteDimension.X, _position.Y));
				//IF player hits the high side of a slope, so he can walk through
				if(_buttonLeft && (int)(_position.X/map.GetTileDimension().X)>X1 &&
					Y2*map.GetTileDimension().Y-_position.Y<=ly2 && ly2 > ry1 && abs(ly2 - ry1) > 1){
					_position.X = (X1+1)*map.GetTileDimension().X;
				}
				//IF player hits the high side of a slope, so he can walk through
				else if(_buttonRight && (int)((_position.X+_spriteDimension.X)/map.GetTileDimension().X)<X2 && 
					Y2*map.GetTileDimension().Y-_position.Y<=ry1 && ry1 > ly2 && abs(ly2 - ry1) > 1){
					_position.X = X2*map.GetTileDimension().X-_spriteDimension.X-1;
				}
				else {
					float newpos;
					bool handled = false;
					//walking up a downward slope: \ <-direction
					if(ly1 > ly2 && _buttonLeft && hl > _spriteDimension.Y){ //down
						float diff=_position.X - (X1 * map.GetTileDimension().X);
						float ratio = (ly1-ly2)/map.GetTileDimension().X;
						newpos = Y2 * map.GetTileDimension().Y + diff * ratio - _spriteDimension.Y - ly1 + map.GetTileDimension().Y;
						handled = true;
					}
					//walking up a downward slope: direction-> /
					else if(ry1 < ry2 && _buttonRight && hr>_spriteDimension.Y){ //Up
						float diff = _position.X+_spriteDimension.X-(X2*map.GetTileDimension().X);
						float ratio = (ry2-ry1)/map.GetTileDimension().X;
						newpos = Y2 * map.GetTileDimension().Y + map.GetTileDimension().Y - diff * ratio - _spriteDimension.Y;
						handled = true;
					}
					//handles player pos
					if(handled && newpos < _position.Y){
						_position.Y = newpos;
						//_velocity.Y = 50;
						//_jumpEnable = true;
					}
					//checks of the player can walk left or right
					//(IF there is no ceiling lower then spriteheight)
					if(_buttonLeft && hl>_spriteDimension.Y)
						_position.X -= _velocity.X*timeDiff;
					else if(_buttonRight && hr>_spriteDimension.Y)
						_position.X += _velocity.X*timeDiff;
				}
			}
			//Minimal one edge hit a block, so now it can only move the difference between the block and the player
			else{
				if(_buttonLeft){
					double xDiff = abs(_position.X - ((X+1) * map.GetTileDimension().X));
					_position.X -= (float)(xDiff-1);
				}
				else {
					double xDiff = abs(_position.X + _spriteDimension.X - (X*map.GetTileDimension().X));
					_position.X += (float)(xDiff-1);
				}

			}
		}
	}
}