#include "render.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "glm/gtc/type_ptr.hpp"

geometry MakeGeometry(vertex * _verts, size_t _vertCount, unsigned * _indices, size_t _indxCount)
{
	//Create an instance of geometry
	geometry NewGeo = {};
	NewGeo.size = _indxCount;
	//generate buffer
	glGenVertexArrays(1, &NewGeo.vao);	//Vertex array object
	glGenBuffers(1, &NewGeo.vbo);		//Vertex buffer object
	glGenBuffers(1, &NewGeo.ibo);		//Index buffer object
	//bind buffers
	glBindVertexArray(NewGeo.vao);
	glBindBuffer(GL_ARRAY_BUFFER, NewGeo.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NewGeo.ibo);
	//populate buffers
	glBufferData(GL_ARRAY_BUFFER, _vertCount * sizeof(vertex), _verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indxCount * sizeof(unsigned), _indices, GL_STATIC_DRAW);
	//describe vertext data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)16);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)32);
	//unbind buffer (in a specific order)
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//return the geometry
	return NewGeo;

}

void FreeGeometry(geometry & _geo)
{
	glDeleteBuffers(1, &_geo.vbo);
	glDeleteBuffers(1, &_geo.ibo);
	glDeleteVertexArrays(1, &_geo.vao);
	_geo = {};
}

shader MakeShader(const char * _vertSource, const char * _fragSource)
{
	//Make shader object
	shader NewShad = {};
	NewShad.Program = glCreateProgram();

	//create the shaders
	GLuint Vert = glCreateShader(GL_VERTEX_SHADER);
	GLuint Frag = glCreateShader(GL_FRAGMENT_SHADER);

	//compile the shaders
	glShaderSource(Vert, 1, &_vertSource, 0);
	glShaderSource(Frag, 1, &_fragSource, 0);
	glCompileShader(Vert);

	//Error checking-------------------------------------------
	GLint Success = 0;
	glGetShaderiv(Vert, GL_COMPILE_STATUS, &Success);
	if (Success == 0) std::cout << "Vert failed to compile\n";

	glCompileShader(Frag);

	//Error checking-------------------------------------------
	glGetShaderiv(Vert, GL_COMPILE_STATUS, &Success);
	if (Success == 0) std::cout << "Frag failed to compile\n";

	//attach the shaders
	glAttachShader(NewShad.Program, Vert);
	glAttachShader(NewShad.Program, Frag);

	//link the shaders
	glLinkProgram(NewShad.Program);

	//Error checking-------------------------------------------
	glGetShaderiv(NewShad.Program, GL_LINK_STATUS, &Success);
	if (Success == 0) std::cout << "NewShad failed to Link\n";

	//delete the shaders
	glDeleteShader(Vert);
	glDeleteShader(Frag);
	
	//return the shader objects
	return NewShad;
}

void FreeShader(shader & _shad)
{
	glDeleteProgram(_shad.Program);
	_shad = {};
}

void Draw(const shader & _shad, const geometry & _geo)
{
	//Bind the shader program
	glUseProgram(_shad.Program);

	//Bind the VAO (geo and indices)
	glBindVertexArray(_geo.vao);

	//Draw
	glDrawElements(GL_TRIANGLES, _geo.size, GL_UNSIGNED_INT, 0);
}

void SetUniform(const shader & _shad, GLuint _location, const glm::mat4 & _value)
{
	glProgramUniformMatrix4fv(_shad.Program, _location, 1, GL_FALSE, glm::value_ptr(_value));
}

void SetUniform(const shader & _shad, GLuint _location, const texture & _value, int _textureSlot)
{
	//specify the texture slot we're working with
	glActiveTexture(GL_TEXTURE0 + _textureSlot);

	//bind the texture to that slot
	glBindTexture(GL_TEXTURE_2D, _value.handle);

	//assign the uniform to the shader
	glProgramUniform1i(_shad.Program, _location, _textureSlot);
}

texture MakeTexture(unsigned int _width, unsigned int _height, unsigned int _channels, const unsigned char * _pixels)
{
	GLenum oglFormat = GL_RGBA;
	switch (_channels)
	{
	case 1:
		oglFormat = GL_RED;
		break;
	case 2:
		oglFormat = GL_RG;
		break;
	case 3:
		oglFormat = GL_RGB;
	case 4:
		oglFormat = GL_RGBA;
		break;
	default:
		//TODO: fatal error, halt the program
		break;
	}

	texture Tex = { 0,_width, _height, _channels };

	//generating and binding the texture
	glGenTextures(1, &Tex.handle);
	glBindTexture(GL_TEXTURE_2D, Tex.handle);
	
	//buffer/send the actual data
	glTexImage2D(GL_TEXTURE_2D, 0, oglFormat, _width, _height, 0, oglFormat, GL_UNSIGNED_BYTE, _pixels);

	//describe how the texture will be used
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	return Tex;
}

void FreeTexture(texture & _tex)
{
	glDeleteTextures(1, &_tex.handle);
	_tex = {};
}

texture loadTexure(const char * _imagePath)
{
	int ImageWidth, ImageHeight, ImageFormat;
	unsigned char *RawPixelData = nullptr;

	//tell stb image to load the image
	stbi_set_flip_vertically_on_load(true);
	RawPixelData = stbi_load(_imagePath, &ImageWidth, &ImageHeight, &ImageFormat, STBI_default);
	//TODO: ensure that rawpixeldata is not null. if it is, the image failed to load
	
	//pass the data to make the texture
	texture Tex = MakeTexture(ImageWidth, ImageHeight, ImageFormat, RawPixelData);
	//free the image
	stbi_image_free(RawPixelData);

	return Tex;
}
