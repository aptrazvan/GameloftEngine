#include "stdafx.h"
#include "Camera.h"

Vector3 convertVector(Vector4 v)
{
	Vector3 res;
	res.x = v.x;
	res.y = v.y;
	res.z = v.z;

	return res;
}
Camera::Camera()
{
	position = Vector3((GLfloat)0, (GLfloat)0, (GLfloat)0.5);
	target = Vector3((GLfloat)0, (GLfloat)0, (GLfloat)0);
	up = Vector3((GLfloat)0, (GLfloat)1, (GLfloat)0);
	xAxis = Vector3((GLfloat)1, (GLfloat)0, (GLfloat)0);
	yAxis = Vector3((GLfloat)0, (GLfloat)1, (GLfloat)0);
	zAxis = Vector3((GLfloat)0, (GLfloat)0, (GLfloat)1);
	viewMatrix.SetIdentity();
	worldMatrix.SetIdentity();
	moveSpeed = 15;
	rotateSpeed = 3;
	Near = 0.2f;
	Far = 5000;
	fov = 45;
	deltaTime = 0.5f;

	perspectiveMatrix.SetPerspective(fov, 16.0f/9.0f, Near, Far);

	setMatrix();

}

Camera::Camera(Vector3 position, Vector3 target)
{
	this->position = position;
	this->target = target;
	moveSpeed = 0;
	rotateSpeed = 0;
	Near = 0;
	Far = 0;
	fov = 0;

}

Camera::Camera(Vector3 position, Vector3 target, Vector3 up, GLfloat translationSpeed, GLfloat rotationSpeed, GLfloat Fov, GLfloat Near, GLfloat Far)
{
	this->position = position;
	this->target = target;
	this->up = up;
	xAxis = Vector3((GLfloat)1, (GLfloat)0, (GLfloat)0);
	yAxis = Vector3((GLfloat)0, (GLfloat)1, (GLfloat)0);
	zAxis = Vector3((GLfloat)0, (GLfloat)0, (GLfloat)1);
	viewMatrix.SetIdentity();
	worldMatrix.SetIdentity();
	moveSpeed = translationSpeed;
	rotateSpeed = rotationSpeed;
	this->Near = Near;
	this->Far = Far;
	fov = Fov;
	deltaTime = 0.5f;
	active = false;

	perspectiveMatrix.SetPerspective(fov, 16.0f / 9.0f, Near, Far);

	setMatrix();

}

void Camera::updateDeltaTime(float deltaTime)
{
	this->deltaTime = deltaTime;
}


void Camera::setAxis()
{
	zAxis = -(target - position).Normalize();
	yAxis = up.Normalize();
	xAxis = -zAxis.Cross(yAxis).Normalize();

}

void Camera::moveOx(GLfloat directie)
{
	Vector3 horizontalMove = zAxis.Cross(yAxis).Normalize() * moveSpeed * directie;
	Vector3 vectorDeplasare = horizontalMove * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;

	setMatrix();
}

void Camera::moveOy(GLfloat directie)
{
	Vector3 upMove = up.Normalize() * moveSpeed * directie;
	Vector3 vectorDeplasare = upMove * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;

	setMatrix();
}

void Camera::moveOz(GLfloat directie)
{
	Vector3 forward = -(target - position).Normalize() * moveSpeed * directie;
	Vector3 vectorDeplasare = forward * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;

	setMatrix();
}

void Camera::rotateOx(GLfloat directie)
{
	GLfloat alpha = rotateSpeed * deltaTime * directie;
	Matrix mRotateOx;
	mRotateOx.SetRotationX(alpha);
	Vector4 rotatedLocalUp = Vector4(0.0f, 1.0f, 0.0f, 0.0f) * mRotateOx;
	up = convertVector(rotatedLocalUp * worldMatrix);
	up = up.Normalize();
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * mRotateOx;
	target = convertVector(rotatedTarget * worldMatrix);

	setMatrix();
	
}

void Camera::rotateOy(GLfloat directie)
{
	GLfloat alpha = rotateSpeed * deltaTime * directie;
	Matrix mRotateOy;
	mRotateOy.SetRotationY(alpha);
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * mRotateOy;
	target = convertVector(rotatedTarget * worldMatrix);

	setMatrix();

}

void Camera::rotateOz(GLfloat directie)
{
	GLfloat alpha = rotateSpeed * deltaTime * directie;
	Matrix mRotateOz;
	mRotateOz.SetRotationZ(alpha);
	Vector4 localTarget = Vector4(0.0f, 1.0f, 0.0f, 0.0f) * mRotateOz;
	up = convertVector(localTarget * worldMatrix).Normalize();


	setMatrix();
	
}

void Camera::setMatrix()
{
	Matrix R, T, invR, invT;

	setAxis();

	R.m[0][0] = xAxis.x;	R.m[0][1] = xAxis.y;	R.m[0][2] = xAxis.z;	R.m[0][3] = 0;
	R.m[1][0] = yAxis.x;	R.m[1][1] = yAxis.y;	R.m[1][2] = yAxis.z;	R.m[1][3] = 0;
	R.m[2][0] = zAxis.x;	R.m[2][1] = zAxis.y;	R.m[2][2] = zAxis.z;	R.m[2][3] = 0;
	R.m[3][0] = 0;			R.m[3][1] = 0;			R.m[3][2] = 0;			R.m[3][3] = 1;

	invR = R.Transpose();

	T.SetTranslation(position);
	invT.SetTranslation(-position);

	worldMatrix = R * T;
	viewMatrix = invT * invR;


}

bool Camera::getActive()
{
	return active;
}

void Camera::setActive(bool active)
{
	this->active = active;
}

Camera::~Camera()
{
}
