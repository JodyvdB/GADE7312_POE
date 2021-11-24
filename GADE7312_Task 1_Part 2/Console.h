#pragma once

#include <json.hpp>;
#include <string>;

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>;

#include "Render.h";
#include "LevelManager.h"

using namespace std;

static class Console {

public:
	Render* renderer;
	string currentLevel;
public:
	Console(Render* render);
	void DisplayInputWindow();
	void DisplayOutputWindow();
	void ShowFPS();
	void ShowTriangleCount();
	void Help();
	void FeedbackHandler();
	void ImportModel();
	void LoadLevel();
	void SaveLevel();
	void UpdateConsole();
	void ConsoleCleanup();
	string convertToString(char* a, int size);
};