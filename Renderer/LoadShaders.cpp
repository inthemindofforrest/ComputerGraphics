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
	Input = _ObjName;
	std::vector<tinyobj::shape_t> shapes;
	LoadedTexture = tinyobj::LoadObj(&Att, &shapes, &Mat, &Warn, &Err, Input.c_str());

	if (!Warn.empty()) {
		std::cout << Warn << std::endl;
	}

	if (!Err.empty()) {
		std::cerr << Err << std::endl;
	}

	if (LoadedTexture)
	{
		std::vector<vertex> Vert;
		std::vector<unsigned int> Indice;



		for (size_t s = 0; s < shapes.size(); s++)//Shapes
		{
			size_t Offset = 0;
			for (size_t Face = 0; Face < shapes[s].mesh.num_face_vertices.size(); Face++)//Shape Faces
			{
				size_t FV = shapes[s].mesh.num_face_vertices[Face];
				for (size_t Ver = 0; Ver < FV; Ver++)//Vertices in face
				{
					tinyobj::index_t idx = shapes[s].mesh.indices[Offset + Ver];
					tinyobj::real_t vx = Att.vertices[3 * idx.vertex_index + 0];
					tinyobj::real_t vy = Att.vertices[3 * idx.vertex_index + 1];
					tinyobj::real_t vz = Att.vertices[3 * idx.vertex_index + 2];

					tinyobj::real_t nx = Att.normals[3 * idx.vertex_index + 0];
					tinyobj::real_t ny = Att.normals[3 * idx.vertex_index + 1];
					tinyobj::real_t nz = Att.normals[3 * idx.vertex_index + 2];

					tinyobj::real_t tx = Att.texcoords[2 * idx.vertex_index + 0];
					tinyobj::real_t ty = Att.texcoords[2 * idx.vertex_index + 1];

					Vert.push_back({ { vx,vy,vz,1 }, { nx,ny,nz,0 }, {tx, ty} });
					Indice.push_back(3 * Face + Ver);
				}
				Offset += FV;
			}
		}
		MyGeo = MakeGeometry(&Vert[0], Vert.size(), &Indice[0], Indice.size());
	}
	else exit(1);

	
}
