#pragma once

#include "SDL.h"
#include <unordered_map>

class Game
{
public:
	Game();

	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& filename);

private:

	// Helper functions
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();

	// Member variables
	SDL_Renderer* mRenderer;
	SDL_Window* mWindow;
	bool mIsRunning;
	Uint32 mTicksCount;

	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	bool mUpdatingActors;

	std::unordered_map<std::string, SDL_Texture*> mTextures;
	std::vector<class SpriteComponent*> mSprites;

};