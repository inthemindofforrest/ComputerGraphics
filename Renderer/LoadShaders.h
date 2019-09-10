#pragma once
#include "render.h"

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include "tobjl/tiny_obj_loader.h"

#include "math.h"

class OBJ
{
	std::string Input;
	tinyobj::attrib_t Att;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> Mat;

	std::string Err;
	std::string Warn;

public:
	bool LoadedTexture;

	geometry MyGeo;

	void init(std::string _ObjName);
};

std::string ReadShader(const char * _FileLocaiton);
