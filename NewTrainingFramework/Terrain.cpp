#include "stdafx.h"
#include "Terrain.h"


Terrain::Terrain()
{
}


Terrain::~Terrain()
{
}

void Terrain::GenerateModel(Vector3 position)
{
	Vector3 currentPos, pos, norm, binorm, tgt;
	Vector2 uv, uvBlend;

	center.x = position.x;
	center.y = offsetY;
	center.z = position.z;

	pos.y = center.y;
	long int a[100][100], k = 0;


	for (int i = 0; i < nrCells+1; i++)
	{
		currentPos.z = center.z + (nrCells / 2) * dimCells - i * dimCells;

		for (int j = 0; j < nrCells+1; j++)
		{
			currentPos.x = center.x - (nrCells / 2) * dimCells + j * dimCells;

			pos.x = currentPos.x;
			pos.z = currentPos.z;
			norm.x = 0;
			norm.y = 1;
			norm.z = 0;
			binorm.x = 0;
			binorm.y = 1;
			binorm.z = 0;
			tgt.x = 0;
			tgt.y = 1;
			tgt.z = 0;
			uv.x = i;
			uv.y = j;
			uvBlend.x = (float)i / (float)(nrCells);
			uvBlend.y = (float)j / (float)(nrCells);
			vertices.push_back(Vertex(pos, norm, binorm, tgt, uv, uvBlend));

			a[i][j] = k++;

		}
	}

	for (int i = 0; i < nrCells; i++)
	{
		for (int j = 0; j < nrCells; j++)
		{

			indices.push_back(a[i][j]);
			indices.push_back(a[i][j + 1]);
			indices.push_back(a[i + 1][j]);

			indices.push_back(a[i][j + 1]);
			indices.push_back(a[i + 1][j]);
			indices.push_back(a[i + 1][j + 1]);

		}
	}

	model = new ModelManager();
	model->nrVertices = vertices.size();
	model->nrIndices = indices.size();

	glGenBuffers(1, &model->verticesId);
	glBindBuffer(GL_ARRAY_BUFFER, model->verticesId);
	glBufferData(GL_ARRAY_BUFFER, model->nrVertices * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &model->indicesId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->indicesId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->nrIndices * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Terrain::Update(Vector3 position)
{
	int i;

	if (abs(position.x - center.x) > dimCells)
	{
		if (position.x > center.x)
		{
			for (i = 0; i < vertices.size(); i++)
			{
				vertices[i].pos.x += dimCells;
				vertices[i].uvBlend.y += 1 / (float)nrCells;
			}

			center.x += dimCells;
		}
		else
		{
			for (i = 0; i < vertices.size(); i++)
			{
				vertices[i].pos.x -= dimCells;
				vertices[i].uvBlend.y -= 1 / (float)nrCells;
			}

			center.x -= dimCells;
		}
	}

	if (abs(position.z - center.z) > dimCells)
	{
		if (position.z > center.z)
		{
			for (i = 0; i < vertices.size(); i++)
			{
				vertices[i].pos.z += dimCells;
				vertices[i].uvBlend.x -= 1 / (float)nrCells;
			}

			center.z += dimCells;
		}
		else
		{
			for (i = 0; i < vertices.size(); i++)
			{
				vertices[i].pos.z -= dimCells;
				vertices[i].uvBlend.x += 1 / (float)nrCells;
			}

			center.z -= dimCells;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, model->verticesId);
	glBufferData(GL_ARRAY_BUFFER, model->nrVertices * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

