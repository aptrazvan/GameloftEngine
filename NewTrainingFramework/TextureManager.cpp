#include "stdafx.h"
#include "TextureManager.h"


TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

void TextureManager::Load()
{
	int width, height, bpp;
	char* arrayPixel;
	GLuint type, format;

	tr->path = "../" + tr->path;
	const char *path = tr->path.c_str();

	glGenTextures(1, &textureId);

	if (tr->type == "2d")
	{
		type = GL_TEXTURE_2D;
	}
	else if (tr->type == "cube")
	{
		type = GL_TEXTURE_CUBE_MAP;
	}

	glBindTexture(type, textureId);

	if (tr->minFilter == "LINEAR")
	{
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else if (tr->minFilter == "NEAREST")
	{
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	if (tr->magFilter == "LINEAR")
	{
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else if (tr->magFilter == "NEAREST")
	{
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	if (tr->wrapS == "CLAMP_TO_EDGE")
	{
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	}
	else if (tr->wrapS == "REPEAT")
	{
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	}

	if (tr->wrapT == "CLAMP_TO_EDGE")
	{
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else if (tr->wrapT == "REPEAT")
	{
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	glEnable(GL_DEPTH_TEST);

	arrayPixel = LoadTGA(path, &width, &height, &bpp);

	if (bpp == 24)
	{
		format = GL_RGB;
	}
	else
	{
		format = GL_RGBA;
	}

	if (tr->type == "2d")
	{		
		glTexImage2D(type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, arrayPixel);
	}
	else if (tr->type == "cube")
	{
		int NC = (width / 4) * (bpp / 8);
		char* buff = new char[NC * height / 3];
		int i, j;

		for (i = 0; i < height / 3; i++)
		{
			for (j = 0; j < NC; j++)
			{
				buff[i * NC + j] = arrayPixel[i * width * bpp / 8 + (j + NC)];
			}
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, (GLvoid*)buff);

		for (i = 0; i < height / 3; i++)
		{
			for (j = 0; j < NC; j++)
			{
				buff[i * NC + j] = arrayPixel[(i + height/3) * width * bpp / 8 + j];
			}
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, (GLvoid*)buff);

		for (i = 0; i < height / 3; i++)
		{
			for (j = 0; j < NC; j++)
			{
				buff[i * NC + j] = arrayPixel[(i + height/3) * width * bpp / 8 + (j + NC)];
			}
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, (GLvoid*)buff);

		for (i = 0; i < height / 3; i++)
		{
			for (j = 0; j < NC; j++)
			{
				buff[i * NC + j] = arrayPixel[(i + height/3) * width * bpp / 8 + (j + 2 * NC)];
			}
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, (GLvoid*)buff);

		for (i = 0; i < height / 3; i++)
		{
			for (j = 0; j < NC; j++)
			{
				buff[i * NC + j] = arrayPixel[(i + height/3) * width * bpp / 8 + (j + 3 * NC)];
			}
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, (GLvoid*)buff);

		for (i = 0; i < height / 3; i++)
		{
			for (j = 0; j < NC; j++)
			{
				buff[i * NC + j] = arrayPixel[(i + 2 * height/3) * width * bpp / 8 + (j + NC)];
			}
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, (GLvoid*)buff);
		
	}
	
	glBindTexture(type, 0);

	
}
