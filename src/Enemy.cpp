#include "Enemy.h"
Enemy::Enemy(EnemyType type, Surface* surface, float X, float Y, int interval, int spriteX, int spriteY):Object(surface, X, Y, spriteX, spriteY){
	_interval = interval;
	if(_interval <= 0) _interval = 1;
	_countInterval = 0;
	_animationState = 0;
	_frame = 0;
	_type = type;
	CanWalkOff=true;
	CanWalkSlope=true;
}
void Enemy::Update(){
	if(_countInterval > _interval){
		_countInterval = 0;
		_animationState++;
		if(_animationState > 2)
			_animationState = 0;
	}
	if(_velocity.X <= 0) _frame=0+_animationState;
	else _frame=3+_animationState;
	_countInterval++;
}
void Enemy::Draw(Window screen, Point2D mapPosition){
	_surface->Draw(screen, (Sint32)(_position.X - mapPosition.X), (Sint32)(_position.Y - mapPosition.Y), &GetFrame());
}
Point2D Enemy::GetCenter(){
	return Point2D(_position.X + _spriteDimension.X / 2, _position.Y + _spriteDimension.Y/2);
}
SDL_Rect Enemy::GetFrame(){
	SDL_Rect rect;
	rect.x = (Sint16)((_frame % _spriteX) * _spriteDimension.X);
	rect.y = (Sint16)((_frame / (_spriteY+1)) * _spriteDimension.Y);
	rect.w = (Sint16)_spriteDimension.X;
	rect.h = (Sint16)_spriteDimension.Y;
	return rect;
}
Rectangle Enemy::GetFrameR(){
	Rectangle frames(((_frame % _spriteX) * _spriteDimension.X),((_frame / _spriteY+1) * _spriteDimension.Y),_spriteDimension.X,_spriteDimension.Y);
	return frames;
}
void Enemy::SetFrame(int frame){
	int maxFrame=_spriteX*_spriteY;
	_frame = frame;
	if(_frame<0){_frame=0;}
	else if(_frame > maxFrame){_frame=maxFrame;}
}

