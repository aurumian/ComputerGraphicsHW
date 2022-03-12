#include "Actor.h"

#include "Game.h"
#include "ActorComponent.h"

Actor::Actor()
{
	Game::GetInstance()->Actors.push_back(this);
}

Actor::~Actor()
{
	// todo: remove this actor from game's array
	//Game::GetInstance()->Actors.
	for (ActorComponent* comp : Components)
	{
		delete comp;
	}
}

void Actor::Update(float DeltaTime)
{
	for (ActorComponent* comp : Components)
	{
		if (comp != nullptr)
		{
			comp->Update(DeltaTime);
		}
	}
}
