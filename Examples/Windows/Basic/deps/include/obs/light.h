

#pragma once


#include <glm/glm.hpp>


struct Light
{
	glm::vec3 position;
	glm::vec3 color;

	Light(){}

	Light(glm::vec3 position, glm::vec3 color) :
		position(position), color(color)
	{}
};