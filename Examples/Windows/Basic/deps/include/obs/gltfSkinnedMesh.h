


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

#include <obs/gltfSkinning.h>


// #define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define BUFFER_OFFSET(i) (reinterpret_cast<void *>(i))


#define CheckGLErrors(desc)
{
	GLenum e = glGetError();
	if (e != GL_NO_ERROR)
	{
		printf("OpenGL error in \"%s\": %d (%d) %s:%d\n", desc, e, e, __FILE__, __LINE__);
		exit(20);
	}
}


class gltfModel
{

public:


	glm::mat4 modelMatrix = glm::mat4(1.0f);
	// glm::mat4 model_rot = glm::mat4(1.0f);
	// glm::vec3 model_pos = glm::vec3(-3, 0, -3);

	gltfModel()
	{

	}


	gltfModel(const std::string &filename)
	{
		if (!loadModel(model, filename.c_str()))
		{
			std::cout << "Failure to load model" << std::endl;
			return;
		}

		vao = bindModel(model);
	}


	void setProjection(glm::mat4 projection)
	{

		shader = new Shader("gltf/shaders/gltf_vert.glsl", "gltf/shaders/gltf_frag.glsl");
		shader->use();
		shader->setMat4("projection", projection);
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
	    modelMatrix = modelMatrix * rotationMatrix;
	}

	void translate(float x, float y, float z)
	{
	    glm::mat4 identityMatrix = glm::mat4(1.0f);
	    glm::mat4 translationMatrix = glm::translate(identityMatrix, glm::vec3(x, y, z));
	    modelMatrix = modelMatrix * translationMatrix;
	}

	void scale(float x, float y, float z)
	{
	    glm::mat4 identityMatrix = glm::mat4(1.0f);
	    glm::mat4 scaleMatrix = glm::scale(identityMatrix, glm::vec3(x, y, z));
	    modelMatrix = modelMatrix * scaleMatrix;
	}


