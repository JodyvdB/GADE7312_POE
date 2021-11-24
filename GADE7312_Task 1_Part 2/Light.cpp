#include "Light.h"

using namespace std;

void PointLight::RenderLight(Shader shader, string name)
{
	shader.setVec3("pointLights[" + name + "].position", position.x, position.y, position.z);
	shader.setVec3("pointLights[" + name + "].ambient", ambient.r, ambient.g, ambient.b);
	shader.setVec3("pointLights[" + name + "].diffuse", diffuse.r, diffuse.g, diffuse.b);
	shader.setVec3("pointLights[" + name + "].specular", specular.r, specular.g, specular.b);

	shader.setFloat("pointLights[" + name + "].k0", constant);
	shader.setFloat("pointLights[" + name + "].k1", linear);
	shader.setFloat("pointLights[" + name + "].k2", quadratic);
	
	//cout << "Loaded point" + name + " Light" << endl;
}

void DirectionalLight::RenderLight(Shader shader)
{
	shader.setVec3("dirLight.direction", direction.x, direction.y, direction.z);

	shader.setVec3("dirLight.ambient", ambient.r,ambient.g,ambient.b);
	shader.setVec3("dirLight.diffuse", diffuse.r,diffuse.g,diffuse.b);
	shader.setVec3("dirLight.specular", specular.r,specular.g,specular.b);

	/*cout << "Loaded Dir Light" << endl;*/
}

void SpotLight::RenderLight(Shader shader, string name)
{
	shader.setVec3("spotLights[" + name + "].ambient", ambient.r, ambient.g, ambient.b);
	shader.setVec3("spotLights[" + name + "].diffuse", diffuse.r, diffuse.g, diffuse.b);
	shader.setVec3("spotLights[" + name + "].specular", specular.r, specular.g, specular.b);

	shader.setVec3("spotLights[" + name + "].position", position.x,position.y,position.z);
	shader.setVec3("spotLights[" + name + "].direction", direction.x, direction.y, direction.z);

	shader.setFloat("spotLights[" + name + "].k0", constant);
	shader.setFloat("spotLights[" + name + "].k1", linear);
	shader.setFloat("spotLights[" + name + "].k2", quadratic);
	shader.setFloat("spotLights[" + name + "].cutOff", glm::cos(glm::radians(cutOff)));
	shader.setFloat("spotLights[" + name + "].outerCutOff", glm::cos(glm::radians(outerCutOff)));

	//cout << "Loaded spot "+ name +" Light" << endl;
}


Light::Light() {
	SetAmbient(0.0f, 0.0f, 0.0f);
	SetDiffuse(0.0f, 0.0f, 0.0f);
	SetSpecular(0.0f, 0.0f, 0.0f);
}
// Set Methods for Base Class....
void Light::SetAmbient(float r, float g, float b) {
	RGB in(r, g, b);
	SetAmbient(in);
}
void Light::SetAmbient(RGB in) {
	ambient = in;
}

void Light::SetDiffuse(float r, float g, float b) {
	RGB in(r, g, b);
	SetDiffuse(in);
}
void Light::SetDiffuse(RGB in) {
	diffuse = in;
}

void Light::SetSpecular(float r, float g, float b) {
	RGB in(r, g, b);
	SetSpecular(in);
}
void Light::SetSpecular(RGB in) {
	specular = in;
}

// Methods for PointLight...
PointLight::PointLight(Vector3 pos, RGB ambient, RGB diffuse, RGB specular,
	float constant, float linear, float quadratic) {
	SetPosition(pos);

	SetAmbient(ambient);
	SetDiffuse(diffuse);
	SetSpecular(specular);

	SetConstant(constant);
	SetLinear(linear);
	SetQuadratic(quadratic);
}
//Sets
void PointLight::SetPosition(float x, float y, float z) {
	Vector3 in(x, y, z);
	SetPosition(in);
}
void PointLight::SetPosition(Vector3 in) {
	position = in;
}

void PointLight::SetConstant(float c) {
	constant = c;
}
void PointLight::SetLinear(float l) {
	linear = l;
}
void PointLight::SetQuadratic(float q) {
	quadratic = q;
}

nlohmann::json PointLight::GetJson() {

	nlohmann::json j = {
		{"pos",{position.x,position.y,position.z}},
		{"ambient",{ambient.r,ambient.g,ambient.b}},
		{"diffuse",{diffuse.r,diffuse.g,diffuse.b}},
		{"specular",{specular.r,specular.g,specular.b}},
		{"constant",constant},
		{"linear",linear},
		{"quadratic",quadratic}
	};
	return j;
}

// Methods for DirectionalLight...
DirectionalLight::DirectionalLight(Vector3 dir, RGB ambient, RGB diffuse, RGB specular) {
	SetDirection(dir);

	SetAmbient(ambient);
	SetDiffuse(diffuse);
	SetSpecular(specular);
}
// Sets
void DirectionalLight::SetDirection(float x, float y, float z) {
	Vector3 in(x, y, z);
	SetDirection(in);
}
void DirectionalLight::SetDirection(Vector3 in) {
	direction = in;
}

nlohmann::json DirectionalLight::GetJson() {

	nlohmann::json j = {
		{"direction",{direction.x,direction.y,direction.z}},
		{"ambient",{ambient.r,ambient.g,ambient.b}},
		{"diffuse",{diffuse.r,diffuse.g,diffuse.b}},
		{"specular",{specular.r,specular.g,specular.b}}
	};
	return j;
}

// Methods for SpotLight...
SpotLight::SpotLight(Vector3 pos, Vector3 dir, RGB ambient, RGB diffuse, RGB specular,
	float constant, float linear, float quadratic, float cutOff, float outCutOff) {
	SetPosition(pos);
	SetDirection(dir);
	
	SetAmbient(ambient);
	SetDiffuse(diffuse);
	SetSpecular(specular);

	SetConstant(constant);
	SetLinear(linear);
	SetQuadratic(quadratic);
	
	SetCutOff(cutOff);
	SetOuterCutOff(outCutOff);
}
// Sets
void SpotLight::SetPosition(float x, float y, float z) {
	Vector3 in(x, y, z);
	SetPosition(in);
}
void SpotLight::SetPosition(Vector3 in) {
	position = in;
}

void SpotLight::SetDirection(float x, float y, float z) {
	Vector3 in(x, y, z);
	SetDirection(in);
}
void SpotLight::SetDirection(Vector3 in) {
	direction = in;
}

void SpotLight::SetConstant(float c) {
	constant = c;
}
void SpotLight::SetLinear(float l) {
	linear = l;
}
void SpotLight::SetQuadratic(float q) {
	quadratic = q;
}
void SpotLight::SetCutOff(float cut) {
	cutOff = cut;
}
void SpotLight::SetOuterCutOff(float outCut) {
	outerCutOff = outCut;
}

nlohmann::json SpotLight::GetJson() {

	nlohmann::json j = {
		{"pos",{position.x,position.y,position.z}},
		{"direction",{direction.x,direction.y,direction.z}},
		{"ambient",{ambient.r,ambient.g,ambient.b}},
		{"diffuse",{diffuse.r,diffuse.g,diffuse.b}},
		{"specular",{specular.r,specular.g,specular.b}},
		{"constant",constant},
		{"linear",linear},
		{"quadratic",quadratic},
		{"cutOff",cutOff},
		{"outerCutOff",outerCutOff}
		};
	return j;
}

