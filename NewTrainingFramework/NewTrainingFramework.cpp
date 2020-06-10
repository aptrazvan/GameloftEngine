// NewTrainingFramework.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "../rapidxml-1.13/rapidxml_utils.hpp"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Globals.h"
#include "Camera.h"
#include "Model.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;


GLuint vboId, verticesId, indicesId, textureId;
Shaders myShaders;
GLfloat alpha = 0, pas = 0.01f;
Camera camera;
Camera camera2;


int nrIndices, width, height, bpp;
char* arrayPixel;
ResourceManager* resourceManager;
SceneManager* sceneManager;


int Init ( ESContext *esContext )
{
	resourceManager = ResourceManager::getInstance();
	resourceManager->Init();

	sceneManager = SceneManager::getInstance();
	sceneManager->Init();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (map<int, ObjectScene*>::iterator it = sceneManager->objects.begin(); it != sceneManager->objects.end(); it++) {
		
		if (it->second->type == "terrain") {
			Terrain* terrain = (Terrain*)it->second;
			terrain->GenerateModel(camera.getPosition());
		}
		else {
			it->second->model = resourceManager->LoadModel(it->second->modelId);
		}

		for (int i = 0; i < it->second->textureIds.size(); i++)
		{
			TextureManager* texture = resourceManager->LoadTexture(it->second->textureIds[i]);
			it->second->texture.push_back(texture);
		}

		it->second->shader = resourceManager->LoadShader(it->second->shaderId);
		
	}

	//creation of shaders and program */
	return myShaders.Init("../Resources/Shaders/ModelShaderVS.vs", "../Resources/Shaders/ModelShaderFS.fs");

}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(myShaders.program);
	
	Matrix mr;
	mr = sceneManager->getActiveCamera()->getViewMatrix() * sceneManager->getActiveCamera()->getPerspectiveMatrix();

	sceneManager->Draw(esContext, mr, sceneManager->getActiveCamera()->getPosition());


	if (myShaders.matrixUniform != -1)
	{
		glUniformMatrix4fv(myShaders.matrixUniform, 1, GL_FALSE, (GLfloat*)mr.m);
	}

	
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update ( ESContext *esContext, float deltaTime )
{

	sceneManager->getActiveCamera()->updateDeltaTime(deltaTime);
	sceneManager->Update(sceneManager->getActiveCamera()->getPosition(), deltaTime);

	alpha += pas;

	if (alpha > 2 * PI)
	{
		alpha -= 2*PI;
	}
}

char prevKey = -1;
bool resetKey = false;
short int stepCounter = 18;

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{	

	switch (key)
	{
		case('W'):
			{
				sceneManager->getActiveCamera()->moveOz(-1);

				if (stepCounter == 18)
				{
					stepCounter = 0;
					resourceManager->PlaySound(resourceManager->LoadSound(1)->soundId);
				}
				else
				{
					stepCounter++;
				}

				break;
			}
		case('S'):
			{
				sceneManager->getActiveCamera()->moveOz(1);

				if (stepCounter == 18)
				{
					stepCounter = 0;
					resourceManager->PlaySound(resourceManager->LoadSound(1)->soundId);
				}
				else
				{
					stepCounter++;
				}

				break;
			}
		case('A'):
		{
			sceneManager->getActiveCamera()->moveOx(1);

			if (stepCounter == 18)
			{
				stepCounter = 0;
				resourceManager->PlaySound(resourceManager->LoadSound(1)->soundId);
			}
			else
			{
				stepCounter++;
			}

			break;
		}

		case('d'):
		case('D'):
		{
			sceneManager->getActiveCamera()->moveOx(-1);

			if (stepCounter == 18)
			{
				stepCounter = 0;
				resourceManager->PlaySound(resourceManager->LoadSound(1)->soundId);
			}
			else
			{
				stepCounter++;
			}

			break;
		}

		case('Q'):
		{
			sceneManager->getActiveCamera()->moveOy(-1);
			break;
		}

		case('E'):
		{
			sceneManager->getActiveCamera()->moveOy(1);
			break;
		}

		case(VK_RIGHT):
		{
			sceneManager->getActiveCamera()->rotateOy(1);
			break;
		}
		case(VK_LEFT):
		{
			sceneManager->getActiveCamera()->rotateOy(-1);
			break;
		}
		case(VK_UP):
		{
			sceneManager->getActiveCamera()->rotateOx(-1);
			break;
		}
		case(VK_DOWN):
		{
			sceneManager->getActiveCamera()->rotateOx(1);
			break;
		}
		case('L'):
		{
			sceneManager->getActiveCamera()->rotateOz(-1);
			break;
		}
		case('P'):
		{
			sceneManager->getActiveCamera()->rotateOz(1);
			break;
		}
		case('Z'):
		{
			if (prevKey == false)
			{
				prevKey = true;
				break;
			}
			prevKey = false;
			sceneManager->setActiveCamera(-2);
			break;
		}
		case('X'):
		{
			if (prevKey == false)
			{
				prevKey = true;
				break;
			}
			prevKey = false;
			sceneManager->setActiveCamera(-1);
			break;
		}
		case('1'):
		{
			sceneManager->setActiveCamera(0);
			break;
		}
		case('2'):
		{
			sceneManager->setActiveCamera(1);
			break;
		}
		case('3'):
		{
			sceneManager->setActiveCamera(2);
			break;
		}
		case('4'):
		{
			sceneManager->setActiveCamera(3);
			break;
		}
		case('5'):
		{
			sceneManager->setActiveCamera(4);
			break;
		}

	}


}

void CleanUp()
{
	glDeleteBuffers(1, &vboId);
}


int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();

	
	printf("Press any key...\n");
	_getch();

	
	return 0;
}

