#include "context.h"
//#include "render.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "LoadShaders.h"

#include "glm/ext.hpp"

int main()
{
	Context Game;
	Game.Init(640, 480, "Source3");



	vertex TriVerts[] =
	{
		{{ -.5f, -.5f, 0, 1 }, {1,0,0,1}, {0,0}},//0
		{{ -.5f,  .5f, 0, 1 }, {1,1,0,1}, {0,1}},//1
		{{  .5f,  .5f, 0, 1 }, {0,1,1,1}, {1,0}},//2
		{{  .5f, -.5f, 0, 1 }, {1,1,1,1}, {1,1}}//3
	};

	unsigned int TriIndices[] = { 0,1,2,0,2,3 };
	//geometry Triangle = MakeGeometry(TriVerts, 4, TriIndices, 6);


	OBJ Object;
	Object.init("models/House1.obj");


	std::string BasicVert = ReadShader("Shaders/ShaderText.txt");
	std::string BasicFrag = ReadShader("Shaders/FragText.txt");


	shader BasicShader = MakeShader(BasicVert.c_str(), BasicFrag.c_str());

	glm::mat4 Model = glm::identity<glm::mat4>();

	glm::mat4 CamProj = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 1000.0f);
	glm::mat4 CamView = glm::lookAt(glm::vec3(0, 0, -250), glm::vec3(0, 50, 0), glm::vec3(0, 1, 0));


	texture Cube = loadTexure("Texture/House1.png");


	SetUniform(BasicShader, 0, CamProj);
	SetUniform(BasicShader, 1, CamView);
	SetUniform(BasicShader, 3, Cube, 0);



	while (!Game.ShouldClose())
	{
		Game.Tick();
		Game.Clear();


		SetUniform(BasicShader, 2, Model);

		Model = glm::rotate(Model, glm::radians(1.f), glm::vec3(0, .1f, 0));

		Draw(BasicShader, Object.MyGeo);
	}
	FreeGeometry(Object.MyGeo);
	Game.Term();
	return 0;
}