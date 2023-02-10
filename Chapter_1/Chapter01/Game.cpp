#include "Game.h"

const int _wallThickness = 15;
const int _screenWidth = 1024;
const int _screenHeight = 768;
const float _paddleHeight = 100.0f;
const float _paddleSpeed = 300.0f;
const float _ballXSpeed = -200.0f;
const float _ballYSpeed = 235.0f;

Game::Game()
{
	mWindow = nullptr;
	mRenderer = nullptr;
	mIsRunning = true;

	mPaddleDir = 0;

	mTicksCount = 0;
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

	mBallPos.x = _screenWidth / 2.0f;
	mBallPos.y = _screenHeight / 2.0f;
	mBallVel.x = _ballXSpeed;
	mBallVel.y = _ballYSpeed;
	mPaddlePos.x = 10.0f;
	mPaddlePos.y = _screenHeight / 2.0f;

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

		mPaddleDir = 0;
		if (state[SDL_SCANCODE_W])
		{
			mPaddleDir--;
		}
		if (state[SDL_SCANCODE_S])
		{
			mPaddleDir++;
		}
	}
}

void Game::UpdateGame()
{
	// Frame limiting for 60fps
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	mTicksCount = SDL_GetTicks();

	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * _paddleSpeed * deltaTime;
		// Check screen boundaries with top wall
		if (mPaddlePos.y < (_paddleHeight / 2.0f + _wallThickness))
		{
			mPaddlePos.y = _paddleHeight / 2.0f + _wallThickness;
		}
		// Check screen boundaries with bottom wall
		else if (mPaddlePos.y > (_screenHeight - _paddleHeight / 2.0f - _wallThickness))
		{
			mPaddlePos.y = _screenHeight - (_paddleHeight / 2.0f) - _wallThickness;
		}
	}

	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	
	// Check ball collision with paddle
	float diff = mPaddlePos.y - mBallPos.y;
	// Take absolute value of the difference
	diff = (diff > 0.0f) ? diff : -diff;
	if (
		// y-difference is small enough
		diff <= _paddleHeight / 2.0f &&
		// x-position is right
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// the ball is moving to the left
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1.0f;
	}
	// Check if offscreen to end game
	else if (mBallPos.x <= 0.0f)
	{
		mIsRunning = false;
	}
	// Check collision with right wall
	else if(mBallPos.x >= (_screenWidth - _wallThickness) && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f;
	}

	// Check ball collisions with top wall
	if (mBallPos.y <= _wallThickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	// Check ball collisions with bottom wall
	if (mBallPos.y >= _screenHeight - _wallThickness && mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
	}
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