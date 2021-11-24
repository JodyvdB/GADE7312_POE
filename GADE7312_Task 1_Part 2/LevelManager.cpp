#include "LevelManager.h"


void LevelManager::SaveLevel(Render* rIn, string path) {

	nlohmann::json save;

	for (int i = 0; i < rIn->models.size(); i++) {
		save["Models"][i] = rIn->models[i].GetJson();
	}
	for (int i = 0; i < rIn->spotLights.size(); i++) {
		save["Spots"][i] = rIn->spotLights[i].GetJson();
	}
	for (int i = 0; i < rIn->pointLights.size(); i++) {
		save["Points"][i] = rIn->pointLights[i].GetJson();
	}
		save["Directional"] = rIn->directionalLight.GetJson();



	std::ofstream ofs(std::experimental::filesystem::current_path().string() + "/LevelData/" + path.c_str() + ".json");

	ofs << save;

	ofs.close();

}

void LevelManager::LoadLevel(Render* rOut, string path) {
	std::ifstream ifs(std::experimental::filesystem::current_path().string()+"/LevelData/" + path.c_str()+".json");
	
	/*cout << std::experimental::filesystem::current_path().string() + "\\LevelData\\" + path.c_str()+".json" << endl;*/
	
	nlohmann::json levelData = nlohmann::json::parse(ifs);

	nlohmann::json models = levelData["Models"];
	nlohmann::json points = levelData["Points"];
	nlohmann::json dirs = levelData["Directional"];
	nlohmann::json spots = levelData["Spots"];

	rOut->ClearData();

	// init models
	for (int i = 0; i < models.size(); i++)
	{
		/*cout << models[i] << endl;*/
		glm::vec3 position(models[i]["pos"][0], models[i]["pos"][1], models[i]["pos"][2]);
		glm::vec3 scale(models[i]["scale"][0], models[i]["scale"][1], models[i]["scale"][2]);

		Model m(models[i].value("Path", "error"),position,scale);
		rOut->AddModel(m);
		//cout << level[i].value("Model", "No_String");
	}

	//init points
	for (int i = 0; i < points.size(); i++)
	{
		/*cout << points[i] << endl;*/
		Vector3 position(points[i]["pos"][0], points[i]["pos"][1], points[i]["pos"][2]);

		RGB ambient(points[i]["ambient"][0], points[i]["ambient"][1], points[i]["ambient"][2]);
		RGB diffuse(points[i]["diffuse"][0], points[i]["diffuse"][1], points[i]["diffuse"][2]);
		RGB specular(points[i]["specular"][0], points[i]["specular"][1], points[i]["specular"][2]);

		PointLight l(position, ambient, diffuse, specular, points[i].value("constant", 0), points[i].value("linear",0), points[i].value("quadratic", 0));
		rOut->AddPointLight(l);
		//cout << level[i].value("Model", "No_String");
	}
	//init Spots
	for (int i = 0; i < spots.size(); i++)
	{
		/*cout << spots[i] << endl;*/
		Vector3 position(spots[i]["pos"][0], spots[i]["pos"][1], spots[i]["pos"][2]);
		Vector3 direction(spots[i]["direction"][0], spots[i]["direction"][1], spots[i]["direction"][2]);

		RGB ambient(spots[i]["ambient"][0], spots[i]["ambient"][1], spots[i]["ambient"][2]);
		RGB diffuse(spots[i]["diffuse"][0], spots[i]["diffuse"][1], spots[i]["diffuse"][2]);
		RGB specular(spots[i]["specular"][0], spots[i]["specular"][1], spots[i]["specular"][2]);

		SpotLight l(position,direction, ambient, diffuse, specular, 
			spots[i].value("constant", 0), spots[i].value("linear",0), spots[i].value("quadratic", 0),
			spots[i].value("cutOff", 0), spots[i].value("outerCutOff", 0));
		rOut->AddSpotLight(l);
		//cout << level[i].value("Model", "No_String");
	}
	
	//init dir light
		/*cout << dirs << endl;*/
		Vector3 direction(dirs["direction"][0], dirs["direction"][1], dirs["direction"][2]);

		RGB ambient(dirs["ambient"][0], dirs["ambient"][1], dirs["ambient"][2]);
		RGB diffuse(dirs["diffuse"][0], dirs["diffuse"][1], dirs["diffuse"][2]);
		RGB specular(dirs["specular"][0], dirs["specular"][1], dirs["specular"][2]);

		DirectionalLight l(direction, ambient, diffuse, specular);
		rOut->directionalLight =l;
		//cout << level[i].value("Model", "No_String");
	
}
