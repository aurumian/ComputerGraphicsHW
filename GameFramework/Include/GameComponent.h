#pragma once

#include "Transform.h"
#include "Game.h"
#include <vector>

class GameComponent
{
public:

	Transform mTransform;

	GameComponent* GetParent() const { return Parent; }

	Transform GetWorldTransform() const;

	virtual void Update(float DeltaTime) {}

	virtual ~GameComponent() = default;

	bool bShouldUpdate = false;

	void SetParent(GameComponent* InParent);

	// todo: pass contructor args
	template<class T>
	T* AddChildComponent()
	{
		return Game::GetInstance()->CreateGameComponent<T>(this);
	}

private:

	GameComponent* Parent = nullptr;

	std::vector<GameComponent*> Children;
};

