#pragma once

#include "glew/GL/glew.h"
#include "glm/glm.hpp"


struct vertex
{
	glm::vec4 pos;
	glm::vec4 normal;
	glm::vec2 uv;

public:
	vertex()
	{

	}
	vertex(glm::vec4 _pos, glm::vec4 _normal)
	{
		pos = _pos;
		normal = _normal;
	}
	vertex(glm::vec4 _pos, glm::vec4 _normal, glm::vec2 _uv)
	{
		pos = _pos;
		normal = _normal;
		uv = _uv;
	}
	int operator=(vertex* _ver)
	{
		normal.x = (*_ver).normal.x;
		normal.y = (*_ver).normal.y;
		normal.z = (*_ver).normal.z;
		normal.w = (*_ver).normal.w;

		pos.x = (*_ver).pos.x;
		pos.y = (*_ver).pos.y;
		pos.z = (*_ver).pos.z;
		pos.w = (*_ver).pos.w;

		uv.x = (*_ver).uv.x;
		uv.y = (*_ver).uv.y;

		return 0;
	}
};

struct geometry
{
	GLuint vao, vbo, ibo;//buffers
	GLuint size;		 //index count
};

struct shader
{
	GLuint Program;
};

struct texture
{
	GLuint handle;
	unsigned int width, height, channels;
};

struct light
{
	glm::vec3 direction;
	glm::vec4 color;
};

geometry MakeGeometry(vertex * _verts, size_t _vertCount, unsigned * _indices, size_t _indxCount);

void FreeGeometry(geometry &_geo);

shader MakeShader(const char * _vertSource, const char * _fragSource);

void FreeShader(shader &_shad);

void Draw(const shader &_shad, const geometry &_geo);

void SetUniform(const shader &_shad, GLuint _location, const glm::mat4 &_value);
void SetUniform(const shader &_shad, GLuint _location, const glm::vec3 &_value);
void SetUniform(const shader &_shad, GLuint _location, const glm::vec4 &_value);
void SetUniform(const shader &_shad, GLuint _location, const texture &_value, int _textureSlot);

texture MakeTexture(unsigned int _width, unsigned int _height, unsigned int _channels, const unsigned char *_pixels/*byte data*/);

void FreeTexture(texture &_tex);

texture loadTexure(const char *_imagePath);