EnemyHandler::EnemyHandler(){
	enemyChars.push_back('1');
	enemyChars.push_back('2');
	enemyChars.push_back('3');
	
	counter=0;
}
void EnemyHandler::AddEnemy(EnemyType type, Graphics* graphics, Point2D position){
	Surface* surface = 0; Point2D velocity; bool canwalkoff = true; bool canwalkslope = true;
	switch(type){
	case EnemyBlob:
		surface = &graphics->enemy[1];
		velocity = Point2D(100, 100);
		canwalkoff = false; 
		canwalkslope = true;
		break;
	case EnemyFire:
		surface = &graphics->enemy[2];
		velocity = Point2D(0, 100);
		canwalkoff = false; 
		canwalkslope = false;
		break;
	case EnemySpider:
		surface = &graphics->enemy[3];
		velocity = Point2D(100, 100);
		canwalkoff = false; 
		canwalkslope = true;
		break;
	default:
		surface = &graphics->enemy[1];
		velocity = Point2D(100, 100);
		canwalkoff = false; 
		canwalkslope = false;
		break;
	}
	Enemy newE(type, surface, position.X, position.Y, 3, 3, 2);
	newE.SetVelocity(velocity.X, velocity.Y);
	newE.CanWalkOff=canwalkoff;
	newE.CanWalkSlope=canwalkslope;
	_enemyList.push_back(newE);
}
void EnemyHandler::Update(Map* map, Player* player,Sounds& sound, long timer, Graphics* graphics){
	if (counter>100)
	{
 		std::vector<std::string> mapArray = map->GetMapArray();
		std::string str;
		char chararcter;
		for(unsigned int y = 0; y < mapArray.size(); y++){
			str = mapArray.at(y);
			for(unsigned int x = 0; x < str.length(); x++){
				chararcter = str[x];
				for(unsigned int i = 0; i < enemyChars.size(); i++){
					if(enemyChars[i]==chararcter){
						AddEnemy((EnemyType)i, graphics, Point2D((float)x, (float)y)*map->GetTileDimension());
					}
				}
			}

		}
		counter=0;
	}
	counter++;
	
	float timeDiff=timer<0?1:(clock()-timer)/1000.0f;
	for(unsigned int i = 0; i <_enemyList.size(); i++){
		Enemy* enemy = &_enemyList.at(i);
		enemy->Update();
		bool update = true;
		if(enemy->GetBoundR().Intersect(player->GetBoundR())){
				//#PLAYER KILLS ENEMY -> update false, remove enemy
				//#ELSE PLAYER LOSES LIFE
				//player hits enemy
			if(player->InvulnerableTime <= 0){
				if(player->GetVelocity().Y > 60){
					int height =(int)(Minimum(player->GetBoundR().Y + player->GetBoundR().H, enemy->GetBoundR().Y + enemy->GetBoundR().H) - Maximum(player->GetBoundR().Y, enemy->GetBoundR().Y));
					int width = (int)(Minimum(player->GetBoundR().X + player->GetBoundR().W, enemy->GetBoundR().X + enemy->GetBoundR().W) - Maximum(player->GetBoundR().X, enemy->GetBoundR().X));
					
					if(player->GetPreviousPosition().Y + player->GetBoundR().Y < enemy->GetBoundR().Y || height <= width){
						_enemyList.erase(_enemyList.begin() + i);
						player->AddInk(20);
						player->Jump();
						player->InvulnerableTime = 0.2f;
						sound.enemyDeath.Play(false);
						update = false;
					}
					else {
						player->Health -= 10;
						player->InvulnerableTime = 2;
						sound.damage.Play(false);
						
					}
				}
				else {
					player->Health -= 10;
					player->InvulnerableTime = 2;
					sound.damage.Play(false);
				}
			}
		}
		if(update){ //enemy can walk
			Point2D velocity = enemy->GetVelocity();
			Point2D position = enemy->GetPosition();
			int Y1 = (int)((position.Y+1) / map->GetTileDimension().Y);
			int Y2 = (int)((position.Y + enemy->GetBound().h-1) / map->GetTileDimension().Y);
			int X1 = (int)(position.X / map->GetTileDimension().X);
			int X2 = (int)((position.X + enemy->GetBound().w) / map->GetTileDimension().X);
			int tl = map->GetCharType(Point2D((float)X1, (float)Y1));
			int tr = map->GetCharType(Point2D((float)X2, (float)Y1));
			int bl = map->GetCharType(Point2D((float)X1, (float)Y2));
			int br = map->GetCharType(Point2D((float)X2, (float)Y2));
			if(enemy->GetVelocity().Y != 0){
				if(velocity.Y > 0){
					if(map->GetMapDimension().Y < position.Y + enemy->GetBound().h){
						position.Y = map->GetMapDimension().Y -enemy->GetBound().h+1;
					}
					else {
					if((bl == TileTypeNormal && br == TileTypeSlope) || (bl == TileTypeSlope && br == TileTypeNormal)){
						position.Y = (Y2)*map->GetTileDimension().Y-enemy->GetBound().h+1;

					}
					else if(bl == TileTypeSlope || br == TileTypeSlope){
						TileData blData = map->GetTileData(X1, Y2); TileData brData = map->GetTileData(X2, Y2);
						int yl1=0, yl2=0, yr1=0, yr2=0; blData.GetSlope(yl1, yl2); brData.GetSlope(yr1, yr2);
						if(bl == TileTypeSlope && br == TileTypeSlope){
							if(yl1 < yl2 && yr1 > yr2){
								float high = (float)(yr1>yl2?yr1:yl2);
								float newpos = (Y2+1)*map->GetTileDimension().Y - high - enemy->GetBound().h;
								if(position.Y > newpos) position.Y = newpos;
								else position.Y += velocity.Y*timeDiff;
							}
							else if(bl == TileTypeSlope && yl1 > yl2){
								float h1 = map->GetSlopeHeight(position + Point2D(0, enemy->GetBound().h));
								float newpos = (Y2*map->GetTileDimension().Y)+h1-enemy->GetBound().h;
								if(position.Y > newpos) position.Y = newpos;
								else position.Y += velocity.Y*timeDiff;
							}
							else if(br == TileTypeSlope && yr1 < yr2){
								float h2 = map->GetSlopeHeight(position + Point2D(enemy->GetBound().w, enemy->GetBound().h));
								float newpos = (Y2*map->GetTileDimension().Y)+h2-enemy->GetBound().h;
								if(position.Y > newpos) position.Y = newpos;
								else position.Y += velocity.Y*timeDiff;
							}
							else{
								float h1 = map->GetSlopeHeight(position + Point2D(0, enemy->GetBound().h));
								float h2 = map->GetSlopeHeight(position + Point2D(enemy->GetBound().w, enemy->GetBound().h));
								float high = h1>h2?h1:h2;
								float newpos = (Y2*map->GetTileDimension().Y)+high-enemy->GetBound().h;
								if(position.Y > newpos) position.Y = newpos;
								else position.Y += velocity.Y*timeDiff;
							}
						}
						else if(bl == TileTypeNormal || br == TileTypeNormal){
							position.Y = (Y2)*map->GetTileDimension().Y-enemy->GetBound().h-1;
						}
						else if(bl == TileTypeSlope && yl1 > yl2){
							float h = map->GetSlopeHeight(position + Point2D(0, enemy->GetBound().h));
							float newpos = (Y2*map->GetTileDimension().Y)+h-enemy->GetBound().h;
							if(position.Y > newpos) position.Y = newpos;
							else position.Y += velocity.Y*timeDiff;
						}
						else if(br == TileTypeSlope&& yr1 < yr2){
							float h = map->GetSlopeHeight(position + Point2D(enemy->GetBound().w, enemy->GetBound().h));
							float newpos = (Y2*map->GetTileDimension().Y)+h-enemy->GetBound().h;
							if(position.Y > newpos) position.Y = newpos;
							else position.Y += velocity.Y*timeDiff;
						}
					}
					
					else if(tl != TileTypeNormal && tr != TileTypeNormal && (bl == TileTypeNormal || br == TileTypeNormal)){
						position.Y = (Y2)*map->GetTileDimension().Y-enemy->GetBound().h + 1;
					}
					else if(bl == TileTypeNormal || br == TileTypeNormal) 
						position.Y = (Y2)*map->GetTileDimension().Y-enemy->GetBound().h+1;
					else 
						position.Y += velocity.Y*timeDiff;
					}
				}
			}
			if(enemy->GetVelocity().X != 0){
				if(bl == TileTypeDrawing)bl=TileTypeNone;
				if(br == TileTypeDrawing)br=TileTypeNone;
				if(tl == TileTypeDrawing)tl=TileTypeNone;
				if(tr == TileTypeDrawing)tr=TileTypeNone;				
				if(velocity.X > 0){

					TileData blData = map->GetTileData(X2, Y1); TileData brData = map->GetTileData(X2, Y2);
					TileType bTile = map->GetCharType(Point2D((float)X2, (float)Y2+1));
					int yl1, yl2, yr1, yr2; blData.GetSlope(yl1, yl2); brData.GetSlope(yr1, yr2);
					if(map->GetMapDimension().X < position.X + enemy->GetBound().w) velocity.X *= -1;
					else if((br != TileTypeNormal) && bl != TileTypeNone && bl != TileTypeDrawing && !enemy->CanWalkOff){
						if(!(br ==TileTypeSlope && enemy->CanWalkSlope))
							if((bl == TileTypeSlope && (bTile == TileTypeNone||bTile==TileTypeDrawing)) || bl != TileTypeSlope)
								velocity.X *= -1;
					}
					else if(br == TileTypeSlope && (bTile==TileTypeNone||bTile==TileTypeDrawing) && (bl != TileTypeNone ||bl!=TileTypeDrawing)&& !enemy->CanWalkSlope)velocity.X*=-1;
					else if((tr == TileTypeSlope || br == TileTypeSlope) && yl1<yl2){
						if(map->GetHeightAtPosition(Point2D(position.X+enemy->GetBound().w, position.Y)) > enemy->GetBound().h){
							position.Y -= 1;
						}
						else velocity.X *= -1;
					}
					else if(tr == TileTypeNormal || br == TileTypeNormal || (enemy->GetVelocity().Y > 0 && tr == TileTypeNormal) || (enemy->GetVelocity().Y < 0 && br == TileTypeNormal))	{
						if(map->GetCharType(Point2D((float)X2, (float)((position.Y + enemy->GetBound().h - 2) / map->GetTileDimension().Y))) == TileTypeNormal)
							velocity.X *= -1;
					}
				}
				else {
					TileData blData = map->GetTileData(X1, Y1); TileData brData = map->GetTileData(X1, Y2);
					TileType bTile = map->GetCharType(Point2D((float)X1, (float)(Y2+1)));
					int yl1, yl2, yr1, yr2; blData.GetSlope(yl1, yl2); brData.GetSlope(yr1, yr2);
					if(position.X < 0) velocity.X *= -1;
					else if((bl != TileTypeNormal ) && br != TileTypeNone && br != TileTypeDrawing && !enemy->CanWalkOff){
						if(!(bl ==TileTypeSlope && enemy->CanWalkSlope))
							if((br == TileTypeSlope && (bTile == TileTypeNone||bTile==TileTypeDrawing)) || br != TileTypeSlope)
								velocity.X *= -1;
					}
					else if(bl == TileTypeSlope && (bTile==TileTypeNone||bTile==TileTypeDrawing) && (br != TileTypeNone ||br!=TileTypeDrawing) && !enemy->CanWalkSlope)velocity.X*=-1;
					else if((tl == TileTypeSlope || bl == TileTypeSlope) && yl1>yl2){
						if(map->GetHeightAtPosition(Point2D(position.X, position.Y)) > enemy->GetBound().h){
							position.Y -= 1;
						}
						else velocity.X *= -1;
					}
					else if(tl == TileTypeNormal || bl == TileTypeNormal) {
						if(map->GetCharType(Point2D((float)X1, (position.Y + enemy->GetBound().h - 3) / map->GetTileDimension().Y)) == TileTypeNormal)
							velocity.X *= -1;
					}
				}
			}
			position.X += velocity.X*timeDiff;
			enemy->SetPosition(position);
			enemy->SetVelocity(velocity);
		}
	}
}
void EnemyHandler::Draw(Window screen, Point2D mapPosition){
	for(unsigned int i = 0; i <_enemyList.size(); i++){
		_enemyList.at(i).Draw(screen, mapPosition);
	}
}
void EnemyHandler::PopulateEnemies(Map* map, Graphics* graphics){
	_enemyList.clear();
	std::vector<std::string> mapArray = map->GetMapArray();
	std::string str;
	char character;
	for(unsigned int y = 0; y < mapArray.size(); y++){
		str = mapArray.at(y);
		for(unsigned int x = 0; x < str.length(); x++){
			character = str[x];
			for(unsigned int i = 0; i < enemyChars.size(); i++){
				if(enemyChars[i]==character){
					AddEnemy((EnemyType)i, graphics, Point2D((float)x, (float)y)*map->GetTileDimension());
				}
			}
		}

	}
	//map->RemoveEnemiesFromArray(enemyChars);
}