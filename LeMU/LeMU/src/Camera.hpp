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

		inline  const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }



	private:
		glm::mat4 projectionMatrix{ 1.0f };


	};



}