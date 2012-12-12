#include "Player.h"

Player::Player(Surface* surface, float X, float Y, int interval, int spriteX, int spriteY):Object(surface, X, Y, spriteX, spriteY)
{
	_maxVelocity = 250;
	_previousPosition = _position;
	_interval = interval;
	if(_interval <= 0) _interval = 1;
	_countInterval = 0;
	_animationState = 0;
	_vDir = VDirNone;
	_hDir = HDirLeft;
	_cameraHDir = HDirNone;
	_frame = 0;
	_buttonUp=_buttonDown=_buttonLeft=_buttonRight=false;
	_velocity.Y = 50;
	InkPool = 100; _maxInkPool = 100;
	Health  = 100; _maxHealth= 100;
	InvulnerableTime = 0;
	_totalInkReceived=(float)_maxInkPool;
}
Rectangle Player::GetPreviousBoundR(float velocityX, float velocityY)
{
	Rectangle bound(_previousPosition.X+velocityX, _previousPosition.Y+velocityY, _spriteDimension.X, _spriteDimension.Y);
	return bound;
}
void Player::SetVelocity(float X, float Y){_velocity.X = X;_velocity.Y = 0;_maxVelocity=(int)Y;}
void Player::AddInk(int value){
	_totalInkReceived += value;
}
void Player::SetMapPosition(Map* map, Point2D screenSize, float timeDiff, float part,float threshold, float speed){
	Point2D maxDim = map->GetMapDimension();
	Point2D centerPoint  = GetCenter();
	Point2D camera = map->GetMapPosition();
	if(maxDim.X < screenSize.X)
	{camera.X=0;}
	else
	{
		float posOnScreen = centerPoint.X - camera.X;
		if (_cameraHDir==HDirNone)
		{
			if (_buttonLeft)
			{
				if (posOnScreen - _velocity.X*timeDiff < screenSize.X*threshold)
				{_cameraHDir=HDirLeft;}
			}else if (_buttonRight)
			{
				if (posOnScreen + _velocity.X*timeDiff > screenSize.X*(1-threshold))
				{_cameraHDir=HDirRight;}
			}
		}
		if (_cameraHDir==HDirLeft)
		{
			if(_buttonLeft)
			{
				if(posOnScreen + _velocity.X*timeDiff +2 < screenSize.X*(1-part))
					camera.X -= _velocity.X * timeDiff*speed;
				else if (posOnScreen <screenSize.X*(1-part))
					camera.X = centerPoint.X - screenSize.X*(1-part);
			}else if (_buttonRight)
			{
				if (posOnScreen + _velocity.X*timeDiff > screenSize.X*(1-threshold))
				{_cameraHDir=HDirRight;}
			}
		}else if (_cameraHDir=HDirRight)
		{
			if (_buttonLeft)
			{
				if (posOnScreen - _velocity.X*timeDiff < screenSize.X*threshold)
				{_cameraHDir=HDirLeft;}
			}
			else if(_buttonRight){
				if(posOnScreen - _velocity.X*timeDiff - 2 > screenSize.X*part)
					camera.X += _velocity.X * timeDiff * speed;
				else if (posOnScreen > screenSize.X*part)
					camera.X = centerPoint.X - screenSize.X*part;
			}
		}
		if(posOnScreen < threshold*screenSize.X && !_buttonLeft)
			camera.X = centerPoint.X - threshold*screenSize.X;
		else if(posOnScreen > (1-threshold)*screenSize.X && !_buttonRight)
			camera.X = centerPoint.X - (1-threshold)*screenSize.X;
		if(camera.X < 0) 
		{
			_cameraHDir=HDirNone;
			camera.X = 0;
		}
		else if(camera.X > maxDim.X - screenSize.X)
		{
			camera.X = maxDim.X - screenSize.X;
			_cameraHDir=HDirNone;
		}
	}
	if(maxDim.Y < screenSize.Y){
		camera.Y=0;//= maxDim.Y-screenSize.Y; //With this, a black strip on the bottom would never appear. Although, no tiles either.
	}else{
		if(centerPoint.Y <= screenSize.Y / 2)
			camera.Y = 0;
		else if(centerPoint.Y >= maxDim.Y - screenSize.Y / 2)
			camera.Y = maxDim.Y - screenSize.Y;
		else 
			camera.Y = centerPoint.Y - screenSize.Y / 2;
	}
	map->SetMapPosition(camera.X,camera.Y);
}
void Player::Update(Map* map, int screenWidth, int screenHeight,Sounds& sounds, long lastTick){
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
	
	float drawDistance=(float)map->GetDrawDistance()/20;
	//_totalInkReceived+=timeDiff;
	float tempInkPool=_totalInkReceived-drawDistance;
	if (tempInkPool > _maxInkPool)
	{InkPool=(float)_maxInkPool; _totalInkReceived=drawDistance+_maxInkPool;}
	else if(InkPool<0)
	{InkPool=0;_totalInkReceived=drawDistance;}
	else
	{InkPool=tempInkPool;}
	if(Health < 0)Health=0;
	else if(Health > _maxHealth) Health = (float)_maxHealth;

	if(InvulnerableTime > 0) InvulnerableTime -= timeDiff;
	else InvulnerableTime = 0;
	//if(Health < _maxHealth) Health++;

	//For the jump
	if(_velocity.Y < _maxVelocity) 
		_velocity.Y+=_maxVelocity*timeDiff;
	//Initiate jump if button up is pressed
	if(_buttonUp && _jumpEnable) {
		_velocity.Y = (float)-_maxVelocity; 
		_jumpEnable = false; 
	}
	//Handles walking and collision
	HandleCollision(map, screenWidth, screenHeight, timeDiff,sounds);
	//map->SetNewMapPosition(Point2D((float)screenWidth, (float)screenHeight), GetCenter());
	
	SetMapPosition(map, Point2D((float)screenWidth, (float)screenHeight), timeDiff, 0.45f,0.2f, 1.4f);

	//Prevents the player from walking out of screen
	Point2D mapDim = map->GetMapDimension();
	if(_position.X - map->GetMapPosition().X < 0) 
		_position.X = map->GetMapPosition().X ;
	else if(mapDim.X > screenWidth)
	{
		if(_position.X + _spriteDimension.X - map->GetMapPosition().X > screenWidth)
			_position.X = screenWidth - _spriteDimension.X +  map->GetMapPosition().X;
	}
	else if(_position.X + _spriteDimension.X > mapDim.X)
		_position.X = mapDim.X - _spriteDimension.X;
	
	if(_position.Y -  map->GetMapPosition().Y< 0){
		_position.Y = map->GetMapPosition().Y;
		_velocity.Y = 0;
	}
	else if(mapDim.Y > screenHeight){
		if(_position.Y + _spriteDimension.Y -  map->GetMapPosition().Y > screenHeight){
			_position.Y = screenHeight - _spriteDimension.Y +  map->GetMapPosition().Y;
			_jumpEnable = true;
			Health = 0;
		}
	}
	else if(_position.Y + _spriteDimension.X > mapDim.Y){
		_position.Y = mapDim.Y - _spriteDimension.Y;
		_jumpEnable = true;
		Health = 0;
	}
	if(_velocity.Y < 0) _vDir = VDirUp;
	else if(_velocity.Y>0)_vDir=VDirDown;
	else _vDir = VDirNone;
	tail.Update(GetBoundR(-map->GetMapPosition().X, -map->GetMapPosition().Y),_hDir);

	
	//Ik wed dat deze functie beter kan, dan van float->int->float->int te gaan.
	//Als ik nu eens snapte waar deze functie voor was
	int X1 = (int)(_position.X/map->GetTileDimension().X);
	int Y1 = (int)(_position.Y/map->GetTileDimension().Y);
	int X2 = (int)((_position.X + _spriteDimension.X)/map->GetTileDimension().X);
	int Y2 = (int)((_position.Y + _spriteDimension.Y)/map->GetTileDimension().Y);

}
void Player::Jump(){
	_velocity.Y = (float)-_maxVelocity; 
	_jumpEnable = false; 
}
float Player::HealthRatio(){
	return ((float)Health)/((float)_maxHealth);
}
float Player::InkPoolRatio(){
	return ((float)InkPool)/((float)_maxInkPool);
}
void Player::DrawHealthBar(WindowSurface screen, int border, unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height, Font* font){
	if(Health < 0)Health=0;
	else if(Health > _maxHealth) Health = (float)_maxHealth;
	if(border <= 0)border = 0;
	
	screen.DrawFilledRect(X, Y, X + Width + 2*border, Y + Height+2*border, 50, 50, 50);
	screen.DrawFilledRect(X+border, Y+border, X+border + (int)(Width * HealthRatio()), Y +border+ Height, 255, 0, 0);
	
	if(font != 0){
		font->SetColor(255,255,255);
		Surface render;
		std::stringstream ss; ss << Health << "/" << _maxHealth;
		render.RenderText(*font, ss.str());
		render.Draw(screen, (Uint32)(((float)X+border) + ((float)Width)/2 - render.GetWidth()/2), (Uint32)(((float)Y+border) + ((float)Height)/2 - render.GetHeight()/2));
		render.Free();
	}
}
void Player::DrawInkBar(WindowSurface screen, int border, unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height, Font* font){
	if(InkPool < 0)InkPool=0;
	else if(InkPool > _maxInkPool) InkPool = (float)_maxInkPool;
	if(border <= 0)border = 0;
	screen.DrawFilledRect(X, Y, X + Width + 2*border, Y+ Height+2*border, 50, 50, 100);
	screen.DrawFilledRect(X+border, Y+border, X+border + (int)(Width * InkPoolRatio()), Y +border+ Height, 2, 20, 2);
	if(font != 0){
		font->SetColor(100,100,255);
		Surface render;
		std::stringstream ss; ss << (unsigned int)InkPool << "/" << _maxInkPool;
		render.RenderText(*font, ss.str());
		render.Draw(screen, (Uint32)(((float)X+border) + ((float)Width)/2 - render.GetWidth()/2), (Uint32)(((float)Y+border) + ((float)Height)/2 - render.GetHeight()/2));
		render.Free();
	}
}
void Player::Reset(Point2D position, bool resetStats){
	_position = position;
	_velocity.Y = 50;
	_jumpEnable = true;
	if(resetStats){
		InkPool = (float)_maxInkPool;
		_totalInkReceived=(float)_maxInkPool;
		Health  =(float) _maxHealth;
		InvulnerableTime = 0;
	}
}
Point2D Player::GetPreviousPosition(){ return _previousPosition; } 
Rectangle Player::GetPreviousBoundR(){return Rectangle(_previousPosition.X, _previousPosition.Y, _spriteDimension.X, _spriteDimension.Y); }
void Player::Draw(WindowSurface screen, Point2D mapPosition)
{
	//Draws the player on the screen
	_surface->Draw(screen, (Sint16)(_position.X - mapPosition.X), (Sint16)(_position.Y - mapPosition.Y), &GetFrame());
	tail.Draw(screen);
	Rectangle playerBounds=GetBoundR(-mapPosition.X, -mapPosition.Y);
	aaellipseRGBA(screen,(Sint16)(playerBounds.X+0.5*playerBounds.W),(Sint16)(playerBounds.Y+0.5*playerBounds.H),100,100,255,255,255,255);
}
void Player::HandleCollision(Map* map, int screenWidth, int screenHeight, float timeDiff,Sounds& sound){
	//The velocities (X and Y) must be smaller than the tile dimensions and the sprite dimensions
	if(_buttonLeft || _buttonRight){
		int X, XO;
		if(_buttonLeft){
			//The new edge columns the player is in. (X=left edge. X0=right edge)
			X = (int)((_position.X - _velocity.X*timeDiff) / map->GetTileDimension().X);
			XO = (int)((_position.X + _spriteDimension.X - _velocity.X*timeDiff) / map->GetTileDimension().X);
		}
		else {
			//The new edge columns the player is in. (X=right edge. X0=left edge)
			X = (int)((_position.X + _velocity.X*timeDiff + _spriteDimension.X) / map->GetTileDimension().X);
			XO = (int)((_position.X + _velocity.X*timeDiff) / map->GetTileDimension().X);
		}
		int Y1 = (int)(_position.Y / map->GetTileDimension().Y); // Top edge of sprite
		int Y2 = (int)((_position.Y + _spriteDimension.Y - 1) / map->GetTileDimension().Y); //bottom edge of sprite
			
		Point2D top((float)X, (float)Y1);
		Point2D bot((float)X, (float)Y2);
		TileType charTypeTop = map->GetCharType(top);
		TileType charTypeBot = map->GetCharType(bot);
		TileType charTypeBotO = map->GetCharType(Point2D((float)XO, (float)Y2));
		//Check if you hit enemies by the side (enemies facing towards player)
		if(InvulnerableTime <= 0){
			TileData t1 = map->GetTileData(X, Y1);
			TileData t2 = map->GetTileData(X, Y2);
			if((t1.GetType() == TileTypeSpike && t2.GetType() == TileTypeSpike) || 
				(t1.GetType() == TileTypeSpike && t2.GetType() == TileTypeNone) || 
				(t1.GetType() == TileTypeNone && t2.GetType() == TileTypeSpike))
			{
				TileSides must = (_buttonLeft)?TSright:TSleft;
				if(t1.Side == must || t2.Side == must){
					InvulnerableTime = 2;	
					Health -= _maxHealth/2;
					sound.damage.Play(false);
				}
			}
		}

		//If top and bot of the new X don't hit anything, the player can freely move to left or right
		if(charTypeTop ==  TileTypeNone && charTypeBot ==  TileTypeNone){
			if(_buttonLeft){_position.X -= _velocity.X*timeDiff;}
			else {_position.X += _velocity.X*timeDiff;}
		}
		//If the player new bot isn't on a slope, but was, and the top still is
		else if(charTypeBot != TileTypeSlope && charTypeTop == TileTypeSlope && charTypeBotO == TileTypeSlope){
			//Get the previous slope
			TileData td = map->GetTileData(XO, Y2);
			int y1, y2; td.GetSlope(y1, y2);
			//If the slope is going up from the player (so:  \<-@ || @->/)
			if((_buttonLeft && y1>y2)||(_buttonRight&&y2>y1)){
				if(_buttonLeft){_position.X -= _velocity.X*timeDiff;}
				else {_position.X += _velocity.X*timeDiff;}
				float h = 0;
				if(_buttonLeft) h = map->GetSlopeHeight(Point2D(_position.X, _position.Y));
				else h = map->GetSlopeHeight(Point2D(_position.X + _spriteDimension.X, _position.Y));
				_position.Y = Y2*map->GetTileDimension().Y - _spriteDimension.Y;
			}
		}
		else if(charTypeBot == TileTypeNormal && charTypeBotO == TileTypeSlope){
			TileData td = map->GetTileData(XO, Y2);
			int y1, y2; td.GetSlope(y1, y2);
			if((_buttonLeft && y1>y2)||(_buttonRight&&y2>y1)){
				int yv = (_buttonLeft)?y1:y2;
				_position.Y = Y2*map->GetTileDimension().Y - _spriteDimension.Y;
				if(_buttonLeft){_position.X -= _velocity.X*timeDiff;}
				else {_position.X += _velocity.X*timeDiff;}
			}

		}
		//If it is a slope
		else if(charTypeBot == TileTypeSlope){
			//velocity defference
			float dx = _buttonLeft?_position.X - _velocity.X * timeDiff:_position.X+_velocity.X*timeDiff;
			int X1 = (int)(dx/map->GetTileDimension().X);
			int X2 = (int)((dx + _spriteDimension.X)/map->GetTileDimension().X);
			//Get tile data
			TileData slopeLeft = map->GetTileData(X1, Y2);
			TileData slopeRight = map->GetTileData(X2, Y2);
			TileData slopeTop = _buttonLeft?map->GetTileData(X1, Y1):map->GetTileData(X2, Y1);
			if(Y1<Y2 && charTypeTop == TileTypeSlope && (top.X != bot.X || top.Y != bot.Y )){
				if(_buttonLeft)
					_position.X -= (X+1) * map->GetTileDimension().X ;
				else 
					_position.X = X*map->GetTileDimension().X-_spriteDimension.X;
			}
			else if(charTypeTop ==  TileTypeDrawing || map->GetCharType(Point2D((float)X, (float)((int)((_position.Y-2) / map->GetTileDimension().Y))))==  TileTypeDrawing){
				float dx = _buttonLeft?-_velocity.X*timeDiff:_velocity.X*timeDiff;
				float h = map->CheckDrawCollision(GetBoundR(-map->GetMapPosition().X + dx, -map->GetMapPosition().Y));
				if(h <= 0){
					_position.X += dx;
				}
				else if(_velocity.Y > 0 && h < 4){
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
				float hl = map->GetHeightAtPosition(Point2D(dx, _position.Y));
				float hr = map->GetHeightAtPosition(Point2D(dx+_spriteDimension.X, _position.Y));
				//IF player hits the high side of a slope, so he can walk through
				if(_buttonLeft && (int)(_position.X/map->GetTileDimension().X)>X1 &&
					Y2*map->GetTileDimension().Y-_position.Y<=ly2 && ly2 > ry1 && abs(ly2 - ry1) > 1){
					_position.X = (X1+1)*map->GetTileDimension().X;
				}
				//IF player hits the high side of a slope, so he can walk through
				else if(_buttonRight && (int)((_position.X+_spriteDimension.X)/map->GetTileDimension().X)<X2 && 
					Y2*map->GetTileDimension().Y-_position.Y<=ry1 && ry1 > ly2 && abs(ly2 - ry1) > 1){
					_position.X = X2*map->GetTileDimension().X-_spriteDimension.X-1;
				}
				else {
					float newpos;
					bool handled = false;
					//walking up a downward slope: \ <-direction
					if(ly1 > ly2 && _buttonLeft && hl > _spriteDimension.Y){ //down
						float diff=_position.X - (X1 * map->GetTileDimension().X);
						float ratio = (ly1-ly2)/map->GetTileDimension().X;
						newpos = Y2 * map->GetTileDimension().Y + diff * ratio - _spriteDimension.Y - ly1 + map->GetTileDimension().Y;
						handled = true;
					}
					//walking up a downward slope: direction-> /
					else if(ry1 < ry2 && _buttonRight && hr>_spriteDimension.Y){ //Up
						float diff = _position.X+_spriteDimension.X-(X2*map->GetTileDimension().X);
						float ratio = (ry2-ry1)/map->GetTileDimension().X;
							
						newpos = Y2 * map->GetTileDimension().Y + map->GetTileDimension().Y - diff * ratio - _spriteDimension.Y;
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
		else if(charTypeBot ==  TileTypeDrawing || charTypeTop ==  TileTypeDrawing){
			if((charTypeBot ==  TileTypeDrawing && (charTypeTop ==  TileTypeNone || charTypeTop ==  TileTypeDrawing) && charTypeTop != TileTypeNormal) ||
				(charTypeTop ==  TileTypeDrawing && (charTypeBot ==  TileTypeNone || charTypeBot ==  TileTypeDrawing) && charTypeBot != TileTypeNormal))		{
				float dx = _buttonLeft?-_velocity.X*timeDiff:_velocity.X*timeDiff;
				float h = map->CheckDrawCollision(GetBoundR(-map->GetMapPosition().X + dx, -map->GetMapPosition().Y));
				if(h <= 0){
					_position.X += dx;
				}
				else if(_velocity.Y > 0 && h < 4 && 
					map->CheckDrawCollision(GetBoundR(-map->GetMapPosition().X + dx, -map->GetMapPosition().Y - h - 1)) <= 0){
					_position.X += dx;
					_position.Y -= h;
				}
			}
		}
		else if(charTypeBot == TileTypeNone && charTypeTop == TileTypeSlope){
			TileData td = map->GetTileData(XO, Y2);
			int y1, y2; td.GetSlope(y1, y2);
			if((_buttonLeft && y1>y2)||(_buttonRight&&y2>y1)){
				if(_buttonLeft){_position.X -= _velocity.X*timeDiff;}
				else {_position.X += _velocity.X*timeDiff;}
				//_position.Y -= 10;
				_velocity.Y = 0;
				_jumpEnable=true;
			}
		}
		//Minimal one edge hit a block, so now it can only move the difference between the block and the player
		else{
			if(_buttonLeft){
				double xDiff = abs(_position.X - ((X+1) * map->GetTileDimension().X));
				_position.X -= (float)(xDiff-1);
			}
			else {
				double xDiff = abs(_position.X + _spriteDimension.X - (X*map->GetTileDimension().X));
				_position.X += (float)(xDiff-1);
			}
		}
	}
	if(_velocity.Y != 0){
	int Y1 = (int)((_position.Y + _velocity.Y*timeDiff) / map->GetTileDimension().Y);
	int Y2 = (int)((_position.Y + _velocity.Y*timeDiff + _spriteDimension.Y - 1) / map->GetTileDimension().Y);
	int Y = (_velocity.Y < 0)?Y1:Y2;
	//The left edge x coord
	int X1 = (int)(_position.X / map->GetTileDimension().X);
	//The right edge x coord
	int X2 = (int)((_position.X + _spriteDimension.X) / map->GetTileDimension().X);
	Point2D left((float)X1, (float)Y);
	Point2D right((float)X2, (float)Y);
	//IF both edges dont hit anything, the player can walk freely upwards or downwards
	int charTypeLeft = map->GetCharType(left);
	int charTypeRight = map->GetCharType(right);
	int charTypeLeft2 = map->GetCharType(Point2D((float)X1, (float)Y2));
	int charTypeRight2 = map->GetCharType(Point2D((float)X2, (float)Y2));
	if(charTypeLeft ==  TileTypeNone && charTypeRight ==  TileTypeNone && charTypeLeft2 ==  TileTypeNone && charTypeRight2 ==  TileTypeNone){
		_position.Y += _velocity.Y*timeDiff;
	}
	//The player hit a block, so now it can only walk the difference between player and the block
	else {
		if(_velocity.Y < 0){
			int Yt = (int)((_position.Y) / map->GetTileDimension().Y);
			int botLeft = map->GetCharType(Point2D((float)X1, (float)Yt));
			int botRight = map->GetCharType(Point2D((float)X2, (float)Yt));/*
			if(((charTypeLeft == 2 || charTypeLeft == 3 || charTypeRight == 2 || charTypeRight == 3) && botLeft ==  TileTypeNone && botRight ==  TileTypeNone) ||
				(charTypeLeft == 2 && charTypeRight == 2) || (charTypeLeft == 3 && charTypeRight==3)) {
				//_position.Y = (Y+1)*map->GetTileDimension().Y;
				_velocity.Y = 0;
			}
			else */
			if(InvulnerableTime <= 0){
				TileData t1 = map->GetTileData(X1, Y1);
				TileData t2 = map->GetTileData(X2, Y1);
			if((t1.GetType() == TileTypeSpike && t2.GetType() == TileTypeSpike) || 
				(t1.GetType() == TileTypeSpike && t2.GetType() == TileTypeNone) || 
				(t1.GetType() == TileTypeNone && t2.GetType() == TileTypeSpike)) {
					if(t1.Side == TSbottom || t2.Side == TSbottom){
						InvulnerableTime = 2;	
						Health -= _maxHealth/2;
						sound.damage.Play(false);
					}
				}
			}
			if(charTypeLeft == TileTypeNormal || charTypeRight == TileTypeNormal || ((charTypeLeft == TileTypeSlope || charTypeRight == TileTypeSlope) && (Yt>Y1)) ){
				_velocity.Y = 0;
				_position.Y = (Y+1)*map->GetTileDimension().Y;
			}
				//_position.Y += _velocity.Y*timeDiff;
			else if(charTypeLeft ==  TileTypeDrawing || charTypeLeft2 ==  TileTypeDrawing || charTypeRight ==  TileTypeDrawing || charTypeRight2 ==  TileTypeDrawing){
				if (!map->CheckDrawCollision(GetBoundR(-map->GetMapPosition().X, -map->GetMapPosition().Y+_velocity.Y*timeDiff)))
					_position.Y += _velocity.Y*timeDiff;
				else _velocity.Y = 0;
			}
			else _position.Y += _velocity.Y*timeDiff;
		}
		else {
			if(InvulnerableTime <= 0){
				TileData t1 = map->GetTileData(X1, Y2);
				TileData t2 = map->GetTileData(X2, Y2);
			if((t1.GetType() == TileTypeSpike && t2.GetType() == TileTypeSpike) || 
				(t1.GetType() == TileTypeSpike && t2.GetType() == TileTypeNone) || 
				(t1.GetType() == TileTypeNone && t2.GetType() == TileTypeSpike)){
					if(t1.Side == TStop || t2.Side == TStop){
						InvulnerableTime = 2;	
						Health -= _maxHealth/2;
						sound.damage.Play(false);
					}
				}
			}
			if(charTypeLeft == TileTypeNone && charTypeRight == TileTypeNone){
				_position.Y += _velocity.Y*timeDiff;
			}
			else if(charTypeLeft == TileTypeNormal || charTypeRight == TileTypeNormal){
				_velocity.Y = 50;
				_jumpEnable = true;
			}
			else if((charTypeLeft ==  TileTypeNone && map->GetCharType(Point2D((float)X1, (float)Y1)) == TileTypeSlope) || 
				charTypeRight ==  TileTypeNone && map->GetCharType(Point2D((float)X2, (float)Y1)) == TileTypeSlope){

			}
			else if(charTypeLeft == TileTypeSlope || charTypeRight == TileTypeSlope){
				//_position.Y += _velocity.Y*timeDiff;
				TileData tl = map->GetTileData((int)left.X, (int)left.Y);
				TileData tr = map->GetTileData((int)right.X, (int)right.Y);
				int yl1, yl2; tl.GetSlope(yl1, yl2);
				int yr1, yr2; tr.GetSlope(yr1, yr2);
				if((charTypeRight == TileTypeSlope && charTypeLeft ==  TileTypeDrawing) || (charTypeLeft == TileTypeSlope && charTypeRight ==  TileTypeDrawing)){
					float height = (charTypeLeft==TileTypeSlope)?map->GetSlopeHeight(Point2D(_position.X, _position.Y+_spriteDimension.Y + _velocity.Y*timeDiff)):
						map->GetSlopeHeight(Point2D(_position.X+_spriteDimension.X, _position.Y+_spriteDimension.Y + _velocity.Y*timeDiff));
					float newpos= (Y2*map->GetTileDimension().Y) + height - _spriteDimension.Y;
						
					if(!map->CheckDrawCollision(GetBoundR(-map->GetMapPosition().X, -map->GetMapPosition().Y + newpos - _position.Y))){
						if(_position.Y + _velocity.Y*timeDiff < newpos)
							_position.Y += _velocity.Y*timeDiff;
						else {
								_position.Y = newpos;
								_velocity.Y = 50;
								_jumpEnable=true;
						}
					}
				}
					
				//on top of 1 or 2 slopes / or \ or /\ 
				else if((charTypeRight == charTypeLeft && yl2 != yr1 && (int)left.X != (int)right.X) || // if 2 of the same slopes to each other // or \\ 
					(charTypeLeft == TileTypeSlope && yl2>yl1 &&  (charTypeRight != TileTypeSlope || (charTypeRight == TileTypeSlope && yr1>yr2))) || 
					(charTypeRight == TileTypeSlope && yr1>yr2&&(charTypeLeft!=TileTypeSlope || (charTypeLeft==TileTypeSlope&&yl2>yl1)))){
					int h = yl2>yr1?yl2:yr1;
					float newpos = ((Y+1)*map->GetTileDimension().Y) - _spriteDimension.Y - h;
					if(newpos >= _position.Y+_velocity.Y*timeDiff)
						_position.Y += _velocity.Y*timeDiff;
					else {
						_position.Y = newpos;
						_velocity.Y = 50;
						_jumpEnable = true;
					}
				}
				else if((charTypeLeft == TileTypeSlope && yl1>yl2) || (charTypeRight == TileTypeSlope && yr1<yr2)){ //Downwards
					float newpos;
					//on top of 2 downward slopes \/
					if((charTypeLeft == TileTypeSlope && yl1>yl2)&&(charTypeRight == TileTypeSlope && yr1<yr2)){
						float hl = map->GetSlopeHeight(Point2D(_position.X, _position.Y+_spriteDimension.Y + _velocity.Y*timeDiff));
						float hr = map->GetSlopeHeight(Point2D(_position.X+_spriteDimension.X, _position.Y+_spriteDimension.Y + _velocity.Y*timeDiff));
						newpos = hl<hr?(Y*map->GetTileDimension().Y) + hl:(Y*map->GetTileDimension().Y) + hr;
					}
					else {
						//normal slope \ or /
						float h = (charTypeLeft == TileTypeSlope && yl1>yl2)?map->GetSlopeHeight(Point2D(_position.X, _position.Y+_spriteDimension.Y + _velocity.Y*timeDiff)):map->GetSlopeHeight(Point2D(_position.X+_spriteDimension.X, _position.Y+_spriteDimension.Y + _velocity.Y*timeDiff));
						newpos = (Y*map->GetTileDimension().Y) + h;
					}

					//Set the new position
					if(newpos - _spriteDimension.Y >= _position.Y+_velocity.Y*timeDiff){
						_position.Y += _velocity.Y*timeDiff;
					}
					/*
					else if((_buttonLeft||_buttonRight)) {
						if((abs(_position.Y - newpos + _spriteDimension.Y) < 8))
						_position.Y = newpos - _spriteDimension.Y;
						//else _position.Y -= 5;
						_velocity.Y = 50;
						_jumpEnable = true;
					}*/
					else {
							
						_velocity.Y = 50;
						_jumpEnable = true;
					}
				}
			}
			else if(charTypeLeft ==  TileTypeDrawing  || charTypeRight ==  TileTypeDrawing){
				if(!map->CheckDrawCollision(GetBoundR(-map->GetMapPosition().X, -map->GetMapPosition().Y + _velocity.Y*timeDiff)))
					_position.Y += _velocity.Y*timeDiff;
				else {
					_velocity.Y = 50;
					_jumpEnable = true;}
			}
		}
	}
	}
	_previousPosition=_position;
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
	Rectangle frames(((_frame % _spriteX) * _spriteDimension.X),((_frame / _spriteY+1) * _spriteDimension.Y),_spriteDimension.X,_spriteDimension.Y);
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
	if(keystates[SDLK_DOWN]||keystates[SDLK_s]) _buttonDown = true; else _buttonDown = false;
	if(keystates[SDLK_UP]||keystates[SDLK_w]) _buttonUp = true; else _buttonUp = false;
	if(keystates[SDLK_LEFT]||keystates[SDLK_a]) _buttonLeft = true; else _buttonLeft = false;
	if(keystates[SDLK_RIGHT]||keystates[SDLK_d]) _buttonRight = true; else _buttonRight = false;
	tail.HandleEvent(sEvent);
}