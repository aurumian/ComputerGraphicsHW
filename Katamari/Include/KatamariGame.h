#pragma once

#include "Game.h"


class KatamariGame : public Game
{
public:
	virtual void PrepareResources() override;

	virtual void Update(float DeltaTime) override;

	GameComponent* Player = nullptr;

};