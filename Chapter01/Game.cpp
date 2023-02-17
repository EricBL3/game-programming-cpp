#include "Game.h"

const int _wallThickness = 15;
const int _screenWidth = 1024;
const int _screenHeight = 768;
const int _numberOfBalls = 4;
const float _paddleHeight = 100.0f;
const float _paddleSpeed = 300.0f;
const float _ballXSpeed = -200.0f;
const float _ballYSpeed = 235.0f;

Game::Game()
{
	mWindow = nullptr;
	mRenderer = nullptr;
	mIsRunning = true;

	mPlayerOnePaddleDir = 0;
	mPlayerTwoPaddleDir = 0;

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

	srand(time(NULL));

	mBalls.clear();
	for (int i = 0; i < _numberOfBalls; i++)
	{
		Ball* newBall = new Ball();
		newBall->ballPos.x = rand() % (_screenWidth - 50) + 50.0f;
		newBall->ballPos.y = rand() % (_screenHeight - _wallThickness) + _wallThickness;
		newBall->ballVel.x = _ballXSpeed;
		newBall->ballVel.y = _ballYSpeed;
		mBalls.push_back(*newBall);
	}

	/*mBallPos.x = _screenWidth / 2.0f;
	mBallPos.y = _screenHeight / 2.0f;
	mBallVel.x = _ballXSpeed;
	mBallVel.y = _ballYSpeed;*/

	mPlayerOnePaddlePos.x = 10.0f;
	mPlayerOnePaddlePos.y = _screenHeight / 2.0f;

	mPlayerTwoPaddlePos.x = _screenWidth - _wallThickness -  10.0f;
	mPlayerTwoPaddlePos.y = _screenHeight / 2.0f;

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

		mPlayerOnePaddleDir = 0;
		if (state[SDL_SCANCODE_W])
		{
			mPlayerOnePaddleDir--;
		}
		if (state[SDL_SCANCODE_S])
		{
			mPlayerOnePaddleDir++;
		}

		mPlayerTwoPaddleDir = 0;
		if (state[SDL_SCANCODE_I])
		{
			mPlayerTwoPaddleDir--;
		}
		if (state[SDL_SCANCODE_K])
		{
			mPlayerTwoPaddleDir++;
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

	if (mPlayerOnePaddleDir != 0)
	{
		mPlayerOnePaddlePos.y += mPlayerOnePaddleDir * _paddleSpeed * deltaTime;
		// Check screen boundaries with top wall
		if (mPlayerOnePaddlePos.y < (_paddleHeight / 2.0f + _wallThickness))
		{
			mPlayerOnePaddlePos.y = _paddleHeight / 2.0f + _wallThickness;
		}
		// Check screen boundaries with bottom wall
		else if (mPlayerOnePaddlePos.y > (_screenHeight - _paddleHeight / 2.0f - _wallThickness))
		{
			mPlayerOnePaddlePos.y = _screenHeight - (_paddleHeight / 2.0f) - _wallThickness;
		}
	}

	if (mPlayerTwoPaddleDir != 0)
	{
		mPlayerTwoPaddlePos.y += mPlayerTwoPaddleDir * _paddleSpeed * deltaTime;
		// Check screen boundaries with top wall
		if (mPlayerTwoPaddlePos.y < (_paddleHeight / 2.0f + _wallThickness))
		{
			mPlayerTwoPaddlePos.y = _paddleHeight / 2.0f + _wallThickness;
		}
		// Check screen boundaries with bottom wall
		else if (mPlayerTwoPaddlePos.y > (_screenHeight - _paddleHeight / 2.0f - _wallThickness))
		{
			mPlayerTwoPaddlePos.y = _screenHeight - (_paddleHeight / 2.0f) - _wallThickness;
		}
	}

	int size = mBalls.size();
	for (int i = 0; i < size; i++)
	{
		mBalls.at(i).ballPos.x += mBalls.at(i).ballVel.x * deltaTime;
		mBalls.at(i).ballPos.y += mBalls.at(i).ballVel.y * deltaTime;

		float diffPlayerOne = mPlayerOnePaddlePos.y - mBalls.at(i).ballPos.y;
		// Take absolute value of the difference
		diffPlayerOne = (diffPlayerOne > 0.0f) ? diffPlayerOne : -diffPlayerOne;

		float diffPlayerTwo = mPlayerTwoPaddlePos.y - mBalls.at(i).ballPos.y;
		diffPlayerTwo = (diffPlayerTwo > 0.0f) ? diffPlayerTwo : -diffPlayerTwo;

		// Check ball collision with player 1 paddle
		if (
			// y-difference is small enough
			diffPlayerOne <= _paddleHeight / 2.0f &&
			// x-position is right
			mBalls.at(i).ballPos.x <= 25.0f && mBalls.at(i).ballPos.x >= 20.0f &&
			// the ball is moving to the left
			mBalls.at(i).ballVel.x < 0.0f)
		{
			mBalls.at(i).ballVel.x *= -1.0f;
		}
		// Check if offscreen to end game
		else if (mBalls.at(i).ballPos.x <= 0.0f || mBalls.at(i).ballPos.x >= _screenWidth)
		{
			mIsRunning = false;
		}
		// Check collision with player 2 paddle
		else if(
			diffPlayerTwo <= _paddleHeight / 2.0f &&
			mBalls.at(i).ballPos.x >= (_screenWidth - 25.0f) && mBalls.at(i).ballPos.x <= (_screenWidth - 20.0f) &&
			mBalls.at(i).ballVel.x > 0.0f)
		{
			mBalls.at(i).ballVel.x *= -1.0f;
		}

		// Check ball collisions with top wall
		if (mBalls.at(i).ballPos.y <= _wallThickness && mBalls.at(i).ballVel.y < 0.0f)
		{
			mBalls.at(i).ballVel.y *= -1;
		}
		// Check ball collisions with bottom wall
		if (mBalls.at(i).ballPos.y >= _screenHeight - _wallThickness && mBalls.at(i).ballVel.y > 0.0f)
		{
			mBalls.at(i).ballVel.y *= -1;
		}
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
	/*
	wall.x = _screenWidth - _wallThickness;
	wall.y = 0;
	wall.w = _wallThickness;
	wall.h = _screenWidth;
	SDL_RenderFillRect(mRenderer, &wall);
	*/

	// Draw ball
	int size = mBalls.size();
	for (int i = 0; i < size; i++)
	{
		SDL_Rect ball{
			static_cast<int>(mBalls.at(i).ballPos.x - _wallThickness / 2),
			static_cast<int>(mBalls.at(i).ballPos.y - _wallThickness / 2),
			_wallThickness,
			_wallThickness
		};

		SDL_RenderFillRect(mRenderer, &ball);
	}

	// Draw player one paddle
	SDL_Rect PlayerOnePaddle{
		static_cast<int>(mPlayerOnePaddlePos.x),
		static_cast<int>(mPlayerOnePaddlePos.y - _paddleHeight / 2),
		_wallThickness,
		static_cast<int>(_paddleHeight)
	};

	SDL_RenderFillRect(mRenderer, &PlayerOnePaddle);

	// Draw player two paddle
	SDL_Rect PlayerTwoPaddle{
		static_cast<int>(mPlayerTwoPaddlePos.x),
		static_cast<int>(mPlayerTwoPaddlePos.y - _paddleHeight / 2),
		_wallThickness,
		static_cast<int>(_paddleHeight)
	};

	SDL_RenderFillRect(mRenderer, &PlayerTwoPaddle);


	SDL_RenderPresent(mRenderer);
}