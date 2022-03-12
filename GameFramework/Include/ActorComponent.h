#pragma once

#include "Transform.h"

class ActorComponent
{
public:

	class Actor* GetOwner() const { return Owner; }

	Transform GetWorldTransform() const;

	virtual void Update(float DeltaTime) {}

	virtual ~ActorComponent() = default;

private:

	void SetOwner(class Actor* InOwner)
	{
		Owner = InOwner;
	}

	class Actor* Owner;
	friend class Actor;
};

