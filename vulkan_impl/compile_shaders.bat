

glslc shaders/shader.vert -o vert.spv
glslc shaders/shader.frag -o frag.spv

glslc shaders/skybox.vert -o sky_vert.spv
glslc shaders/skybox.frag -o sky_frag.spv

move *.spv bin/res/shaders

pause