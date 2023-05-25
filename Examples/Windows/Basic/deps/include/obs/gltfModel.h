


#pragma once



#include "stb_image.h"

#include <gl/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

#include <string>
#include <vector>


#include <tiny_gltf/tiny_gltf.h>


#define BUFFER_OFFSET(i) ((char *)NULL + (i))



class gltfModel
{

public:


	glm::mat4 modelMatrix = glm::mat4(1.0f);




	gltfModel()
	{

	}


	gltfModel(const std::string &filename)
	{}


	void setProjection(glm::mat4 projection)
	{}

	void rotate(float x, float y, float z)
	{}

	void translate(float x, float y, float z)
	{}

	void scale(float x, float y, float z)
	{}


	std::map<int, GLuint> bindMesh(std::map<int, GLuint> vbos, tinygltf::Model &model, tinygltf::Mesh &mesh)
	{}






	// bind models
	void bindModelNodes(std::map<int, GLuint> vbos, tinygltf::Model &model, tinygltf::Node &node)
	{}



	GLuint bindModel(tinygltf::Model &model)
	{}




	void drawMesh(tinygltf::Model &model, tinygltf::Mesh &mesh)
	{}


	// recursively draw node and children nodes of model
	void drawModelNodes(tinygltf::Model &model, tinygltf::Node &node)
	{}


	void drawModel(GLuint vao, tinygltf::Model &model)
	{}



	void draw(Camera camera)
	{}






	void dbgModel(tinygltf::Model &model)
	{}


private:

	tinygltf::Model model;
	std::string filename;
	Shader *shader;
	GLuint vao;




	bool loadModel(tinygltf::Model &model, const char *filename)
	{}



};