#include "pch.h"
#include "Component.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

shared_ptr<Transform> Component::GetTransform()
{ 
	shared_ptr<Component> transform = GetGameObject()->GetComponent(EComponent::Transform);
	return static_pointer_cast<Transform>(transform); 
}
