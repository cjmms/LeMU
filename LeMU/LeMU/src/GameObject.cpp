#include "GameObject.hpp"

namespace LeMU
{


	GameObject LeMU::GameObject::createGameObject()
	{
		static id_t currentID = 0;
		return GameObject{ currentID++ };
	}

	glm::mat2 Transform2DComponent::mat2() 
	{ 
		const float sinRotation = glm::sin(rotation);
		const float cosRotation = glm::cos(rotation);
		// TODO:: replaced with Quaternion
		glm::mat2 rotateMat{ {cosRotation, sinRotation}, {-sinRotation, cosRotation} };

		glm::mat2 scaleMat{ {scale.x, 0}, {0, scale.y} };

		return rotateMat * scaleMat; 
	}

}