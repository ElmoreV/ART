//SDLFramework Rev 5


#include "SDLFramework.h"
#ifdef ERROR
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#endif

Error::Error(){};
Error::Error(ErrorState error,std::string errorMessage, int errorCode, bool showSDLError)
{HandleError(error,errorMessage,errorCode,showSDLError);};
//Errorstate error: Log/Caption/Exit. Determines how to handle the error
//errorMessage: the message describing the error,and it will be logged
//errorCode: optional code describing developer code values. Default=0; With code 0, the code will not be logged.
//showSDLError: when true, it add the internal SDLError message to the log message. Default = true.
void Error::HandleError(ErrorState error,std::string errorMessage, int errorCode, bool showSDLError)
{
#ifdef ERROR

	if (error==Exit)//If the error-severity is exit-worthy, log the error, and exit
	{
		LogError(errorMessage,errorCode);
		if (showSDLError)
		{LogError(SDL_GetError(),-1);}
		exit(errorCode);
	}else if (error==Caption)//The error should be shown in the caption to warn the user (it also logs it)
	{
		CaptionError(errorMessage,errorCode);
		LogError(errorMessage,errorCode);
		if (showSDLError)
		{LogError(SDL_GetError(),-1);}
	}else if (error==CaptionOnly)
	{
		CaptionError(errorMessage,errorCode);
	}else if (error==Log)//The error isn't that severe, and should be just logged
	{
		LogError(errorMessage,errorCode);
		if (showSDLError)
		{LogError(SDL_GetError(),-1);}
	}
#endif
};
//When code is positive non-zero: add the clocktime to the log
//When code = 0, don't add it
void Error::LogError(std::string message, int code)
{
#ifdef ERROR
	std::string str;
	if (code>0)
	{
		//Add the clocktime to the log
		std::stringstream clockstr; 
		clockstr<<(int)clock();
		str=clockstr.str();str+="   ";	
	}
	str+=message;

	if (code!=0)
	{
		str+="  ";
		std::stringstream integer;
		if (code<0)
		{integer<<"-";code=-code;} 
		integer<<code;
		str+=integer.str();
	}
	std::ofstream out("log.txt",std::ios::out|std::ios::app|std::ios::binary);//Log it and make a newline
	out<<str;
	if (str.length()>0)
	{out<<"\n\r";out<<std::endl;}
	out.flush();
	out.close();
#endif
};
//Show the error in the caption
//With the message and the code
void Error::CaptionError(std::string message, int code)
{
#ifdef ERROR
	std::string str=message;
	if (code!=0)//Add code if non-zero
	{
		str+="   ";
		std::stringstream integer;
		if (code<0){integer<<"-";code=-code;}  
		integer<<code;
		str+=integer.str();
	}
	SDL_WM_SetCaption(str.c_str(),0);
#endif
}; 
//Initialize all the Dll-components of SDL
bool InitSDL()
{
	int flags=SDL_INIT_VIDEO;//Initialize video component
	#ifdef AUDIO
	flags|=SDL_INIT_AUDIO;//Initialize audio component
	#endif
	if (SDL_Init(flags)==-1)//Initialize SDL
	{
		Error error(Exit,"The initializing of SDL failed with code:");
		return false;
	};
	#ifdef FONT
	if (TTF_Init()==-1)//Initialize Font extension
	{
		Error error(Exit,"The initializing of TTF failed with code:");
		return false;
	};
	#endif
	#ifdef AUDIO
	if (Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 )==-1)//Open audio device on a certain sample rate
	{
		Error error(Exit,"The opening of the audio device failed with code:");
		return false;
	}
	#endif
	return true;
}
//Quit the SDL,TTF and mixer extenstion
void ClearSDL()
{
	SDL_Quit();
	#ifdef FONT
	TTF_Quit();
	#endif
	#ifdef AUDIO
	Mix_CloseAudio();
	#endif
}

#ifdef FONT

//Font constructors
Font::Font():_ttf(0){_textColor.b=0;_textColor.g=0;_textColor.r=0;}

Font::Font(TTF_Font* font):_ttf(font){_textColor.b=0;_textColor.g=0;_textColor.r=0;}
//Open a (new) font (and release the old one if it's needed)
bool Font::OpenFont(std::string filename, int size)
{
	Free();
	_ttf=TTF_OpenFont( filename.c_str(), size );
	if (_ttf==0)
	{		
		std::string errorMessage="The following font file could not be opened: ";
		errorMessage+=filename;
		Error error(Caption,errorMessage);
		return false;
	}
	return true;
}
//Set and get the text Color (with RGB colors or the in SDL already defined SDL_Color)
void Font::SetColor(int R,int G, int B)
{
	SDL_Color text={R,G,B};
	_textColor=text;
}
void Font::SetColor(SDL_Color color)
{
	_textColor=color;
}
SDL_Color Font::GetColor()
{
	return _textColor;
}
//Free's the font's resources
void Font::Free()
{
	if (_ttf!=0)
	{
		TTF_CloseFont(_ttf);
		_ttf=0;
	}
}

