#pragma once

#include "SDL.h"
#include <vector>
#include <random>

struct Vector2
{
	float x;
	float y;
};

struct Ball
{
	Vector2 ballPos;
	Vector2 ballVel;
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

	std::vector<Ball> mBalls;
	Uint32 mTicksCount;
	
};