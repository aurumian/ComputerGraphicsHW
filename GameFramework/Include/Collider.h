#pragma once

#include "MathInclude.h"
#include "ActorComponent.h"

#define COLLIDER_CHILD_BODY() \
public: \
virtual bool DoesOverlap(const Collider& Other) const override \
{ \
	return Other.DoesOverlap(*this); \
} \

class Collider : public ActorComponent
{
public:

	virtual bool DoesOverlap(const Collider& Other) const = 0;

	virtual bool DoesOverlap(const class AABB2DCollider& Other) const;
};