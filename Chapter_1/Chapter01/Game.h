#pragma once

#include "SDL.h"

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
	SDL_Window* mWindow;
	bool mIsRunning;
};