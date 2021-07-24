#pragma once

#include "Model.hpp"

// std
#include <memory>

namespace LeMU
{
	struct TransformComponent
	{
		glm::vec3 translation{};
		glm::vec3 scale{1.0f, 1.0f, 1.0f};
		glm::vec3 rotation;				// radians

		// Translation * Ry * Rx * Rz * Scale transformation
		// Rotation convention uses tait-bryan angles with axis order Y, X, Z
		glm::mat4 mat4();
	};



	class GameObject
	{
		public:
			using id_t = unsigned int;		// each object has unique ID

			static GameObject createGameObject();

			id_t getID() const { return id; }
			
			// since all objects are unique, copy constructor and assignment constructor are deleted
			GameObject& operator=(const GameObject&) = delete;
			GameObject(const GameObject&) = delete;

			// move operator and assignment operator are default
			GameObject(GameObject&&) = default;
			GameObject& operator=(GameObject&&) = default;


			std::shared_ptr<Model> model{};
			glm::vec3 color{};
			TransformComponent transform{};

		private:
			GameObject(id_t objectID) :id(objectID){}	// private constructor, make sure id is unique

			id_t id;
	};
}