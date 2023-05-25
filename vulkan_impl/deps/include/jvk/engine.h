
#pragma once

#include "window.h"
#include "vertex.h"


class Engine
{

public:

	Engine();

	~Engine();

	void render(std::vector<glm::mat4> mesh_data, glm::mat4 view, glm::mat4 projection);

	// setup
	void init(std::vector<Vertex> l_vertices, std::vector<glm::mat4> matrices);


	Window *window;
};