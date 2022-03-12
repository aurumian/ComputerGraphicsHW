#pragma once

#include "Transform.h"
#include "Game.h"
#include <vector>
#include <type_traits>


class Actor
{
public:
	Actor();
	virtual ~Actor();

	Transform Transform;

	virtual void Update(float DeltaTime);

	template<class T>
	T* AddActorComponent()
	{
		T* comp = new T();

		Components.push_back(comp);

		static_cast<class ActorComponent*>(comp)->SetOwner(this);

		if constexpr (std::is_base_of<class MeshRenderer, T>::value)
		{
			Game::GetInstance()->Renderers.push_back(comp);
		}
		else if constexpr (std::is_base_of<class AABB2DCollider, T>::value)
		{
			Game::GetInstance()->Colliders.push_back(comp);
		}

		return comp;
	}

	template <class T>
	T* GetComponent()
	{
		for (ActorComponent* comp : Components)
		{
			T* tComp = dynamic_cast<T*>(comp);
			if (tComp != nullptr)
			{
				return tComp;
			}
		}

		return nullptr;
	}

private:
	std::vector<class ActorComponent*> Components;
};

