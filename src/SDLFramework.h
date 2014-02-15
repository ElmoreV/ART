//SDLFramework Rev 6


#ifndef SDLFRAMEWORK_H
#define SDLFRAMEWORK_H
#include <ctime>
#define IMAGE
#define FONT
#define AUDIO
#define DRAWING
#define ERROR

//General declarations
#include <string>

#define NO_STDIO_REDIRECT 1
//Main SDL files
#include <SDL\\SDL.h>
#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")
#ifdef IMAGE
//Image extension
#include <SDL\\SDL_image.h>
#pragma comment(lib,"SDL2_image.lib")
#endif
#ifdef FONT
//Text extension
#include <SDL\\SDL_ttf.h>
#pragma comment(lib,"SDL2_ttf.lib")
#endif
#ifdef DRAWING
//'Drawing' extension
//#include <SDL\\SDL2_gfxPrimitives.h>
//#pragma comment(lib,"sdlgfx.lib")
#endif
#ifdef AUDIO
//Sound extension
#include <SDL\\SDL_mixer.h>
#pragma comment(lib,"SDL2_mixer.lib")
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
	Error(ErrorState error,std::string errorMessage, int errorCode=0, bool showSDLError=false, SDL_Window* win=0);
	//Errorstate error: Log/Caption/Exit. Determines how to handle the error
//errorMessage: the message describing the error,and it will be logged
//errorCode: optional code describing developer code values. Default=0; With code 0, the code will not be logged.
//showSDLError: when true, it add the internal SDLError message to the log message. Default = true.
	void HandleError(ErrorState error,std::string errorMessage, int errorCode=0, bool showSDLError=false,SDL_Window* win=0);
private:
	//Log the error to a file ('log.txt')
	//When code is positive non-zero: add the clocktime to the log
	//When code = 0, don't add it
	void LogError(std::string message, int code);
	//Show the error in the caption
	//With the message and the code
	void CaptionError(std::string message, int code,SDL_Window* win);

};



//Initialize all the Dll-components of SDL
bool InitSDL();
//Quit and free the Dlls that were used
void ClearSDL();

#ifdef FONT
//Font class, takes care of the Font which will be used for rendering text, contains the fonttype and the colour
class Font
{
public:
	Font();
	Font(TTF_Font* font);
	//Open a (new) font (and release the old one if it's needed)
	bool OpenFont(std::string filename, int size=28);
	//Set and get the text Color (with RGB colors or the in SDL already defined SDL_Color)
	void SetColor(int R,int G, int B);
	void SetColor(SDL_Color color);
	SDL_Color GetColor();
	//Free's the font's resources
	void Free();
	//Returns the font object as TTF_Font object native to SDL
	operator TTF_Font* ();
	//Return whether the font is initialized
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
	operator SDL_Surface* ();//Return for maximum compatibility
	SDL_Surface* operator->();//Return a reference(exact copy)of the surface to make calling e.g. MapRGB() easier
	bool IsInit();//Whether the surface is initialized
protected:
	BaseSurface();
	BaseSurface(SDL_Surface* surface);
	SDL_Surface* _surface;
	SDL_Texture* _texture;
};

//This Surface is special: it's the window, with window-like functions
class Window
{
private:
	int _width, _height;
	SDL_Window* _window;
	SDL_Renderer* _renderContext;
public:
	//Creating the window.
	//Without any parameters, it is just an empty one
	Window();
	Window(SDL_Window* window, SDL_Renderer* renderer);
	Window(int width, int height, bool windowFrame=true);
	//Creates a screen to work on
	//Width and height are the screen dimensions to set
	//bpp is bits per pixel. 32 gives the normal RGBA color scheme
	//doublebuffering enables the use of doublebuffering
	//windowFrame: when true: it shows the outlines of the window
	// when false: it doesn't show the outlines of the window(and no close,minimize or maximize button)
	bool CreateWindow(int width,int height, bool windowFrame=true);
	//Set the window caption
	void SetCaption(std::string caption);
	//Set the window icon
	void SetIcon(BaseSurface icon);
	//Update the window surface, so that all blits are shown
	void UpdateWindow();
	//Fill the entire window with a certain color (default=black RGB(0,0,0))
	bool ClearWindow(int r=0x00, int g=0x00, int b=0x00);
	operator SDL_Renderer*(){return _renderContext;};
	operator SDL_Window*(){return _window;}
	bool Resize(int width, int height);
	int GetWidth(){return _width;}
	int GetHeight(){return _height;}
#ifdef DRAWING
	//Draw a filled rectangle
	bool DrawFilledRect(int x1, int y1, int x2, int y2, int r, int g, int b);
	//Draw a line from 1 to 2
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
	//Loads an image unto a surface.
	//Can be a BMP
	//With the IMAGE extension, it can be almost any other image file.
	//colorKeyR/G/B is the masking color used to make the sprite partially transparent.
	//useImageAlpha makes SDL render the image with the alpha channel already present in the image
	//alpha makes the surface use a certain alpha value
	bool LoadImage(std::string filename, int colorKeyR=-1,int colorKeyG=-1, int colorKeyB=-1,bool useImageAlpha=false, int alpha=SDL_ALPHA_OPAQUE);
	#ifdef FONT
	//Renders a text with a certain font on the surface to draw
	bool RenderText(Font font,std::string text);
	#endif
	//Set a mask color
	//Check if it's between 0 and 255
	//If one of the colors is set to -1, unset the color mask(and transparancy on that part)
	bool MaskColor(int r=-1,int g=-1,int b=-1);
	//gets the current mask color and set it in r,g, b
	void Surface::GetMaskColor(int& r,int& g, int& b);
	//Draw: x,y are the screen's position
	//clip is the part of the loaded surface image to draw
	//Window is the surface to draw on
	bool Draw(Window window,unsigned int x=0, unsigned int y=0, SDL_Rect* clip=0);
	//Set the alpha (blending) transparancy
	bool SetTransparency(int alpha);
	//Get the surface width or height
	int GetWidth();
	int GetHeight();
	//Free the surface resources
	void Free();
private:
	int _r,_g,_b;

};

//An easy way to fill the SDL_Rect structure
void FillRect (SDL_Rect* destination,int x, int y, int w, int h);

std::string IntToString(int i);
//Formulas to get the bigger/smaller value
#define Maximum(a, b) ((a > b) ? a : b)
#define Minimum(a, b) ((a > b) ? b : a)

#endif
