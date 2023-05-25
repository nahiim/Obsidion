#pragma once


#include <iostream>

#include <glm/glm.hpp>

#include <obs/Shader.h>

#include <ft2build.h>
#include FT_FREETYPE_H 



struct Character
{
    unsigned int textureID;  // ID handle of the glyph texture
    glm::ivec2   size;       // Size of glyph
    glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;    // Offset to advance to next glyph
};


class Text
{

private:

	FT_Library ft;
	FT_Face face;
	std::map<char, Character> Characters;

	Shader *s;
	unsigned int VAO, VBO;

	void configQuads()
	{
	    // configure VAO/VBO for texture quads
	    // -----------------------------------
	    glGenVertexArrays(1, &VAO);
	    glGenBuffers(1, &VBO);
	    glBindVertexArray(VAO);
	    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	    glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	    glBindBuffer(GL_ARRAY_BUFFER, 0);
	    glBindVertexArray(0);
	}

	void init(const std::string& path)
	{
		if (FT_Init_FreeType(&ft))
		{
		    throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");
		}

		if (FT_New_Face(ft, path.c_str(), 0, &face))
		{
		    throw std::runtime_error("ERROR::FREETYPE: Failed to load font");
		}else
		{
			loadGlyph();
		}
	}

	void loadGlyph()
	{
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
		  
		for (unsigned char c = 0; c < 128; c++)
		{
		    // load character glyph 
		    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		    {
		        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
		        continue;
		    }
		    // generate texture
		    unsigned int texture;
		    glGenTextures(1, &texture);
		    glBindTexture(GL_TEXTURE_2D, texture);
		    glTexImage2D(
		        GL_TEXTURE_2D,
		        0,
		        GL_RED,
		        face->glyph->bitmap.width,
		        face->glyph->bitmap.rows,
		        0,
		        GL_RED,
		        GL_UNSIGNED_BYTE,
		        face->glyph->bitmap.buffer
		    );
		    // set texture options
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		    // now store character for later use
		    Character character = {
		        texture, 
		        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
		        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
		        face->glyph->advance.x
		    };
		    Characters.insert(std::pair<char, Character>(c, character));
		    glBindTexture(GL_TEXTURE_2D, 0);
		}


		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

public:

	// Default constructor
	Text()
	{}


	Text(const std::string& path)
	{
		init(path);
		configQuads();		
	}

	void setProjection(const glm::mat4& projection)
	{
		s = new Shader("res/shaders/font_vert.glsl", "res/shaders/font_frag.glsl");
		s->use();
		s->setMat4("projection", projection);
	}


	void render(std::string text, float x, float y, float scale, glm::vec3 color)
	{
	    // activate corresponding render state	
	    s->use();
	    s->setVec3("text_color", glm::vec3(color.x, color.y, color.z));

	    glActiveTexture(GL_TEXTURE0);
	    glBindVertexArray(VAO);

	    // iterate through all characters
	    std::string::const_iterator c;
	    for (c = text.begin(); c != text.end(); c++)
	    {
	        Character ch = Characters[*c];

	        float xpos = x + ch.bearing.x * scale;
	        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

	        float w = ch.size.x * scale;
	        float h = ch.size.y * scale;
	        // update VBO for each character
	        float vertices[6][4] = {
	            { xpos,     ypos + h,   0.0f, 0.0f },            
	            { xpos,     ypos,       0.0f, 1.0f },
	            { xpos + w, ypos,       1.0f, 1.0f },

	            { xpos,     ypos + h,   0.0f, 0.0f },
	            { xpos + w, ypos,       1.0f, 1.0f },
	            { xpos + w, ypos + h,   1.0f, 0.0f }           
	        };
	        // render glyph texture over quad
	        glBindTexture(GL_TEXTURE_2D, ch.textureID);
	        // update content of VBO memory
	        glBindBuffer(GL_ARRAY_BUFFER, VBO);
	        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
	        glBindBuffer(GL_ARRAY_BUFFER, 0);
	        // render quad
	        glDrawArrays(GL_TRIANGLES, 0, 6);
	        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
	        x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
	    }
	    glBindVertexArray(0);
	    glBindTexture(GL_TEXTURE_2D, 0);
	}
};