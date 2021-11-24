
#ifndef RENDER_H
#define RENDER_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "model.h"
#include "texture.h"
#include "shader.h"
#include "Light.h"

#include <iostream>
#include <vector>

using namespace std;

class Render
{
public:
	Render(); 

	//Lights & Modles
	DirectionalLight directionalLight;
	vector<SpotLight> spotLights;
	vector<PointLight> pointLights;

	vector<Model> models;

	//unsigned int loadTexture(const char* path);

	// window settings
	GLFWwindow* window = glfwCreateWindow(800 * 2, 600 * 1.5f, "Engine Test", NULL, NULL);
	static const unsigned int SCR_WIDTH = 800 * 2;
	static const unsigned int SCR_HEIGHT = 600 * 1.5f;

	// camera
	static Camera camera; //= (glm::vec3(0.0f, 0.0f, 0.0f));
	static float lastX; //= SCR_WIDTH / 2.0f;
	static float lastY; //= SCR_HEIGHT / 2.0f;
	static bool firstMouse; //= true;

	// timing
	static float deltaTime;
	static float lastFrame;
	
	

private: 
	//shader stuff
	Shader shader;
	Shader skyboxShader;
	SpotLight flashlight;


public:

	// add assets
	void AddModel(Model model);
	void AddModel(vector<Model> models);
	int GetTriangles();
	void ClearData();
	void AddDirLight(DirectionalLight light);
	void AddSpotLight(SpotLight light);
	void AddPointLight(PointLight light);

	//set callbacks
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	void InitRenderer();
	void CreateWindow();
	void UpdateRender();
	void CleanUp();
	
	void processInput(GLFWwindow* window);
	//void LoadLights();
	void LoadModels();
	void LoadModel(int pos);
	void RenderLights();


	unsigned int loadCubemap(vector<std::string> faces);
};
#endif


