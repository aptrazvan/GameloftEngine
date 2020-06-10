#include "stdafx.h"
#include "SceneManager.h"


SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
}


SceneManager* SceneManager::spInstance = NULL;
SceneManager* SceneManager::getInstance()
{
	if (!spInstance)
	{
		spInstance = new SceneManager();
	}
	return spInstance;
}
void SceneManager::metodaInstanta() {
	//nitel cod
}

void passValues(Terrain* target, ObjectScene object)
{
	target->id = object.id;
	target->modelId = object.modelId;
	target->shaderId = object.shaderId;
	target->type = object.type;
	target->blend = object.blend;
	target->name = object.name;
	target->textureIds = object.textureIds;
	target->position = object.position;
	target->rotation = object.rotation;
	target->scale = object.scale;
}

void SceneManager::Init()
{
	using namespace rapidxml;
	xml_document<> doc;
	ifstream file("../Resources/sceneManager.xml");
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	string content(buffer.str());
	doc.parse<0>(&content[0]);

	xml_node<> *pRoot = doc.first_node();

	xml_node<> *pNode = pRoot->first_node("backgroundColor")->first_node();
	bgColor.x = atof(pNode->value());

	pNode = pNode->next_sibling();
	bgColor.y = atof(pNode->value());

	pNode = pNode->next_sibling();
	bgColor.z = atof(pNode->value());

	for (xml_node<> *pNode = pRoot->first_node("cameras")->first_node("camera"); pNode; pNode = pNode->next_sibling("camera"))
	{
		CameraScene camera;

		xml_attribute<> *pAttr = pNode->first_attribute();
		camera.id = atoi(pAttr->value());

		xml_node<> *currentNode = pNode->first_node("position");
		xml_node<> *currentNodeVector = currentNode->first_node();
		camera.position.x = atof(currentNodeVector->value());

		currentNodeVector = currentNodeVector->next_sibling();
		camera.position.y = atof(currentNodeVector->value());

		currentNodeVector = currentNodeVector->next_sibling();
		camera.position.z = atof(currentNodeVector->value());

		currentNode = pNode->first_node("target");
		currentNodeVector = currentNode->first_node();
		camera.target.x = atof(currentNodeVector->value());

		currentNodeVector = currentNodeVector->next_sibling();
		camera.target.y = atof(currentNodeVector->value());

		currentNodeVector = currentNodeVector->next_sibling();
		camera.target.z = atof(currentNodeVector->value());

		currentNode = pNode->first_node("up");
		currentNodeVector = currentNode->first_node();
		camera.up.x = atof(currentNodeVector->value());

		currentNodeVector = currentNodeVector->next_sibling();
		camera.up.y = atof(currentNodeVector->value());

		currentNodeVector = currentNodeVector->next_sibling();
		camera.up.z = atof(currentNodeVector->value());

		currentNode = pNode->first_node("translationSpeed");
		camera.translationSpeed = atof(currentNode->value());

		currentNode = pNode->first_node("rotationSpeed");
		camera.rotationSpeed = atof(currentNode->value());

		currentNode = pNode->first_node("fov");
		camera.Fov = atof(currentNode->value());

		currentNode = pNode->first_node("near");
		camera.Near = atof(currentNode->value());

		currentNode = pNode->first_node("far");
		camera.Far = atof(currentNode->value());

		cameras.push_back(new Camera(camera.position, camera.target, camera.up, camera.translationSpeed, camera.rotationSpeed, camera.Fov, camera.Near, camera.Far));

	}

	if (pRoot->first_node("activeCamera"))
	{
		xml_node<> *pNode = pRoot->first_node("activeCamera");
		int activeCamera = atoi(pNode->value());

		cameras[activeCamera]->setActive(true);
	}

	for (xml_node<> *pNode = pRoot->first_node("objects")->first_node("object"); pNode; pNode = pNode->next_sibling("object"))
	{
		ObjectScene* object;
		vector<int> textureIds;

		xml_attribute<> *pAttr = pNode->first_attribute();

		xml_node<> *currentNode = pNode->first_node("type");
		string type = currentNode->value();

		if (type == "terrain")
		{
			Terrain* terrain = new Terrain();

			terrain->id = atoi(pAttr->value());

			currentNode = pNode->first_node("number");
			terrain->nrCells = atoi(currentNode->value());

			currentNode = pNode->first_node("dimension");
			terrain->dimCells = atof(currentNode->value());

			currentNode = pNode->first_node("offset");
			terrain->offsetY = atof(currentNode->value());

			currentNode = pNode->first_node("heights");
			xml_node<> *currentNodeVector = currentNode->first_node();
			terrain->heights.x = atof(currentNodeVector->value());

			currentNodeVector = currentNodeVector->next_sibling();
			terrain->heights.y = atof(currentNodeVector->value());

			currentNodeVector = currentNodeVector->next_sibling();
			terrain->heights.z = atof(currentNodeVector->value());

			objects.insert(pair<int, Terrain*>(terrain->id, terrain));

			object = (ObjectScene*)terrain;

		}
		else if (type == "skybox")
		{
			SkyBox* skybox = new SkyBox();

			skybox->id = atoi(pAttr->value());

			objects.insert(pair<int, SkyBox*>(skybox->id, skybox));

			object = (ObjectScene*)skybox;
		}
		else if (type == "fire")
		{
			Fire* fire = new Fire();

			fire->id = atoi(pAttr->value());

			currentNode = pNode->first_node("dispMax");
			fire->dispMax = atof(currentNode->value());

			objects.insert(pair<int, Fire*>(fire->id, fire));

			object = (ObjectScene*)fire;
		}
		else if (type == "normal")
		{
			object = new ObjectScene();
			object->id = atoi(pAttr->value());
			objects.insert(pair<int, ObjectScene*>(object->id, object));

		}

		object->type = type;
		object->trajectory = NULL;
		object->circleTrajectory = false;


		currentNode = pNode->first_node("model");
		object->modelId = atoi(currentNode->value());

		currentNode = pNode->first_node("shader");
		object->shaderId = atoi(currentNode->value());

		

		if (pNode->first_node("wired/"))
		{

		}

		currentNode = pNode->first_node("blend");
		object->blend = currentNode->value();

		currentNode = pNode->first_node("name");
		object->name = currentNode->value();

		if (pNode->first_node("color"))
		{
			currentNode = pNode->first_node("color");
			xml_node<> *currentNodeVector = currentNode->first_node();
			object->colors.x = atof(currentNodeVector->value());

			currentNodeVector = currentNodeVector->next_sibling();
			object->colors.y = atof(currentNodeVector->value());

			currentNodeVector = currentNodeVector->next_sibling();
			object->colors.z = atof(currentNodeVector->value());

		}

		currentNode = pNode->first_node("textures");
		for (xml_node<> *currentNodeVector = currentNode->first_node("texture"); currentNodeVector; currentNodeVector = currentNodeVector->next_sibling("texture"))
		{
			int textureId = atoi(currentNodeVector->first_attribute()->value());
			textureIds.push_back(textureId);
		}

		object->textureIds = textureIds;

		currentNode = pNode->first_node("position");
		xml_node<> *currentNodeVector = currentNode->first_node();
		object->position.x = atof(currentNodeVector->value());

		currentNodeVector = currentNodeVector->next_sibling();
		object->position.y = atof(currentNodeVector->value());

		currentNodeVector = currentNodeVector->next_sibling();
		object->position.z = atof(currentNodeVector->value());

		currentNode = pNode->first_node("rotation");
		currentNodeVector = currentNode->first_node();
		object->rotation.x = atof(currentNodeVector->value());

		currentNodeVector = currentNodeVector->next_sibling();
		object->rotation.y = atof(currentNodeVector->value());

		currentNodeVector = currentNodeVector->next_sibling();
		object->rotation.z = atof(currentNodeVector->value());

		currentNode = pNode->first_node("scale");
		currentNodeVector = currentNode->first_node();
		object->scale.x = atof(currentNodeVector->value());

		currentNodeVector = currentNodeVector->next_sibling();
		object->scale.y = atof(currentNodeVector->value());

		currentNodeVector = currentNodeVector->next_sibling();
		object->scale.z = atof(currentNodeVector->value());

		if (pNode->first_node("followingCamera"))
		{
			currentNode = pNode->first_node("followingCamera");

			if (currentNode->first_node("ox"))
			{
				object->followingCamera.x = 1;
				object->offset.x = object->position.x;
			}

			if (currentNode->first_node("oy"))
			{
				object->followingCamera.y = 1;
				object->offset.y = object->position.y;
			}

			if (currentNode->first_node("oz"))
			{
				object->followingCamera.z = 1;
				object->offset.z = object->position.z;
			}
		}

		

		if (pNode->first_node("trajectory"))
		{
			object->trajectory = new Trajectory();
			object->trajectory->iterationCount = 0;

			currentNode = pNode->first_node("trajectory");

			if ((string)(currentNode->first_attribute("iteration-count")->value()) != "infinite")
			{
				object->trajectory->iterationCount = atoi(currentNode->first_attribute("iteration-count")->value());
			}
			else
			{
				object->trajectory->iterationCount = -1;
			}

			if (currentNode->first_attribute("direction")->value())
			{
				object->trajectory->direction = currentNode->first_attribute("direction")->value();
			}

			object->trajectory->trajectoryType = currentNode->first_attribute("type")->value();
			object->trajectory->speed = atof(currentNode->first_attribute("speed")->value());
			object->trajectory->forward = true;
			object->trajectory->currentPoint = 0;

			if (object->trajectory->trajectoryType != "circle")
			{
				currentNode = currentNode->first_node("points");

				for (xml_node<> *currentNodeVector = currentNode->first_node("point"); currentNodeVector; currentNodeVector = currentNodeVector->next_sibling("point"))
				{
					Vector3 point;
					xml_node<> *currentNodeVectorCoord = currentNodeVector->first_node();
					point.x = atof(currentNodeVectorCoord->value());

					currentNodeVectorCoord = currentNodeVectorCoord->next_sibling();
					point.y = atof(currentNodeVectorCoord->value());

					currentNodeVectorCoord = currentNodeVectorCoord->next_sibling();
					point.z = atof(currentNodeVectorCoord->value());

					object->trajectory->points.push_back(point);
				}
			}
			else
			{
				Vector3 center;

				currentNodeVector = currentNode->first_node("center");

				currentNodeVector = currentNodeVector->first_node();
				center.x = atof(currentNodeVector->value());

				currentNodeVector = currentNodeVector->next_sibling();
				center.y = atof(currentNodeVector->value());

				currentNodeVector = currentNodeVector->next_sibling();
				center.z = atof(currentNodeVector->value());

				object->trajectory->points.push_back(center);

				currentNodeVector = currentNode->first_node("radius");
				object->trajectory->radius = atof(currentNodeVector->value());

				Vector3 vector;

				currentNode = currentNode->first_node("rotationPlane");

				currentNodeVector = currentNode->first_node();
				vector.x = atof(currentNodeVector->value());

				currentNodeVector = currentNodeVector->next_sibling();
				vector.y = atof(currentNodeVector->value());

				currentNodeVector = currentNodeVector->next_sibling();
				vector.z = atof(currentNodeVector->value());

				object->trajectory->points.push_back(vector);

				object->trajectory->offsetPosition = object->position;

				object->trajectory->alpha = 0;

				object->circleTrajectory = true;

			}
			

			object->trajectory->lastPosition = object->trajectory->points[0];

		}


	}
	
	pNode = pRoot->first_node("fog");

	xml_node<> *currentNode = pNode->first_node("lowRadius");
	fogLowRadius = atof(currentNode->value());

	currentNode = pNode->first_node("highRadius");
	fogHighRadius = atof(currentNode->value());

	currentNode = pNode->first_node("color");
	xml_node<> *currentNodeVector = currentNode->first_node();
	fogColor.x = atof(currentNodeVector->value());

	currentNodeVector = currentNodeVector->next_sibling();
	fogColor.y = atof(currentNodeVector->value());

	currentNodeVector = currentNodeVector->next_sibling();
	fogColor.z = atof(currentNodeVector->value());

	activeCamera = atoi(pRoot->first_node("activeCamera")->value());

}

