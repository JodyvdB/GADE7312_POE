#include "texture.h"

#include <iostream>

//Texture::Texture() {}

Texture::Texture(std::string dir, std::string path, aiTextureType type)
	: dir(dir), path(path), type(type) {
	generate();
}

void Texture::generate()
{
	glGenTextures(1, &id);
}

void Texture::load(bool flip)
{
	stbi_set_flip_vertically_on_load(flip);

	int width, height, nChannels;

	unsigned char* data = stbi_load((dir + "/" + path).c_str(), &width, &height, &nChannels, 0);

	GLenum colourMode = GL_RGB;
	switch (nChannels)
	{
	case 1:
		colourMode = GL_RED;
		break;
	case 4:
		colourMode = GL_RGBA;
		break;
	}

	if (data)
	{
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, colourMode, width, height, 0, colourMode, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		//setting texture filters and wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Image not loaded at: " << path << std::endl;
	}

	stbi_image_free(data);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}
