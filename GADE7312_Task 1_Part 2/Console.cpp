#include "Console.h"

bool showFPS = true;

bool showTriangles = false;

bool help = true;

char levelLoad[128] = "";

char modelImport[128] = "";
float posx, posy, posz;
float scalex, scaley, scalez;

bool actionCompleted = false;
bool errorOccured = false;
int feedbackCode = 0;

int triangleCount = 0;

Console::Console(Render* render) {
	renderer = render;
	currentLevel = "Wolfenstein Level";
	
}

void Console::DisplayInputWindow() {
	ImGui::Begin("Console");

	ImGui::Text("NOTE: Hold right click to move and interact with the camera.");

	ImGui::Text("\n\nStats\n");

	ImGui::Checkbox("Show FPS", &showFPS);

	ImGui::Checkbox("Show Triangles", &showTriangles);

	ImGui::Checkbox("Help?", &help);

	ImGui::Text("\n\n\nCommands:\n");

	ImGui::Text("Import a Model:\n");

	ImGui::PushItemWidth(300.0f);

	ImGui::InputText("<-- Model Directory", modelImport, IM_ARRAYSIZE(modelImport));

	ImGui::PushItemWidth(60.0f);
	ImGui::InputFloat("xPos", &posx);
	ImGui::SameLine();
	ImGui::InputFloat("yPos", &posy);
	ImGui::SameLine();
	ImGui::InputFloat("zPos", &posz);

	ImGui::InputFloat("xscale", &scalex);
	ImGui::SameLine();
	ImGui::InputFloat("yscale", &scaley);
	ImGui::SameLine();
	ImGui::InputFloat("zscale", &scalez);

	if (ImGui::Button("Import Model")) {
		ImportModel();
	}

	ImGui::Text("\n\nLevel Handling\n");

	ImGui::PushItemWidth(300.0f);

	ImGui::InputText("<-- Level Name", levelLoad, IM_ARRAYSIZE(levelLoad));

	if (ImGui::Button("Load Level")) {
		LoadLevel();
	}

	ImGui::SameLine();

	if (ImGui::Button("Save Level")) {
		SaveLevel();
	}

	ImGui::End();
}

void Console::DisplayOutputWindow() {
	ImGui::Begin("Output");

	if (showFPS) {
		ShowFPS();
	}

	if (showTriangles) {
		ShowTriangleCount();
	}

	if (help) {
		Help();
	}



	if (errorOccured || actionCompleted) {
		FeedbackHandler();
	}

	ImGui::End();
}

void Console::ShowFPS() {
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
}

void Console::ShowTriangleCount() {
	triangleCount = renderer->GetTriangles();

	ImGui::Text("\n%d", triangleCount);
}

void Console::Help() {
	ImGui::Text("\nHELP:\nTo import a model, provide the correct file path to the .obj/fbx file.\nThe program's default file location is in the source folder. Models should \nbe placed within the 'Models' folder within the source folder for consistency.");
	ImGui::Text("\nTo load a new level, place the level .json file within the \n'Leveldata' folder located in the project source folder. Then, input the \nlevel.json file's name into the field and press load. This will \nautomatically save the current level before loading the new one.");
	ImGui::Text("\nSaving the current level with a level name that does not currently exist in \nthe 'Leveldata' folder will create a NEW .json level file and save the \ncurrently loaded level into it.");
}

void Console::FeedbackHandler() {
	ImGui::Text("\n\n\nConsole Feedback:");
	
	switch (feedbackCode) {
	case 1:
		ImGui::Text("\nModel Imported Correctly.");
		break;
	case 2:
		ImGui::Text("\nLevel Loaded Correctly.");
		break;
	case 3:
		ImGui::Text("\nLevel Saved Correctly.");
		break;
	case 4:
		ImGui::Text("\nFailed to import model. Invalid file path.");
		break;
	case 5:
		ImGui::Text("\nFailed to load level. Invalid level name.");
		break;
	case 6:
		ImGui::Text("\nFailed to save level.");
		break;
	case 7:
		ImGui::Text("\nObject scale is 0. Object won't be visible.");
		break;
	}
}

void Console::ImportModel() {
	string path = convertToString(modelImport, (sizeof(modelImport) / sizeof(char)));

	std::ifstream ifs(std::experimental::filesystem::current_path().string() + "\\" + path.c_str());

	if (!ifs) {
		std::cout << "File does not exist" << std::endl;

		errorOccured = true;
		feedbackCode = 4;

		return;
	}

	Model m(path, glm::vec3(posx, posy, posz), glm::vec3(scalex, scaley, scalez));

	if (scalex == 0.0f && scaley == 0.0f && 0.0f) {
		errorOccured = true;
		feedbackCode = 7;
		
		return;
	}

	try
	{
		renderer->AddModel(m);
		renderer->LoadModels();

		actionCompleted = true;
		feedbackCode = 1;
	}
	catch (const std::exception&)
	{
		errorOccured = true;
		feedbackCode = 4;
	}
}

void Console::LoadLevel() {
	string path = convertToString(levelLoad, (sizeof(levelLoad) / sizeof(char)));

	if (!currentLevel.empty()) {
		LevelManager::SaveLevel(renderer, currentLevel);
	}
	
	try
	{
		LevelManager::LoadLevel(renderer, path);

		renderer->LoadModels();

		actionCompleted = true;
		feedbackCode = 2;

		currentLevel = path;
	}
	catch (const std::exception&)
	{
		errorOccured = true;
		feedbackCode = 5;
	}
}

void Console::SaveLevel() {
	string path = convertToString(levelLoad, (sizeof(levelLoad) / sizeof(char)));
	
	try
	{
		LevelManager::SaveLevel(renderer, levelLoad);

		actionCompleted = true;
		feedbackCode = 3;

		currentLevel = path;
	}
	catch (const std::exception&)
	{
		errorOccured = true;
		feedbackCode = 6;
	}
}

void Console::UpdateConsole() {
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	DisplayInputWindow();
	DisplayOutputWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	glfwSwapBuffers(renderer->window);
	glfwPollEvents();
}

void Console::ConsoleCleanup() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

string Console::convertToString(char* a, int size)
{
	int i;
	string s = "";
	for (i = 0; i < size; i++) {
		s = s + a[i];
	}
	return s;
}