void SceneManager::Draw(ESContext *esContext, Matrix mr, Vector3 position)
{
	
	for (map<int, ObjectScene*>::iterator it = objects.begin(); it != objects.end(); it++) {

		it->second->Draw(mr, position);
	}
	
}

void SceneManager::Update(Vector3 position, float deltaTime)
{
	for (map<int, ObjectScene*>::iterator it = objects.begin(); it != objects.end(); it++) {
		if (it->second->type == "terrain")
		{
			((Terrain*)(it->second))->Update(position);
		}
		else if (it->second->type == "fire")
		{
			((Fire*)(it->second))->Update();
		}
		it->second->Update(position, deltaTime);
	}
}

Camera* SceneManager::getActiveCamera()
{
	for (int i = 0; i < cameras.size(); i++)
	{
		if (cameras[i]->getActive() == true)
		{
			return cameras[i];
		}
	}
}

void SceneManager::setActiveCamera(int camera)
{
	if (camera > (int)cameras.size())
	{
		return;
	}

	for (int i = 0; i < cameras.size(); i++)
	{
		if (cameras[i]->getActive() == true)
		{
			cameras[i]->setActive(false);

			if (camera == -2)
			{
				if (i == 0)
				{
					cameras[cameras.size() - 1]->setActive(true);
				}
				else
				{
					cameras[i - 1]->setActive(true);
				}
			}
			else if (camera == -1)
			{
				if (i == cameras.size() - 1)
				{
					cameras[0]->setActive(true);
				}
				else
				{
					cameras[i + 1]->setActive(true);
				}
			}
			else
			{
				cameras[camera]->setActive(true);
			}

			return;
		}
		
	}

}
