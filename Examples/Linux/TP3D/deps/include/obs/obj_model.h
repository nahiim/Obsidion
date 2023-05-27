// #ifndef MESH_INCLUDED_H
// #define MESH_INCLUDED_H

#pragma once

// #include "stb_image.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <string>
#include <vector>

#include "obj_loader.h"
#include "vertex.h"
#include "shader.h"
#include "camera.h"
#include "light.h"



enum MeshBufferPositions
{
	POSITION_VB,
	TEXCOORD_VB,
	NORMAL_VB,
	INDEX_VB
};


class ObjModel
{
	public:
		ObjModel();
	    ObjModel(const std::string& fileName, const std::string& texturePath);
		ObjModel(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);

		void setProjection(glm::mat4 projection);

		void Draw(Camera camera, Light *light, float ambience);

		void Update();

		void translate(float x, float y, float z);
		void rotate(float x, float y, float z);
		void scale(float x, float y, float z);

		virtual ~ObjModel();
		
	protected:


	private:
		static const unsigned int NUM_BUFFERS = 4;
		// void operator=(const Model& mesh) {}
		ObjModel(const ObjModel& mesh) {}

		void LoadTexture(const std::string& texturePath);

	    void InitModel(const IndexedModel& model);

		GLuint m_vertexArrayObject;
		GLuint m_vertexArrayBuffers[NUM_BUFFERS];
		unsigned int m_numIndices;

		GLuint m_texture;

	    glm::mat4 modelMatrix;
	    glm::mat4 view, projection;

	    Shader* shader;
};

// #endif
