#include "stdafx.h"
#include "Trajectory.h"



Trajectory::Trajectory()
{
}


Trajectory::~Trajectory()
{
}

void Trajectory::Update(Vector3 *position, Vector3 *rotation, Vector3 *scale, Matrix mr, Matrix *placement, float deltaTime)
{
	if (trajectoryType == "linear" || trajectoryType == "line_strip" || trajectoryType == "line_loop")
	{
		Vector3 distance;
		int nextPoint;

		if (forward == true)
		{
			nextPoint = currentPoint + 1;
			distance = points[nextPoint] - points[currentPoint];
		}
		else
		{
			nextPoint = currentPoint - 1;
			distance = points[nextPoint] - points[currentPoint];
		}

		Vector3 move = distance.Normalize() * speed * deltaTime;
		*position += move;

		float pointDistance = (points[nextPoint].x - points[currentPoint].x) * (points[nextPoint].x - points[currentPoint].x);
		pointDistance += (points[nextPoint].y - points[currentPoint].y) * (points[nextPoint].y - points[currentPoint].y);
		pointDistance += (points[nextPoint].z - points[currentPoint].z) * (points[nextPoint].z - points[currentPoint].z);

		float positionDistance = (position->x - points[currentPoint].x) * (position->x - points[currentPoint].x);
		positionDistance += (position->y - points[currentPoint].y) * (position->y - points[currentPoint].y);
		positionDistance += (position->z - points[currentPoint].z) * (position->z - points[currentPoint].z);

		if (positionDistance > pointDistance)
		{
			if (iterationCount > 0)
			{
				iterationCount--;
			}

			*position = points[nextPoint];
			currentPoint = nextPoint;

			if (currentPoint == 0 || currentPoint == points.size() - 1)
			{
				if (direction == "normal")
				{
					currentPoint = 0;
					*position = points[currentPoint];
				}
				else
				{
					forward = !forward;
				}
			}
		}
	}
	else if (trajectoryType == "circle")
	{

		Matrix scenePosition, rotX, rotY, rotZ, scaleMatrix, transMatrix, radiusMatrix;

		rotX.SetRotationX(rotation->x);
		rotY.SetRotationY(rotation->y);
		rotZ.SetRotationZ(rotation->z);
		transMatrix.SetTranslation(position->x, position->y, position->z);
		radiusMatrix.SetTranslation(radius, radius, radius);
		scaleMatrix.SetScale(scale->x, scale->y, scale->z);


		alpha += speed * deltaTime;

		Matrix rotationMatrix;
		rotationMatrix.SetRotationAngleAxis(alpha, points[1].x, -points[1].y, points[1].z);

		*placement = scaleMatrix * rotX * rotY * rotZ * transMatrix * rotationMatrix * radiusMatrix  * mr ;


	}
}
