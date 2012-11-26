#include "Player.h"

Player::Player(std::string filename, float X, float Y, int interval, int spriteX, int spriteY):Object(filename, X, Y, spriteX, spriteY)
{
	_maxVelocity = 250;
	_previousPosition = _position;
	_interval = interval;
	if(_interval <= 0) _interval = 1;
	_countInterval = 0;
	_animationState = 0;
	_vDir = VDirNone;
	_hDir = HDirLeft;
	_frame = 0;
	_buttonUp=_buttonDown=_buttonLeft=_buttonRight=false;
	_velocity.Y = 50;
};
Rectangle Player::GetPreviousBoundR(float velocityX, float velocityY)
{
	Rectangle bound(_previousPosition.X+velocityX, _previousPosition.Y+velocityY, _spriteDimension.X, _spriteDimension.Y);
	return bound;
}
void Player::SetVelocity(float X, float Y){_velocity.X = X;_velocity.Y = 0;_maxVelocity=(int)Y;}
void Player::Update(Map& map1, int screenWidth, int screenHeight, long lastTick){
	//Update animation image for the player
	if(_buttonLeft || _buttonRight){
		if(_countInterval > _interval){
			_countInterval = 0;
			_animationState++;
			if(_animationState > 2)
				_animationState = 0;
		}
		if(_buttonLeft){
			_frame=0+_animationState;
			_hDir = HDirLeft;
		}
		else {
			_hDir = HDirRight;
			_frame=3+_animationState;
		}
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
	HandleCollision(map1, screenWidth, screenHeight, timeDiff);
	map1.SetNewMapPosition(Point2D((float)screenWidth, (float)screenHeight), GetCenter());

	//Prevents the player from walking out of screen
	Point2D mapDim = map1.GetMapDimension();
	if(_position.X - map1.GetMapPosition().X < 0) 
		_position.X = map1.GetMapPosition().X ;
	else if(mapDim.X > screenWidth)
	{
		if(_position.X + _spriteDimension.X - map1.GetMapPosition().X > screenWidth)
			_position.X = screenWidth - _spriteDimension.X +  map1.GetMapPosition().X;
	}
	else if(_position.X + _spriteDimension.X > mapDim.X)
		_position.X = mapDim.X - _spriteDimension.X;
	
	if(_position.Y -  map1.GetMapPosition().Y< 0){
		_position.Y = map1.GetMapPosition().Y;
		_velocity.Y = 0;
	}
	else if(mapDim.Y > screenHeight){
		if(_position.Y + _spriteDimension.Y -  map1.GetMapPosition().Y > screenHeight){
			_position.Y = screenHeight - _spriteDimension.Y +  map1.GetMapPosition().Y;
			_jumpEnable = true;
		}
	}
	else if(_position.Y + _spriteDimension.X > mapDim.Y){
		_position.Y = mapDim.Y - _spriteDimension.Y;
		_jumpEnable = true;
	}
	if(_velocity.Y < 0) _vDir = VDirUp;
	else if(_velocity.Y>0)_vDir=VDirDown;
	else _vDir = VDirNone;
}
void Player::Draw(WindowSurface screen, Point2D mapPosition)
{
	//Draws the player on the screen
	_surface.Draw(screen, (Sint16)(_position.X - mapPosition.X), (Sint16)(_position.Y - mapPosition.Y), &GetFrame());
}
void Player::HandleCollision(Map& map, int screenWidth, int screenHeight, float timeDiff){
	if(timeDiff*_velocity.Y <= map.GetTileDimension().Y && timeDiff*_velocity.X<=map.GetTileDimension().X &&
		timeDiff*_velocity.Y<=_spriteDimension.Y&&timeDiff*_velocity.X<=map.GetTileDimension().X){
		int Y1 = (int)((_position.Y + _velocity.Y*timeDiff) / map.GetTileDimension().Y);
		int Y2 = (int)((_position.Y + _velocity.Y*timeDiff + _spriteDimension.Y - 1) / map.GetTileDimension().Y);
		int Y = (_velocity.Y < 0)?Y1:Y2;
		//The left edge x coord
		int X1 = (int)(_position.X / map.GetTileDimension().X);
		//The right edge x coord
		int X2 = (int)((_position.X + _spriteDimension.X) / map.GetTileDimension().X);
		Point2D left((float)X1, (float)Y);
		Point2D right((float)X2, (float)Y);
		//IF both edges dont hit anything, the player can walk freely upwards or downwards
		int charTypeLeft = map.GetCharType(left);
		int charTypeRight = map.GetCharType(right);
		int charTypeLeft2 = map.GetCharType(Point2D((float)X1, (float)Y2));
		int charTypeRight2 = map.GetCharType(Point2D((float)X2, (float)Y2));
		if(charTypeLeft < 2 && charTypeRight < 2 && charTypeLeft2 < 2 && charTypeRight2 < 2){
			_position.Y += _velocity.Y*timeDiff;
		}
		//The player hit a block, so now it can only walk the difference between player and the block
		else {
			if(_velocity.Y < 0){
				int Yt = (int)((_position.Y) / map.GetTileDimension().Y);
				int botLeft = map.GetCharType(Point2D(X1, Yt));
				int botRight = map.GetCharType(Point2D(X2, Yt));
				if(((charTypeLeft == 2 || charTypeLeft == 3 || charTypeRight == 2 || charTypeRight == 3) && botLeft <2 && botRight < 2) ||
					(charTypeLeft == 2 && charTypeRight == 2) || (charTypeLeft == 3 && charTypeRight==3)) {
					//_position.Y = (Y+1)*map.GetTileDimension().Y;
					_velocity.Y = 0;
				}
				else if(charTypeLeft == 2 || charTypeLeft == 3 || charTypeRight == 2 || charTypeRight == 3)
					_position.Y += _velocity.Y*timeDiff;
				else if(charTypeLeft == 5 || charTypeLeft2 == 5 || charTypeRight == 5 || charTypeRight2 == 5){
					if (!map.CheckDrawCollision(GetBoundR(-map.GetMapPosition().X, -map.GetMapPosition().Y+_velocity.Y*timeDiff)))
						_position.Y += _velocity.Y*timeDiff;
					else _velocity.Y = 0;
				}
				else _position.Y += _velocity.Y*timeDiff;
			}
			else {
				if(charTypeLeft == 2 || charTypeRight == 2){
					_velocity.Y = 50;
					_jumpEnable = true;
				}
				else if((charTypeLeft < 2 && map.GetCharType(Point2D(X1, Y1)) == 3) || 
					charTypeRight < 2 && map.GetCharType(Point2D(X2, Y1)) == 3){

				}
				else if(charTypeLeft == 3 || charTypeRight == 3){
					//_position.Y += _velocity.Y*timeDiff;
					TileData tl = map.GetTileData((int)left.X, (int)left.Y);
					TileData tr = map.GetTileData((int)right.X, (int)right.Y);
					int yl1, yl2; tl.GetSlope(yl1, yl2);
					int yr1, yr2; tr.GetSlope(yr1, yr2);
					if((charTypeRight == 3 && charTypeLeft == 5) || (charTypeLeft == 3 && charTypeRight == 5)){
						float height = (charTypeLeft==3)?map.GetSlopeHeight(Point2D(_position.X, _position.Y+_spriteDimension.Y + _velocity.Y*timeDiff)):
							map.GetSlopeHeight(Point2D(_position.X+_spriteDimension.X, _position.Y+_spriteDimension.Y + _velocity.Y*timeDiff));
						float newpos= (Y*map.GetTileDimension().Y) + height - _spriteDimension.Y;
						if (newpos>_position.Y + _velocity.Y*timeDiff){
							newpos =_position.Y+_velocity.Y*timeDiff;
							if(!map.CheckDrawCollision(GetBoundR(-map.GetMapPosition().X, -map.GetMapPosition().Y + newpos - _position.Y))){
								_position.Y = newpos;
							}
							else {_velocity.Y = 50; 
							_jumpEnable = true;	}
						}
						else {
							if(!map.CheckDrawCollision(GetBoundR(-map.GetMapPosition().X, -map.GetMapPosition().Y + newpos - _position.Y))){
								_position.Y = newpos;
							}
							//_velocity.Y = 50;
							_jumpEnable = true;
						}
						
					}
					
					//on top of 1 or 2 slopes / or \ or /\ 
					else if((charTypeRight == charTypeLeft && yl2 != yr1 && (int)left.X != (int)right.X) || // if 2 of the same slopes to each other // or \\ 
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
							float h = (charTypeLeft == 3 && yl1>yl2)?map.GetSlopeHeight(Point2D(_position.X, _position.Y+_spriteDimension.Y + _velocity.Y*timeDiff)):map.GetSlopeHeight(Point2D(_position.X+_spriteDimension.X, _position.Y+_spriteDimension.Y + _velocity.Y*timeDiff));
							newpos = (Y*map.GetTileDimension().Y) + h;
						}

						//Set the new position
						if(newpos - _spriteDimension.Y >= _position.Y+_velocity.Y*timeDiff){
							_position.Y += _velocity.Y*timeDiff;
						}
						else {
							_position.Y = newpos - _spriteDimension.Y;
							_velocity.Y = 50;
							_jumpEnable = true;
						}
					}
				}
				else if(charTypeLeft == 5  || charTypeRight == 5){
					if(!map.CheckDrawCollision(GetBoundR(-map.GetMapPosition().X, -map.GetMapPosition().Y + _velocity.Y*timeDiff)))
						_position.Y += _velocity.Y*timeDiff;
					else {
						_velocity.Y = 50;
						_jumpEnable = true;}
				}
			}
		}
		if(_buttonLeft || _buttonRight){
			int X;
			if(_buttonLeft){
				//The new x position after walking if to left
				X = (int)((_position.X - _velocity.X*timeDiff) / map.GetTileDimension().X);
			}
			else {
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
				TileData slopeTop = _buttonLeft?map.GetTileData(X1, Y1):map.GetTileData(X2, Y1);
				if(Y1<Y2 && charTypeTop == 3 && (top.X != bot.X || top.Y != bot.Y )){
					if(_buttonLeft)
						_position.X -= (X+1) * map.GetTileDimension().X ;
					else 
						_position.X = X*map.GetTileDimension().X-_spriteDimension.X;
				}
				else if(charTypeTop == 5){
					float dx = _buttonLeft?-_velocity.X*timeDiff:_velocity.X*timeDiff;
					float h = map.CheckDrawCollision(GetBoundR(-map.GetMapPosition().X + dx, -map.GetMapPosition().Y));
					if(h <= 0){
						_position.X += dx;
					}
					else if(_velocity.Y > 0 && h < 4 &&
						map.CheckDrawCollision(GetBoundR(-map.GetMapPosition().X + dx, -map.GetMapPosition().Y - h - 1)) <= 0){
						_position.X += dx;
						_position.Y -= h;
					}
				}
				else{
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
						}
						//checks of the player can walk left or right
						//(IF there is no ceiling lower then spriteheight)
						if(_buttonLeft && hl>_spriteDimension.Y)
							_position.X -= _velocity.X*timeDiff;
						else if(_buttonRight && hr>_spriteDimension.Y)
							_position.X += _velocity.X*timeDiff;
					}
				}
			}
			else if(charTypeBot == 5 || charTypeTop == 5){
				if((charTypeBot == 5 && (charTypeTop <2 || charTypeTop == 5) && charTypeTop != 2) ||
					(charTypeTop == 5 && (charTypeBot <2 || charTypeBot == 5) && charTypeBot != 2))		{
					float dx = _buttonLeft?-_velocity.X*timeDiff:_velocity.X*timeDiff;
					float h = map.CheckDrawCollision(GetBoundR(-map.GetMapPosition().X + dx, -map.GetMapPosition().Y));
					if(h <= 0){
						_position.X += dx;
					}
					else if(_velocity.Y > 0 && h < 4 && 
						map.CheckDrawCollision(GetBoundR(-map.GetMapPosition().X + dx, -map.GetMapPosition().Y - h - 1)) <= 0){
						_position.X += dx;
						_position.Y -= h;
					}
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
		_previousPosition=_position;
	}
}
Point2D Player::GetCenter() { return Point2D(_position.X + _spriteDimension.X, _position.Y + _spriteDimension.Y); };
HorizontalDirection Player::GetHorizontalDir()
{
	/*
	if(_buttonLeft) return HDirLeft;
	else /*if(_buttonRight) return HDirRight;
	//else return HDirNone;
	*/
	return _hDir;
}
VerticalDirection Player::GetVerticalDir()
{
	return _vDir;
}
//Get a rectangle of the clipimage that is showed on the screen
SDL_Rect Player::GetFrame()
{
	SDL_Rect frames;
	frames.x = (Sint16)((_frame % _spriteX) * _spriteDimension.X); //Get X on the surface
	frames.y = (Sint16)((_frame / (_spriteY+1)) * _spriteDimension.Y); //Get Y on the surface
	frames.w = (Sint16)_spriteDimension.X; //Clip width
	frames.h = (Sint16)_spriteDimension.Y; //Clip Height
	return frames;
}
Rectangle Player::GetFrameR()
{
	Rectangle frames(((_frame % _spriteX) * _spriteDimension.X),((_frame / _spriteY) * _spriteDimension.Y),_spriteDimension.X,_spriteDimension.Y);
	return frames;
}
//Set the clipnumber that will be showed (like a book, from left to right, and then from top to bottom)
void Player::SetFrame(int frame)
{
	int maxFrame=_spriteX*_spriteY;
	_frame = frame;
	if(_frame<0){_frame=0;}
	else if(_frame > maxFrame){_frame=maxFrame;}
}
//Keep up if arrowbuttons are pressed
void Player::HandleEvent(SDL_Event sEvent)
{
	Uint8* keystates = SDL_GetKeyState(NULL);
	if(keystates[SDLK_DOWN]) _buttonDown = true; else _buttonDown = false;
	if(keystates[SDLK_UP]) _buttonUp = true; else _buttonUp = false;
	if(keystates[SDLK_LEFT]) _buttonLeft = true; else _buttonLeft = false;
	if(keystates[SDLK_RIGHT]) _buttonRight = true; else _buttonRight = false;
}