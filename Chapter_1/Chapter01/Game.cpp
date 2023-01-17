#include "Game.h"

const int _wallThickness = 15;
const int _screenWidth = 1024;
const int _screenHeight = 768;
const float _paddleHeight = 100.0f;

Game::Game()
{
	mWindow = nullptr;
	mRenderer = nullptr;
	mIsRunning = true;

	mBallPos.x = _screenWidth / 2.0f;
	mBallPos.y = _screenHeight / 2.0f;

	mPaddlePos.x = 10.0f;
	mPaddlePos.y = _screenHeight / 2.0f;
}

bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", 100, 100, _screenWidth, _screenHeight,	0);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow,	-1,	SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	return true;
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}

		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_ESCAPE])
		{
			mIsRunning = false;
		}
	}
}

void Game::UpdateGame()
{

}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0,	255, 255);
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	// Top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		_screenWidth,		// Width
		_wallThickness	// Height
	};

	SDL_RenderFillRect(mRenderer, &wall);

	// Bottom wall
	wall.y = _screenHeight - _wallThickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// Right wall 
	wall.x = _screenWidth - _wallThickness;
	wall.y = 0;
	wall.w = _wallThickness;
	wall.h = _screenWidth;
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw ball
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - _wallThickness / 2),
		static_cast<int>(mBallPos.y - _wallThickness / 2),
		_wallThickness,
		_wallThickness
	};

	SDL_RenderFillRect(mRenderer, &ball);

	// Draw paddle
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - _paddleHeight / 2),
		_wallThickness,
		static_cast<int>(_paddleHeight)
	};

	SDL_RenderFillRect(mRenderer, &paddle);


	SDL_RenderPresent(mRenderer);
}