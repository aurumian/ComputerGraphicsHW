#include "SphereCollider.h"

bool SphereCollider::DoesOverlap(const SphereCollider& Other) const
{
	return Vector3::DistanceSquared(GetWorldTransform().Position, Other.GetWorldTransform().Position) <= Radius * Radius + Other.Radius * Other.Radius;
}