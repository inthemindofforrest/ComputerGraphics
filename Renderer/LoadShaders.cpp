#include "LoadShaders.h"


std::string ReadShader(const char * _FileLocaiton)
{
	std::string FinalString = "";
	std::string Line;
	std::ifstream ReadFile;
	ReadFile.open(_FileLocaiton, std::ios::in);
	
	if (ReadFile.is_open())
	{
		while (std::getline(ReadFile,Line))
		{
			FinalString += Line + "\n";
		}
		ReadFile.close();
	}
	else std::cout << "Unable to open" << _FileLocaiton;
	return FinalString;
}

void OBJ::init(std::string _ObjName)
{
	std::vector<vertex> cachedVertices;
	std::vector<unsigned int> cachedIndices;

	std::string warn, err;

	bool ret = tinyobj::LoadObj(&Att, &shapes, &Mat, &warn, &err, _ObjName.c_str());

	if (!warn.empty())
		std::cout << warn << std::endl;

	if (!err.empty())
		std::cerr << err << std::endl;

	if (!ret)
		exit(1);

	// Loop shapes
	for (size_t s = 0; s < shapes.size(); s++)
	{
		// Loop faces
		size_t index_offset = 0;

		for (size_t Face = 0; Face < shapes[s].mesh.num_face_vertices.size(); Face++)
		{
			int fv = shapes[s].mesh.num_face_vertices[Face];
			// Loop vertices in the face.
			for (size_t Ver = 0; Ver < fv; Ver++)
			{
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + Ver];
				//Get three vertices
				tinyobj::real_t vx = Att.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = Att.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = Att.vertices[3 * idx.vertex_index + 2];

				//Get the normals for the three verticies
				tinyobj::real_t nx = Att.normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = Att.normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = Att.normals[3 * idx.normal_index + 2];

				//Get the tex coord
				tinyobj::real_t tx = Att.texcoords[2 * idx.texcoord_index + 0];
				tinyobj::real_t ty = Att.texcoords[2 * idx.texcoord_index + 1];

				//Push it all to make the model
				cachedVertices.push_back({ {vx, vy, vz, 1}, {nx, ny, nz, 0}, {tx, ty} });
				cachedIndices.push_back(3 * Face + Ver);
			}
			index_offset += fv;
		}
	}

	MyGeo = MakeGeometry(&cachedVertices[0], cachedVertices.size(), &cachedIndices[0], cachedIndices.size());
}