Font::operator TTF_Font* (){return _ttf;};
bool Font::IsInit(){return (_ttf!=0);}
#endif

//BaseSurface

BaseSurface::operator SDL_Surface* (){return _surface;};//Return for maximum compatibility
SDL_Surface* BaseSurface::operator->(){return _surface;};//Return a reference(exact copy)of the surface to make calling e.g. MapRGB() easier
BaseSurface::BaseSurface():_surface(0){}; //Default constructor, initialize _surface to 0
BaseSurface::BaseSurface(SDL_Surface* surface):_surface(surface){}; //Surface constructor, which handles SDL_Surface* input;
bool BaseSurface::IsInit(){return (_surface!=0);}//Checks if the surface is already initialized

//Surface

Surface::Surface():BaseSurface(){MaskColor();};//Default ctor, calls the ctor of BaseSurface
Surface::Surface(SDL_Surface* surface):BaseSurface(surface){MaskColor();};//Surface ctor, calls the ctor of BaseSurface

//Draw: x,y are the screen's position
//clip is the part of the loaded surface image to draw
//windowSurface is the surface to draw on
bool Surface::Draw(WindowSurface windowSurface,unsigned int x, unsigned int y, SDL_Rect* clip)
{
	if (_surface!=0)
	{
		SDL_Rect offset;//Fill a rectangle with the x's and y's, width and heigth aren't important for the destination rectangle
		offset.x=x;
		offset.y=y;
		int retVal=SDL_BlitSurface(_surface,clip,windowSurface,&offset);
		if (retVal==-1)//Return value =1 when error
		{		
			Error error(Caption,"Failed to blit to the window with code: ");
			return false;
		}else if (retVal==-2)//Return value =2 when video memory is lost
		{
			Error error(Log,"The Blit-memory was lost in VRAM: ");
			//TODO:
			//Draw individual pixels in video memory, because that memory was lost
			return false;//Must become return true when successful
		}else
		{
			return true;
		}
	}
	return false;
};
//Loads an image unto a surface.
//Can be a BMP
//With the IMAGE extension, it can be almost any other image file.
//colorKeyR/G/B is the masking color used to make the sprite partially transparent.
//useImageAlpha makes SDL render the image with the alpha channel already present in the image
//alpha makes the surface use a certain alpha value
bool Surface::LoadImage(std::string filename,int colorKeyR,int colorKeyG, int colorKeyB,bool useImageAlpha,int alpha)
{
	Free();//Free the resource if it is already occupied
	Surface tempLoaded;
#ifndef IMAGE
	tempLoaded=SDL_LoadBMP(filename.c_str());//Load the BMP
#else
	tempLoaded=IMG_Load(filename.c_str());//Load the image file
#endif
	if (tempLoaded==0)//If error?
	{
		std::string errorMessage="The following image file cannot be opened: ";
		errorMessage+=filename;
		Error error(Caption,errorMessage);
		return false;
	}
	//Colorkey and transparency here for hardware acceleration
	if (colorKeyR!=-1&&colorKeyG!=-1&&colorKeyB!=-1)
	{tempLoaded.MaskColor(colorKeyR,colorKeyG,colorKeyB);}
	if (alpha!=SDL_ALPHA_OPAQUE)
	{tempLoaded.SetTransparency(alpha);}
	//convert to video buffer pixel format
	if (useImageAlpha)
	{_surface=::SDL_DisplayFormatAlpha(tempLoaded);}
	else{_surface=SDL_DisplayFormat(tempLoaded);}
	if (_surface==0)//If an error occurs
	{
		_surface=tempLoaded;//Just use the unoptimized image then
		std::string errorMessage="The following image file could not be converted: ";
		errorMessage+=filename;
		Error error(Log,errorMessage);
	}else
	{
		tempLoaded.Free();//Otherwise free the resource
	}

	return true;
};
#ifdef FONT
//Renders a text with a certain font on the surface to draw
bool Surface::RenderText(Font font,std::string text)
{
	Free();//Free surface if already taken
	_surface=TTF_RenderText_Solid( font, text.c_str(), font.GetColor() );//Render the Text
	if (_surface==0)
	{		
		Error error(Caption,"The following text could not be rendered: "+text);
		return false;
	}
	return true;
};
#endif
//Set a mask color
//Check if it's between 0 and 255
//If one of the colors is set to -1, unset the color mask(and transparancy on that part)
bool Surface::MaskColor(int r,int g,int b)
{
	if (_surface!=0)
	{
		int flags=SDL_SRCCOLORKEY;
		if (r==-1||g==-1||b==-1){flags=0;}//If one of the colours is -1,then unset the colorkey
		if (r>0xff){r=0xff;};if (g>0xff){g=0xff;};if (b>0xff){b=0xff;};//Keep the values within their limits
		if (r!=_r||g!=_g||b!=_b)//if the color is not already set to the same colour
		{
			_r=r;_g=g;_b=b;//Set the maskcolour in memory
			if (r<0x00){r=0x00;};if (g<0x00){g=0x00;};if (b<0x00){b=0x00;};//If it's below zero, change it to 0
			if (::SDL_SetColorKey(_surface,flags,SDL_MapRGB(_surface->format,r,g,b))==-1)
			{		
				Error error(Log,"Failed to set Colorkey with code :",0,true);
				return false;
			}
		}
		return true;
	}
	return false;
}
//gets the current mask color and set it in r,g, b
void Surface::GetMaskColor(int& r,int& g, int& b)
{
	r=_r;
	g=_g;
	b=_b;
};
//Set the alpha (blending) transparancy
bool Surface::SetTransparency(int alpha)
{
	if (_surface!=0)
	{
		if (alpha>SDL_ALPHA_OPAQUE){alpha=SDL_ALPHA_OPAQUE;}//Check if some limits are exceeded
		if (alpha<SDL_ALPHA_TRANSPARENT){alpha=SDL_ALPHA_TRANSPARENT;}
		if (SDL_SetAlpha(_surface,SDL_SRCALPHA,alpha)==-1)
		{		
			Error error(Log,"Failed to set transparency",0,true);
			return false;
		};
		return true;
	}
	return false;
};
//Get the surface width
int Surface::GetWidth(){return _surface!=0?_surface->w:0;}
//Get the surface height
int Surface::GetHeight(){return _surface!=0?_surface->h:0;}
void Surface::Free()//Freeing the surface
{
	if (_surface!=0)//When the surface is actually initialized, free the surface
	{
		SDL_FreeSurface(_surface);
		_surface=0;
	}
}
//Creating the windowsurface.
//Without any parameters, it is just an empty one
WindowSurface::WindowSurface():BaseSurface(){};
WindowSurface::WindowSurface(SDL_Surface* surface):BaseSurface(){_surface=surface;};
WindowSurface::WindowSurface(int width, int height, int bpp, bool doublebuffering, bool windowFrame):BaseSurface()
{
	CreateWindowSurface(width,height,bpp,doublebuffering,windowFrame);
};
//Creates a screen to work on
//Width and height are the screen dimensions to set
//bpp is bits per pixel. 32 gives the normal RGBA color scheme
//doublebuffering enables the use of doublebuffering
//windowFrame: when true: it shows the outlines of the window
// when false: it doesn't show the outlines of the window(and no close,minimize or maximize button)
bool WindowSurface::CreateWindowSurface(int width,int height, int bpp, bool doublebuffering, bool windowFrame)
{
	int modeFlags=0;
	//Render the buffer in system memory or in video memory with doublebuffering, which can increase aestetics
	if (doublebuffering==true){modeFlags|=SDL_HWSURFACE|SDL_DOUBLEBUF;}else{modeFlags|=SDL_SWSURFACE;}
	//Makes the frame resizable, or just non-existent, depending on the boolean
	if (windowFrame==false){modeFlags|=SDL_NOFRAME;}else{modeFlags|=SDL_RESIZABLE;}
	//Create the window surface
	_surface=::SDL_SetVideoMode(width,height,bpp,modeFlags);
	_width=width;_height=height;
	if (_surface==0)
	{		
		Error error(Exit,"Failed to create window surface",0,true);
		return false;
	};
	return true;
};
//Set the window caption
void WindowSurface::SetCaption(std::string caption)
{
	SDL_WM_SetCaption(caption.c_str(),0);//Could also set the icon.
}
//Update the window surface, so that all blits are shown
bool WindowSurface::UpdateWindow()
{
	if (_surface!=0)
	{
		if (SDL_Flip(_surface)==-1)
		{		
			Error error(Caption,"Failed to update screen with code: ",0,true);
			return false;
		}
		return true;
	}
	return false;
}
//Fill the entire window with a certain color (default=black RGB(0,0,0))
bool WindowSurface::ClearWindow(int r, int g, int b)
{
	if (_surface!=0)
	{
		if (::SDL_FillRect(_surface,0,SDL_MapRGB(_surface->format,r,g,b)))
		{	
			Error error(Caption,"Failed to clear the window",0,true);
			return false;
		};
		return true;
	}
	return false;
}
#ifdef DRAWING
//Draw a filled rectangle
bool WindowSurface::DrawFilledRect(int x1, int y1, int x2, int y2, int r, int g, int b)
{
	SDL_Rect rectangle;
	::FillRect(&rectangle,x1,y1,x2-x1,y2-y1);
	int retVal=SDL_FillRect(_surface,&rectangle,SDL_MapRGB(_surface->format,r,g,b));
	if (retVal==-1)
	{		
		Error error(Log,"Failed to draw filled rectangle",0,true);
		return false;
	}
	return true;
}
//Draw a line from 1 to 2
bool WindowSurface::DrawLine(int x1, int y1, int x2, int y2, int r, int g, int b)
{
	if (::lineRGBA(_surface,x1,y1,x2,y2,r,g,b,255)==-1)
	{
		Error error(Log,"Failed to draw line",0,true);
		return false;
	};
	return true;
};
#endif



//An easy way to fill the SDL_Rect structure
void FillRect (SDL_Rect* destination,int x, int y, int w, int h)
{
	destination->x=x;
	destination->y=y;
	destination->w=w;
	destination->h=h;
}