	std::map<int, GLuint> bindMesh(std::map<int, GLuint> vbos, tinygltf::Model &model, tinygltf::Mesh &mesh)
	{
		for (size_t i = 0; i < model.bufferViews.size(); ++i)
		{
			const tinygltf::BufferView &bufferView = model.bufferViews[i];
			if (bufferView.target == 0)
			{  // TODO impl drawarrays
		  		std::cout << "WARN: bufferView.target is zero" << std::endl;
		  		continue;  // Unsupported bufferView.
		             /*
		               From spec2.0 readme:
		               https://github.com/KhronosGroup/glTF/tree/master/specification/2.0
		                        ... drawArrays function should be used with a count equal to
		               the count            property of any of the accessors referenced by the
		               attributes            property            (they are all equal for a given
		               primitive).
		             */
			}

			const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
			std::cout << "bufferview.target " << bufferView.target << std::endl;

			GLuint vbo;
			glGenBuffers(1, &vbo);
			vbos[i] = vbo;
			glBindBuffer(bufferView.target, vbo);

			std::cout << "buffer.data.size = " << buffer.data.size()
			          << ", bufferview.byteOffset = " << bufferView.byteOffset
			          << std::endl;

			glBufferData(bufferView.target, bufferView.byteLength,
			             &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
		}

		for (size_t i = 0; i < mesh.primitives.size(); ++i)
		{
			tinygltf::Primitive primitive = mesh.primitives[i];
			tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

			for (auto &attrib : primitive.attributes)
			{
				tinygltf::Accessor accessor = model.accessors[attrib.second];
				int byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);
				glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);

				int size = 1;
				if (accessor.type != TINYGLTF_TYPE_SCALAR)
				{
		    		size = accessor.type;
				}

				int vaa = -1;
				if (attrib.first.compare("POSITION") == 0) vaa = 0;
				if (attrib.first.compare("NORMAL") == 0) vaa = 1;
				if (attrib.first.compare("TEXCOORD_0") == 0) vaa = 2;
				
				if (vaa > -1)
				{
					glEnableVertexAttribArray(vaa);
					glVertexAttribPointer
					(
						vaa,
						size,
						accessor.componentType,
						accessor.normalized ? GL_TRUE : GL_FALSE,
						byteStride,
						BUFFER_OFFSET(accessor.byteOffset)
					);
		  		}
		  		else
		  		{
		    		std::cout << "vaa missing: " << attrib.first << std::endl;
		  		}
			}

			if (model.textures.size() > 0)
			{
				// fixme: Use material's baseColor
				tinygltf::Texture &tex = model.textures[0];

				if (tex.source > -1)
				{

				    GLuint texid;
				    glGenTextures(1, &texid);

				    tinygltf::Image &image = model.images[tex.source];

				    glBindTexture(GL_TEXTURE_2D, texid);
				    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				    GLenum format = GL_RGBA;

				    if (image.component == 1) {
				      format = GL_RED;
				    } else if (image.component == 2) {
				      format = GL_RG;
				    } else if (image.component == 3) {
				      format = GL_RGB;
				    } else {
				      // ???
				    }

				    GLenum type = GL_UNSIGNED_BYTE;
				    if (image.bits == 8) {
				      // ok
				    } else if (image.bits == 16) {
				      type = GL_UNSIGNED_SHORT;
				    } else {
				      // ???
				    }

		    		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, format, type, &image.image.at(0));
				}
			}
		}

	 	

	 	return vbos;
	}






	// bind models
	void bindModelNodes(std::map<int, GLuint> vbos, tinygltf::Model &model, tinygltf::Node &node)
	{
		if ((node.mesh >= 0) && (node.mesh < model.meshes.size()))
		{
			bindMesh(vbos, model, model.meshes[node.mesh]);
		}

		for (size_t i = 0; i < node.children.size(); i++)
		{
			assert((node.children[i] >= 0) && (node.children[i] < model.nodes.size()));
			bindModelNodes(vbos, model, model.nodes[node.children[i]]);
		}
	}



	GLuint bindModel(tinygltf::Model &model)
	{
		std::map<int, GLuint> vbos;
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		const tinygltf::Scene &scene = model.scenes[model.defaultScene];
		for (size_t i = 0; i < scene.nodes.size(); ++i)
		{
			assert((scene.nodes[i] >= 0) && (scene.nodes[i] < model.nodes.size()));
			bindModelNodes(vbos, model, model.nodes[scene.nodes[i]]);
		}

		glBindVertexArray(0);
		// cleanup vbos
		for (size_t i = 0; i < vbos.size(); ++i)
		{
			glDeleteBuffers(1, &vbos[i]);
		}

		return vao;
	}




	void drawMesh(tinygltf::Model &model, tinygltf::Mesh &mesh)
	{
		for (size_t i = 0; i < mesh.primitives.size(); ++i)
		{
	    	tinygltf::Primitive primitive = mesh.primitives[i];
	    	tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

	    	glDrawElements(primitive.mode, indexAccessor.count, indexAccessor.componentType, BUFFER_OFFSET(indexAccessor.byteOffset));
	  	}
	}


	// recursively draw node and children nodes of model
	void drawModelNodes(tinygltf::Model &model, tinygltf::Node &node)
	{
  		if ((node.mesh >= 0) && (node.mesh < model.meshes.size()))
  		{
	    	drawMesh(model, model.meshes[node.mesh]);
		}

	  	
	  	for (size_t i = 0; i < node.children.size(); i++)
	  	{
	    	drawModelNodes(model, model.nodes[node.children[i]]);
	  	}
	}


	void drawModel(GLuint vao, tinygltf::Model &model)
	{
		glBindVertexArray(vao);

	  	const tinygltf::Scene &scene = model.scenes[model.defaultScene];
	  	
	  	for (size_t i = 0; i < scene.nodes.size(); ++i)
	  	{
	    	drawModelNodes(model, model.nodes[scene.nodes[i]]);
		}

	  	glBindVertexArray(0);
	}



	void draw(Camera camera)
	{
		shader->use();
		shader->setMat4("model", modelMatrix);
		shader->setMat4("view", camera.GetViewMatrix());

		drawModel(vao, model);
	}






	void dbgModel(tinygltf::Model &model)
	{
		for (auto &mesh : model.meshes)
		{
	    	std::cout << "mesh : " << mesh.name << std::endl;
	    	
	    	for (auto &primitive : mesh.primitives)
	    	{
	      		const tinygltf::Accessor &indexAccessor = model.accessors[primitive.indices];

				std::cout << "indexaccessor: count " << indexAccessor.count << ", type "
	                << indexAccessor.componentType << std::endl;

				tinygltf::Material &mat = model.materials[primitive.material];
	      		
	      		for (auto &mats : mat.values)
	      		{
	        		std::cout << "mat : " << mats.first.c_str() << std::endl;
				}

	      		for (auto &image : model.images)
	      		{
			        std::cout << "image name : " << image.uri << std::endl;
			        std::cout << "  size : " << image.image.size() << std::endl;
			        std::cout << "  w/h : " << image.width << "/" << image.height
	                  << std::endl;
				}

				std::cout << "indices : " << primitive.indices << std::endl;
				std::cout << "mode     : "
	                << "(" << primitive.mode << ")" << std::endl;

				for (auto &attrib : primitive.attributes)
				{
	        		std::cout << "attribute : " << attrib.first.c_str() << std::endl;
				}
		    }
		}
	}


private:

	tinygltf::Model model;
	std::string filename;
	Shader *shader;
	GLuint vao;




	bool loadModel(tinygltf::Model &model, const char *filename)
	{
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;

		bool res = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
		if (!warn.empty())
			std::cout << "WARN: " << warn << std::endl;


		if (!err.empty())
			std::cout << "ERR: " << err << std::endl;

		if (!res)
			std::cout << "Failed to load glTF: " << filename << std::endl;
		else
			std::cout << "Loaded glTF: " << filename << std::endl;


		return res;
	}



};