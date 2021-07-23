#pragma once

#include "Model.hpp"

// std
#include <memory>

namespace LeMU
{
	struct Transform2DComponent
	{
		glm::vec2 translation{};
		glm::vec2 scale{1.f, 1.f};
		float rotation;				// radians

		glm::mat2 mat2();
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
			Transform2DComponent transform2D{};

		private:
			GameObject(id_t objectID) :id(objectID){}	// private constructor, make sure id is unique

			id_t id;
	};
}