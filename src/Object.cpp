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
void Object::SetPosition(Point2D position){_position = position; }
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