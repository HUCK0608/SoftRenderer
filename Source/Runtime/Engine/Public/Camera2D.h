#pragma once

#include "GameObject2D.h"

class Camera2D
{
public:
	Camera2D() {}

	GameObject2D& GetGameObject();
	Matrix3x3 GetViewMatrix();

private:
	GameObject2D GameObject;
};