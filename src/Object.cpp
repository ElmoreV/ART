#include "Object.h"
#include "Rectangle.h"
//Constructor
Object::Object(Surface* surface, float X, float Y, int spriteX, int spriteY)
{
	_surface = surface;
	_position.X = X; _position.Y = Y;
	_velocity.X = 0.0f; _velocity.Y = 0.0f;
	_spriteX = spriteX; _spriteY = spriteY;
	_spriteDimension.X = ((float)_surface->GetWidth()) / (float)_spriteX;
	_spriteDimension.Y = ((float)_surface->GetHeight()) / (float)_spriteY;
}
//Get a rectangle of the position and the dimentions
SDL_Rect Object::GetBound(float velocityX, float velocityY)
{
	SDL_Rect bound = {(Sint16)(_position.X+velocityX), (Sint16)(_position.Y+velocityY), (Sint16)_spriteDimension.X, (Sint16)_spriteDimension.Y};
	return bound;
}
Rectangle Object::GetBoundR(float velocityX, float velocityY)
{
	Rectangle bound(_position.X+velocityX, _position.Y+velocityY, _spriteDimension.X, _spriteDimension.Y);
	return bound;
}
void Object::SetVelocity(Point2D velocity){_velocity=velocity; }
Point2D Object::GetVelocity(){ return _velocity; }
Point2D Object::GetPosition(){ return _position; }

#ifdef OBJ_PIXEL_COLLISION

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
Rectangle Object::ConvertToImagePositionR(Rectangle rect)
{
	Rectangle normalized(rect.X-_position.X,rect.Y-_position.Y,rect.W,rect.H);
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
Rectangle Object::GetBoundingBoxR(Rectangle boundsA, Rectangle boundsB)
{
	float leftCollision=Maximum(boundsA.X,boundsB.X);
	float rightCollision=Maximum(boundsA.X+boundsA.W,boundsB.X+boundsB.W);
	float topCollision=Minimum(boundsA.Y,boundsB.Y);
	float botCollision=Maximum(boundsA.Y+boundsA.H,boundsB.Y+boundsB.H);
	float width=rightCollision-leftCollision;
	float height=botCollision-topCollision;
	if (width>=0.0f&& height>=0.0f)
	{return Rectangle(leftCollision,rightCollision,width,height);
	}else
	{return Rectangle();}
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
bool Object::CheckCollisionR(Object objB)
{
	Rectangle collisionRect=GetBoundingBoxR(GetBoundR(), objB.GetBoundR());
	if(collisionRect.W == 0 && collisionRect.H == 0)
	{return false;}
	//Get the boundingbox on the two surfaces
	Rectangle normalA = ConvertToImagePositionR(collisionRect);
    Rectangle normalB = objB.ConvertToImagePositionR(collisionRect);
	//Check if two no-transpirant pixel collide
    for(int y = 0; y < collisionRect.H; y++)
	{
        for(int x = 0; x < collisionRect.W; x++)
		{
			//Return true if two non-transpirant pixels collide
			if(GetAlphaXY(this, (int)(normalA.X + x), int(normalA.X + y)) && GetAlphaXY(&objB, (int)(normalB.X + x), (int)(normalB.X + y)))
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
#endif
//Changes the sprite to the new sprite with the rgb data for the mask color
bool Object::ChangeSprite(std::string filename, int r, int g, int b )
{
	if(_surface != 0) { return _surface->LoadImage(filename.c_str(), r, g, b); }
	else return false;
}
//Changes the object position
void Object::SetPosition(float X, float Y){_position.X = X;_position.Y = Y;}
void Object::SetPosition(Point2D position){_position = position; }
//Changes the object velocity
void Object::SetVelocity(float X, float Y){_velocity.X = X;_velocity.Y = Y;}
//Set the colorkey (mask) of the surface
bool Object::MaskColor(int r, int g, int b)
{
	if(_surface!=0){return _surface->MaskColor(r, g, b);}
	else{return false;}
}
//Releases resources of the class
void Object::Free(){_surface->Free();}