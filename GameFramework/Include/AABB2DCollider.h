#pragma once

#include "ActorComponent.h"

class AABB2DCollider : public ActorComponent
{
public:
	bool DoesOverlap(const AABB2DCollider& Other) const;

private:
	bool DoesOverlap1D(float a, float b, float c, float d) const;
	bool DoesOverlapRange1D(float X, float XHalfSize, float Y, float YHalfSize) const;
};

