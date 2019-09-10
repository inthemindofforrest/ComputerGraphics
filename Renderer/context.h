#pragma once

class Context
{
private:
	struct GLFWwindow * Window;

public :
	bool Init(int _width, int _height, const char * _title);
	void Tick();
	void Term();
	void Clear();

	bool ShouldClose() const;
};