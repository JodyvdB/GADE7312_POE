#pragma once

#include <json.hpp>;
#include <string>;
#include <iostream>;
#include <fstream>;
#include <vector>;
#include "Model.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>;

#include "Render.h";

using namespace std;

static class LevelManager
{

public: 
	static void SaveLevel(Render* rIn, string path);
	static void LoadLevel(Render* rOut, std::string path);
};

