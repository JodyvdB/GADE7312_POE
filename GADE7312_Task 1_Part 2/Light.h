#pragma once
#include "shader.h"
#include "json.hpp"
using namespace std;

struct RGB{
	float r, g, b;
	
	public: 
		RGB() {
			r = 0;
			g = 0;
			b = 0;
		}
		RGB(float _r, float _g, float _b) {
		r = _r;
		g = _g;
		b = _b;
	}
};

struct Vector3{
	float x, y, z;

	public:
		Vector3() {
			x = 0;
			y = 0;
			z = 0;
		}
		Vector3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
};

class Light
{
public:
	RGB ambient;
	RGB diffuse;
	RGB specular;
	
	Light();


	//Sets
	void SetAmbient(float r, float g, float b);
	void SetAmbient(RGB in);

	void SetDiffuse(float r, float g, float b);
	void SetDiffuse(RGB in);

	void SetSpecular(float r, float g, float b);
	void SetSpecular(RGB in);
};

class PointLight : public Light {
public:
	Vector3 position;
	float constant;
	float linear;
	float quadratic;

	PointLight(Vector3 pos, RGB ambient, RGB diffuse, RGB specular,
		float constant, float linear, float quadratic);

	void RenderLight(Shader shader, string name);

	//Sets
	void SetPosition(float x, float y, float z);
	void SetPosition(Vector3 in);

	void SetConstant(float c);
	void SetLinear(float l);
	void SetQuadratic(float q);

	nlohmann::json GetJson();
};
class DirectionalLight : public Light {
public:

	Vector3 direction;

	DirectionalLight() {

	}
	DirectionalLight(Vector3 dir, RGB ambient, RGB diffuse, RGB specular);

	void RenderLight(Shader shader);

	//Sets
	void SetDirection(float x, float y, float z);
	void SetDirection(Vector3 in);

	nlohmann::json GetJson();
};
class SpotLight : public Light {
public:

	SpotLight() {

	}

	Vector3 position;
	Vector3 direction;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;

	SpotLight(Vector3 pos, Vector3 dir, RGB ambient, RGB diffuse, RGB specular, 
		float constant, float linear, float quadratic, float cutOff, float outCutOff);

	void RenderLight(Shader shader, string name);

	//Sets
	void SetPosition(float x, float y, float z);
	void SetPosition(Vector3 v3);

	void SetDirection(float x, float y, float z);
	void SetDirection(Vector3 v3);

	void SetConstant(float c);
	void SetLinear(float l);
	void SetQuadratic(float q);
	void SetCutOff(float cut);
	void SetOuterCutOff(float outCut);

	nlohmann::json GetJson();
};


