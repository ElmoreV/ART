#ifndef PLAYER_H
#define PLAYER_H
#include "SDLFramework.h"
#include "Object.h"
#include "Tail.h"
//Enemuration in which horizontal direction the player goes
enum HorizontalDirection
{
	HDirLeft,
	HDirRight,
	HDirNone,
};
//Enemuration in which vertical direction the player goes
enum VerticalDirection
{
	VDirUp,
	VDirDown,
	VDirNone
};
//Declarations of save and loadclasses that are used
class Saver;
class Loader;
//The player class
class Player : public Object
{
	friend Saver;
	friend Loader;
protected:
	//The tail of the mouse
	Tail tail;
	//The position of the mouse in the previous frame
	Point2D _previousPosition;
	//Enemuration in which vertical direction the player goes
	VerticalDirection _vDir;
	//Enemuration in which horizontal direction the player goes
	HorizontalDirection _hDir;
	//Enemuration in which horizontal direction the camera goes
	HorizontalDirection _cameraHDir;
	//Required variables for the animation
	//_interval is a constant after how many frames the animation must change to another
	//_countInteval will count the amount of frames that passed (to check with interval if a new frame is necessary)
	//_animationState tracks what the current animation is (walk left, walk right, etc)
	//_frame is the part of the image that must be showed
	int _interval, _countInterval, _animationState, _frame;
	//_maxVelocity= Maximum amount of pixel the mouse can fall per second
	//_maxHealth=Maximum amount of health the mouse can have
	//_maxInkPool=Maximum amount of ink the mouse can have
	int _maxVelocity, _maxHealth, _maxInkPool;
	//Whether or not the player can jump
	bool _jumpEnable;
	//Moves the mouse
	//Check if the mouse does not walk against a wall, or any other block, or drawing
	//Decreases health if it hits a spike on the wrong side
	void HandleCollision(Map* map, int screenWidth, int screenHeight, float timeDiff,Sounds& sound);
	//Return the boundingbox of the player in the previous frame
	Rectangle GetPreviousBoundR(float velocityX, float velocityY);
	//the four movement keys of the player
	bool _buttonUp, _buttonDown, _buttonLeft, _buttonRight;
	//Amount of ink, the player has received in the game
	float _totalInkReceived;
public:
	//To set the default values
	Player(Surface* surface, float X, float Y, int interval, int spriteX=1, int spriteY=1);
	void SetVelocity(float X, float Y);
	//Draws the frame of the player on the screen
	void Draw(WindowSurface screen, Point2D mapPosition);
	//Update the position based on the velocity and draws the clip on screen
	void Update(Map* map1, int screenWidth, int screenHeight,Sounds& sounds, long lastTick=-1);
	void HandleEvent(SDL_Event sEvent); //Keep up if arrowbuttons are pressed
	Point2D GetCenter();//Returns the centerposition of the player
	HorizontalDirection GetHorizontalDir(); //Returns de horizontal direction of the player
	VerticalDirection GetVerticalDir();//Returns de vertical direction of the player
	SDL_Rect GetFrame();//Get the part of the image that is showed
	Rectangle GetFrameR();//The function with floats
	void SetFrame(int frame);//Set the animation (if it has more then 1)
	//Health = The amount of health the player has left
	//InkPool = The amount of ink the player has left
	//InvulnerableTime = the amount of second that the player can lose any health
	float Health, InkPool, InvulnerableTime;
	//HealthRatio() = Returns the ratio of the health : Health / _maxHealth
	//InkPoolRatio() = Returns the ratio of the Ink : InkPool / _maxInkPool
	float HealthRatio(), InkPoolRatio();
	//Adds new ink to the players inkpool
	void AddInk(int value);
	//Jumps the mouse
	void Jump();
	//Return the position of the mouse in the previous frame
	Point2D GetPreviousPosition();
	//Return the position and dimensions of the mouse in the previous frame
	Rectangle GetPreviousBoundR();
	//Draws the healthbar on the screen, with given dimensions
	//If font*!=0, it also renders the text of the amount of health and the total health
	void DrawHealthBar(WindowSurface screen, int borderWidth, unsigned int X, unsigned int Y, unsigned int Width = 100, unsigned int Height = 20, Font* font = 0);
	//Draws the inkbar on the screen, with given dimensions
	//If font*!=0, it also renders the text of the amount of ink and the total ink
	void DrawInkBar(WindowSurface screen, int borderWidth, unsigned int X, unsigned int Y, unsigned int Width = 100, unsigned int Height = 20, Font* font = 0);
	//Resets the player, sets the position and the stats like health, if the bool resetStats=true
	void Reset(Point2D position = 0, bool resetStats=false);
	//Calculate and set the cameraposition based on the players position
	void SetMapPosition(Map* map, Point2D screenSize, float timeDiff, float part=0.5,float threshold=0.3, float speed=1.2);
};
#endif