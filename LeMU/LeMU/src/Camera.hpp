#pragma once


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>



namespace LeMU
{

	class Camera 
	{
	public:
		void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);

		void setPerspectiveProjection(float fovy, float aspect, float near, float far);

		inline const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
		inline const glm::mat4& getViewMatrix() const { return viewMatrix; }

		void setViewDirection(  
			glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.0f, -1.0f, 0.0f});

		void setViewTarget(
			glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{ 0.0f, -1.0f, 0.0f });

		// specify the orientation by using euler angle
		void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

	private:
		glm::mat4 projectionMatrix{ 1.0f };
		glm::mat4 viewMatrix{ 1.0f };

	};



}