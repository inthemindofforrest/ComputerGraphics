#include "context.h"
//#include "render.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "LoadShaders.h"

#include "glm/ext.hpp"
#include <string>

int main()
{
	Context Game;
	Game.Init(640, 480, "Source3");

	OBJ Object;
	Object.init("models/Girl.obj");


	std::string BasicVert = ReadShader("Shaders/ShaderText.txt");
	std::string BasicFrag = ReadShader("Shaders/FragText.txt");


	shader BasicShader = MakeShader(BasicVert.c_str(), BasicFrag.c_str());

	glm::mat4 Model = glm::identity<glm::mat4>();

	glm::mat4 CamProj = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 1000.0f);
	glm::mat4 CamView = glm::lookAt(glm::vec3(0, 0, -100), glm::vec3(0, 25, 0), glm::vec3(0, 1, 0));


	texture Cube = loadTexure("Texture/Girl.png");

	light sun;
	sun.direction = glm::vec4{ 0,-1, 1,1 };
	sun.color = glm::vec4{ 1, 1, 1, 1 };

	SetUniform(BasicShader, 0, CamProj);
	SetUniform(BasicShader, 1, CamView);
	SetUniform(BasicShader, 3, Cube, 0);
	SetUniform(BasicShader, 5, sun.color);
	

	while (!Game.ShouldClose())
	{
		Game.Tick();
		Game.Clear();


		glm::mat4 sunRotation = glm::rotate(glm::identity<glm::mat4>(), glm::radians(.1f), glm::vec3(0, 0, 1));
		sun.direction = glm::vec4(sun.direction, 0) * sunRotation;

		Model = glm::rotate(Model, glm::radians(1.f), glm::vec3(0, 1, 0));
		SetUniform(BasicShader, 2, Model);
		SetUniform(BasicShader, 4, sun.direction);
		Draw(BasicShader, Object.MyGeo);
	}
	FreeGeometry(Object.MyGeo);
	Game.Term();
	return 0;
}