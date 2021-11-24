#include "mesh.h"
#include <iostream>

std::vector<Vertex> Vertex::genList(float* vertices, int noVertices)
{
	std::vector<Vertex> ret(noVertices);

	int stride = sizeof(Vertex) / sizeof(float);

	for (int i = 0; i < noVertices; i++)
	{
		ret[i].pos = glm::vec3(
			vertices[i * stride + 0],
			vertices[i * stride + 1],
			vertices[i * stride + 2]
		);

		ret[i].normal = glm::vec3(
			vertices[i * stride + 3],
			vertices[i * stride + 4],
			vertices[i * stride + 5]
		);

		ret[i].texCoord = glm::vec2(
			vertices[i * stride + 6],
			vertices[i * stride + 7]
		);
	}

	return ret;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	: vertices(vertices), indices(indices), textures(textures) {
	setup();
}

void Mesh::render(Shader shader)
{
	//textures
	unsigned int diffuseIdx = 0;
	unsigned int normalIdx = 0;

	//shader.setInt("material.diffuse", 0);
	//shader.setInt("material.normal", 0);
	/*std::cout << "Mesh Verts: "<< vertices.size() <<"||  Textures:" <<textures.size()<< std::endl;*/

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		/*std::cout << textures[i].path << std::endl;*/

		//activate texture
		glActiveTexture(GL_TEXTURE0 + i);

		//get texture info
		std::string number;
		std::string name;

		/*std::cout << "grabbing texture Info" << std::endl;*/
		switch (textures[i].type)
		{
		case aiTextureType_DIFFUSE:
			/*std::cout << "diffuse" << std::endl;*/
			number = std::to_string(diffuseIdx++);
			name = "diffuse" + std::to_string(diffuseIdx++);
			shader.setInt("material.diffuse", i);
			break;
		case aiTextureType_HEIGHT:
			/*std::cout << "normal" << std::endl;*/
			number = std::to_string(normalIdx++);
			name = "normal" + std::to_string(normalIdx++);
			shader.setInt("material.normal", i);
			break;
		}

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);

		//set shader value
		//shader.setInt(name, i);
		//bind texture
		//textures[i].bind();
	}
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//glActiveTexture(GL_TEXTURE0);
}

void Mesh::cleanup()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::setup()
{
	//create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//load data into VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	//load data into EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//set vertex attribute pointers
	//vertex.position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//vertex.normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
	//vertex.texCoord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoord)));

	glBindVertexArray(0);
}
