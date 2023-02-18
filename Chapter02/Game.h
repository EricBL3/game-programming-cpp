#pragma once

#include "SDL.h"
#include "Actor.h"

class Game
{
public:
	Game();

	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(Actor* actor);
	void RemoveActor(Actor* actor);

private:

	// Helper functions
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// Member variables
	SDL_Renderer* mRenderer;
	SDL_Window* mWindow;
	bool mIsRunning;
	Uint32 mTicksCount;

	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	bool mUpdatingActors;

};