//SDLFramework Rev 4


#ifndef SDLFRAMEWORK_H
#define SDLFRAMEWORK_H
#include <ctime>
#define IMAGE
//#define FONT
//#define AUDIO
#define DRAWING
#define ERROR
//General declarations
#include <string>

#define NO_STDIO_REDIRECT 1
//Main SDL files
#include <SDL\\SDL.h>
#pragma comment(lib,"SDL.lib")
#pragma comment(lib,"SDLmain.lib")
#ifdef IMAGE
//Image extension
#include <SDL\\SDL_image.h>
#pragma comment(lib,"SDL_image.lib")
#endif
#ifdef FONT
//Text extension
#include <SDL\\SDL_ttf.h>
#pragma comment(lib,"SDL_ttf.lib")
#endif
#ifdef DRAWING
//'Drawing' extension
#include <SDL\\SDL_gfxPrimitives.h>
#pragma comment(lib,"sdlgfx.lib")
#endif
#ifdef AUDIO
//Sound extension
#include <SDL\\SDL_mixer.h>
#pragma comment(lib,"SDL_mixer.lib")
#endif
#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )
#ifdef AUDIO
#include "SDLSound.h"
#endif

enum ErrorState{None,Log,Caption,CaptionOnly,Exit};
class Error
{
public:
	Error();
	Error(ErrorState error,std::string errorMessage, int errorCode=0, bool showSDLError=false);
	void HandleError(ErrorState error,std::string errorMessage, int errorCode=0, bool showSDLError=false);
private:
	void LogError(std::string message, int code);
	void MessageError(std::string message, int code);
	//ErrorState _errState;
};




bool InitSDL();
void ClearSDL();

#ifdef FONT
//Font class, takes care of the Font which will be used for rendering text, contains the fonttype and the colour
class Font
{
public:
	Font();
	Font(TTF_Font* font);
	bool OpenFont(std::string filename, int size=28);
	void SetColor(int R,int G, int B);
	void SetColor(SDL_Color color);
	SDL_Color GetColor();
	void Free();
	operator TTF_Font* ();
	bool IsInit();
protected:
	TTF_Font* _ttf;
	SDL_Color _textColor;
};
#endif


/*BaseSurface is the base class for all surfaces, basically for inheritance, as it now just behaves like a normal SDL_Surface*
*/
class BaseSurface
{
public:	
	operator SDL_Surface* ();
	SDL_Surface* operator->();
	bool IsInit();
protected:
	BaseSurface();
	BaseSurface(SDL_Surface* surface);
	SDL_Surface* _surface;
};

//This Surface is special: it's the window, with window-like functions
class WindowSurface:public BaseSurface
{
private:
	int _width, _height;
public:
	WindowSurface();
	WindowSurface(SDL_Surface* surface);
	WindowSurface(int width, int height, int bpp=0, bool doublebuffering=false, bool windowFrame=true);
	bool CreateWindowSurface(int width,int height, int bpp=0, bool doublebuffering=false, bool windowFrame=true);
	void SetCaption(std::string caption);
	bool UpdateWindow();
	bool ClearWindow(int r=0x00, int g=0x00, int b=0x00);
	int GetWidth(){return _width;}
	int GetHeight(){return _height;}
#ifdef DRAWING
	bool DrawFilledRect(int x1, int y1, int x2, int y2, int r, int g, int b);
	bool DrawLine(int x1, int y1, int x2, int y2, int r, int g, int b);
#endif
};

/* A normal surface, useful for blitting text and images onto the screen
Freeing the resource must be done manually at the end with Surface.Free() , 
but it's all taken care of in LoadImage and RenderText*/
class Surface: public BaseSurface
{
public:
	Surface();
	Surface(SDL_Surface* surface);
	bool LoadImage(std::string filename, int colorKeyR=-1,int colorKeyG=-1, int colorKeyB=-1,bool useImageAlpha=false, int alpha=SDL_ALPHA_OPAQUE);
	#ifdef FONT
	bool RenderText(Font font,std::string text);
	#endif
	bool MaskColor(int r=-1,int g=-1,int b=-1);
	void Surface::GetMaskColor(int& r,int& g, int& b);
	bool Draw(WindowSurface windowSurface,unsigned int x=0, unsigned int y=0, SDL_Rect* clip=0);
	bool SetTransparency(int alpha);
	void Free();
private:
	int _r,_g,_b;

};
/*class CanvasSurface:public BaseSurface{};*/

void FillRect (SDL_Rect* destination,int x, int y, int w, int h);
//Formulas to get the bigger/smaller value
#define Maximum(a, b) ((a > b) ? a : b)
#define Minimum(a, b) ((a > b) ? b : a)

#endif
