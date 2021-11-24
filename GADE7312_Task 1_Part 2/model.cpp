#include "model.h"
#include <iostream>
#define Debug(x) std::cout << x << std::endl;

Model::Model(std::string path, glm::vec3 pos, glm::vec3 size)
	: pos(pos), size(size) {
	mPath = path;
}

void Model::render(Shader shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::scale(model, size);
	//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//std::cout << "pos: " << size << std::endl;
	shader.setMat4("model", model);
	shader.setFloat("material.shininess", 0.5f);
	shader.setFloat("material.ambient", 0.8f);

	std::cout << "Loading " + std::to_string(meshes.size()) + " meshes..." << std::endl;
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].render(shader);
		std::cout << "Loading Mesh: " + std::to_string(i) << std::endl;
	}
}

int Model::GetTriangles() {
	int triangleCount = 0;

	for (Mesh m : meshes)
	{
		triangleCount += m.indices.size() / 3;
	}

	return triangleCount;
}

void Model::cleanup()
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].cleanup();
	}
}

void Model::loadModel(std::string path)
{
	
	Debug("Loading model:" << path);
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	//check if file not read properly
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Debug("Couldn't load model from " << path << std::endl << import.GetErrorString());

		return;
	}

	directory = path.substr(0, path.find_last_of("/"));
	Debug("Root Directory of Active Model:" << directory);
	processNode(scene->mRootNode, scene);
		
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	Debug(node->mNumMeshes);
	Debug( (*node).mName.data);
	//process all meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	std::cout << "Post Load Mesh count:" + std::to_string(meshes.size())<< std::endl;

	//process the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	//vertices, indices, and textures - these 3 are used to create a mesh
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	//go through each vertex, process coords and values
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		//pos
		vertex.pos = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z);
		//normals
		vertex.normal = glm::vec3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z);
		//textures
		if (mesh->mTextureCoords[0])
		{
			vertex.texCoord = glm::vec2(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y);
		}
		else
		{
			vertex.texCoord = glm::vec2(0.0f);
		}
		//push to list
		vertices.push_back(vertex);
	}
	//process indices - load in face
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		//load in diffuse and bump/normal maps
		//DIFFUSE
		std::vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		//NORMAL
		std::vector<Texture> normalMaps = loadTextures(material, aiTextureType_HEIGHT);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadTextures(aiMaterial* mat, aiTextureType type)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::cout << str.C_Str() << std::endl;

		//prevent duplicate loading
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		//texture hasn't been loaded yet
		if (!skip)
		{
			std::cout << "loading " << directory << std::endl;
			Texture tex(directory, str.C_Str(), type);
			tex.load(false);
			textures.push_back(tex);
			textures_loaded.push_back(tex);
		}

	}

	return textures;
}

nlohmann::json Model::GetJson() {

	nlohmann::json j= {
		{"pos",{pos.x,pos.y,pos.z}},
		{"scale",{size.x,size.y,size.z}},
		{"Path",mPath}
	};
	return j;
}
