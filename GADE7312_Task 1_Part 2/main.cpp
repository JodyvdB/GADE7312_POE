#include <json.hpp>
#include "Console.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static const unsigned int SCR_WIDTH = 800 * 2;
static const unsigned int SCR_HEIGHT = 600 * 1.5f;

int main()
{
	//GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	
	Render renderer;
	
	LevelManager::LoadLevel(&renderer, "Empty Scene");

	renderer.InitRenderer();

	Console console(&renderer);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(renderer.window))
	{
		renderer.UpdateRender();
		console.UpdateConsole();
	}

	LevelManager::SaveLevel(&renderer, console.currentLevel);

	renderer.CleanUp();
	console.ConsoleCleanup();
}
