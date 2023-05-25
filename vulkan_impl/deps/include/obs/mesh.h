
#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


#include <jvk/vertex.h>




class Mesh 
{
public:

	glm::mat4 transformMatrix = glm::mat4(1.0f);

	unsigned int index = 0;

	const std::vector<Vertex> m_vertices =
	{
		// FRONT
		{{-0.5f, -0.5f, -0.5f}, { 1.0f,  1.0f,  1.0f}},
		{{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f,  0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  1.0f}},
		{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  1.0f}},
		{{-0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}},
		{{-0.5f, -0.5f, -0.5f}, { 1.0f,  1.0f,  1.0f}},

		// BACK
		{{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}},
		{{ 0.5f, -0.5f,  0.5f}, { 1.0f,  1.0f,  0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  1.0f}},
		{{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  1.0f}},
		{{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}},
		{{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}},

		// LEFT
		{{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}},
		{{-0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}},
		{{-0.5f, -0.5f, -0.5f}, { 1.0f,  1.0f,  1.0f}},
		{{-0.5f, -0.5f, -0.5f}, { 1.0f,  1.0f,  1.0f}},
		{{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}},
		{{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}},

		// RIGHT
		{{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  1.0f}},
		{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  1.0f}},
		{{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f,  0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f,  0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, { 1.0f,  1.0f,  0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  1.0f}},

		// TOP
		{{-0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  1.0f}},
		{{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  1.0f}},
		{{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  1.0f}},
		{{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}},
		{{-0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}},

		// BUTTOM
		{{-0.5f, -0.5f, -0.5f}, { 1.0f,  1.0f,  1.0f}},
		{{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f,  0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, { 1.0f,  1.0f,  0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, { 1.0f,  1.0f,  0.0f}},
		{{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}},
		{{-0.5f, -0.5f, -0.5f}, { 1.0f,  1.0f,  1.0f}}
	};


	Mesh()
	{}

	~Mesh()
	{}



	void push_data(std::vector<Vertex> &vertices, std::vector<glm::mat4> &mesh_data, unsigned int &mesh_count)
	{
		mesh_count++;
		index = mesh_count;

		// PUSH VERTICES TO JVK ENGINE
		for (size_t i = 0; i < m_vertices.size(); i++)
		{
			vertices.push_back(m_vertices[i]);
		}

		// PUSH MATRIX TO JVK ENGINE
		mesh_data.push_back(transformMatrix);
	}


	void sync(std::vector<glm::mat4> &mesh_data)
	{
		// PUSH MATRIX TO JVK ENGINE
		mesh_data[index - 1] = transformMatrix;
	}


	void rotate(float x, float y, float z)
	{
	    glm::vec3 eular = glm::vec3(
	        glm::radians(x),
	        glm::radians(y),
	        glm::radians(z)
	    );
	    glm::quat rotation = glm::quat(eular);
	    glm::mat4 rotationMatrix = glm::toMat4(rotation);
	    transformMatrix = transformMatrix * rotationMatrix;
	}

	void translate(float x, float y, float z)
	{
	    glm::mat4 identityMatrix = glm::mat4(1.0f);
	    glm::mat4 translationMatrix = glm::translate(identityMatrix, glm::vec3(x, y, z));
	    transformMatrix = transformMatrix * translationMatrix;
	}

	void scale(float x, float y, float z)
	{
	    glm::mat4 identityMatrix = glm::mat4(1.0f);
	    glm::mat4 scaleMatrix = glm::scale(identityMatrix, glm::vec3(x, y, z));
	    transformMatrix = transformMatrix * scaleMatrix;
	}
};