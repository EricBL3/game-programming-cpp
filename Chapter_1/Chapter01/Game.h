#pragma once

#include "SDL.h"

struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game();

	// Initialize the game
	bool Initialize();

	// Runs the game loop until  the game is over
	void RunLoop();

	// Shutdown the game
	void Shutdown();

private:

	// Helper functions
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// Member variables
	SDL_Renderer* mRenderer;
	SDL_Window* mWindow;
	bool mIsRunning;

	Vector2 mPlayerOnePaddlePos;
	int mPlayerOnePaddleDir;

	Vector2 mPlayerTwoPaddlePos;
	int mPlayerTwoPaddleDir;

	Vector2 mBallPos;
	Vector2 mBallVel;
	Uint32 mTicksCount;
	
};