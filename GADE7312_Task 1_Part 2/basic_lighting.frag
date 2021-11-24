#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 light1Pos; 
uniform vec3 light1Color;

uniform vec3 light2Pos; 
uniform vec3 light2Color;

uniform vec3 light3Pos; 
uniform vec3 light3Color;

uniform vec3 viewPos; 
uniform vec3 objectColor;

// function prototypes
vec3 CalcLightOne();
vec3 CalcLightTwo();
vec3 CalcLightThree();

void main()
{
	vec3 result = CalcLightOne() + CalcLightTwo() + CalcLightThree();

	FragColor = vec4(result, 1.0);
} 

vec3 CalcLightOne() {

	// Point Light 1
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * light1Color;
  	
    // diffuse 
    vec3 norm = normalize(-Normal);
    vec3 lightDir = normalize(light1Pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.5);
    vec3 diffuse = diff * light1Color;
    
    // specular
    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.1), 32);
    vec3 specular = specularStrength * spec * light1Color;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;

	return result;
}

vec3 CalcLightTwo() {

	// Point Light 1
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * light2Color;
  	
    // diffuse 
    vec3 norm = normalize(-Normal);
    vec3 lightDir = normalize(light2Pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.5);
    vec3 diffuse = diff * light2Color;
    
    // specular
    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.1), 32);
    vec3 specular = specularStrength * spec * light2Color;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;

	return result;
}

vec3 CalcLightThree() {

	// Point Light 1
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * light3Color;
  	
    // diffuse 
    vec3 norm = normalize(-Normal);
    vec3 lightDir = normalize(light3Pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.5);
    vec3 diffuse = diff * light3Color;
    
    // specular
    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.1), 32);
    vec3 specular = specularStrength * spec * light3Color;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;

	return result;
}