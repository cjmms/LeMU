#include "GameObject.hpp"

#include <gtc/matrix_transform.hpp>

namespace LeMU
{


	GameObject LeMU::GameObject::createGameObject()
	{
		static id_t currentID = 0;
		return GameObject{ currentID++ };
	}

	glm::mat4 TransformComponent::mat4() 
	{ 
		auto transform = glm::translate(glm::mat4{1.0f}, translation);

		// rotate rotation.y radians along with y axis
		transform = glm::rotate(transform, rotation.y, { 0.0f, 1.0f, 0.0f });
		transform = glm::rotate(transform, rotation.x, { 1.0f, 0.0f, 0.0f });
		transform = glm::rotate(transform, rotation.z, { 0.0f, 0.0f, 1.0f });

		transform = glm::scale(transform, scale);

		return transform;
	}

}