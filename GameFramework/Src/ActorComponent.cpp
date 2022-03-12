#include "ActorComponent.h"

#include "Actor.h"

Transform ActorComponent::GetWorldTransform() const
{
    return GetOwner()->Transform;
}
