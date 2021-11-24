#include "Render.h"


using namespace std;

Render::Render() {
	
}


// Global initialisers
//GLFWwindow* Render::window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Engine Test", NULL, NULL);
Camera Render::camera(glm::vec3(0.0f, 0.0f, 3.0f));
float Render::lastX = SCR_WIDTH / 2.0f;
float Render::lastY = SCR_HEIGHT / 2.0f;
bool Render::firstMouse = true;

float Render::deltaTime = 0.0f;
float Render::lastFrame = 0.0f;

bool mouseVisibility = false;

unsigned int skyboxVAO, skyboxVBO;
unsigned int cubemapTexture;


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Render::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void Render::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
		{
			/*camera.cursor_state(window, GLFW_CURSOR_DISABLED);*/
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			if (firstMouse)
			{
				lastX = static_cast<float>(xpos);
				lastY = static_cast<float>(ypos);
				firstMouse = false;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos;

			lastX = xpos;
			lastY = ypos;

			camera.ProcessMouseMovement(xoffset, yoffset);
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			lastX = SCR_WIDTH / 2;
			lastY = SCR_HEIGHT / 2;
			firstMouse = true;
		}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void Render::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);

	cout << "Mouse scrolled" << endl;
}




//--------------------------------------
//Main Methods--------------------------
//--------------------------------------
void Render::CreateWindow() {
	// glfw window creation
// --------------------
	
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		//return -1; 
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

void Render::InitRenderer() {

	/*cout << "confirm Globals" << endl;
	cout << "Camera: " + to_string(camera.Position.x) +", "+ to_string(camera.Position.y) + ", " + to_string(camera.Position.z) << endl;
	cout << "Last X: " + to_string(lastX) << endl;
	cout << "Last Y: " + to_string(lastY) << endl;*/
	//camera
	//camera = (glm::vec3(0.0f, 0.0f, 0.0f));
	//Render::lastX = SCR_WIDTH / 2.0f;
	//Render::lastY = SCR_HEIGHT / 2.0f;
	//Render::firstMouse = true;



	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// set Flashlight Params
	//flashlight = SpotLight(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), RGB(0.0f, 0.0f, 0.0f), RGB(1.0f, 1.0f, 1.0f), RGB(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 12.5f, 15.0f);
	flashlight.SetAmbient(0.0f, 0.0f, 0.0f);
	flashlight.SetDiffuse(2.0f, 2.0f, 2.0f);
	flashlight.SetSpecular(1.0f, 1.0f, 1.0f);

	flashlight.SetConstant(1.0f);
	flashlight.SetLinear(0.09f);
	flashlight.SetQuadratic(0.032f);

	flashlight.SetCutOff(12.5f);
	flashlight.SetOuterCutOff(15.0f);



#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Engine Test", NULL, NULL);
	cout << "Window: " + to_string(window==NULL) << endl;
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		//return -1; 
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		//return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	shader = Shader("model.vert", "model.frag");
	skyboxShader = Shader("skybox.vert", "skybox.frag");

	//Load Models
	LoadModels();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// skybox VAO
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	vector<std::string> faces
	{
		"Skybox_Right.png",
		"Skybox_Left.png",
		"Skybox_Top.png",
		"Skybox_Bottom.png",
		"Skybox_Front.png",
		"Skybox_Back.png"
	};
	cubemapTexture = loadCubemap(faces);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

}

void Render::UpdateRender() {
	
	// per-frame time logic
		// --------------------
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// input
	// -----
	processInput(window);

	// render
	// ------
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// view/projection transformation
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	// be sure to activate shader when setting uniforms/drawing objects
	shader.use();
	shader.setInt("noSpotLights", 1);
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);

	/*
	   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
	   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
	   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
	   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
	*/

	// Lights render
	RenderLights();

	//model render
	/*cout << "Total rendered models: " << models.size() << endl;*/
	for (Model m : models) {
		m.render(shader);
	}



	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader.use();
	view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
	skyboxShader.setMat4("view", view);
	skyboxShader.setMat4("projection", projection);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);


	
	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	/*glfwSwapBuffers(window);
	glfwPollEvents();*/
}

void Render::CleanUp() {

	//MODEL IMPORT CLEANUP
	for (int i = 0; i < models.size(); i++)
	{
		models[i].cleanup();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
}

//--------------------------------------
//Auxilary Methods----------------------
//--------------------------------------


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Render::processInput(GLFWwindow* window)
{
	// quit
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		// movement
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			camera.ProcessKeyboard(UPWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			camera.ProcessKeyboard(DOWNWARD, deltaTime);
	}
}

void Render::LoadModels() {
	for (int i = 0; i < models.size(); i++)
	{
		models[i].loadModel(models[i].mPath);
	}
}

void Render::LoadModel(int pos) {
	models[pos].loadModel(models[pos].mPath);
}

//Adds single light to list of lights
void Render::AddDirLight(DirectionalLight light) {
	directionalLight = light;
}
void Render::AddSpotLight(SpotLight light) {
	spotLights.push_back(light);
}
void Render::AddPointLight(PointLight light) {
	pointLights.push_back(light);
}


//Adds Model
void Render::AddModel(Model m) {
	models.push_back(m);
}


//Adds Multiple Models
void Render::AddModel(vector<Model> models) {
	for (Model m : models) {
		Render::models.push_back(m);
	}	
}

int Render::GetTriangles() {
	int triangleCount = 0;
	for (Model m : models) {
		
		triangleCount += m.GetTriangles();
	}
	return triangleCount;
}

void Render::ClearData() {
	models.clear();
	spotLights.clear();
	pointLights.clear();
}

void Render::RenderLights() {
	directionalLight.RenderLight(shader);
	for (int i = 0; i < spotLights.size(); i++) {
		spotLights[i].RenderLight(shader, to_string(1+i) + "");
	}
	/*cout << "Loaded spots" << endl;*/
	for (int i = 0; i < pointLights.size(); i++)
	{
		pointLights[i].RenderLight(shader,to_string(i) );
	}
	/*cout << "Loaded points" << endl;*/
	// update flashlight
	flashlight.SetPosition(camera.Position.x, camera.Position.y, camera.Position.z);
	flashlight.SetDirection(camera.Front.x, camera.Front.y, camera.Front.z);
	flashlight.RenderLight(shader, "0");
}

unsigned int Render::loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}