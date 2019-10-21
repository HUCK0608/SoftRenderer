#include "Precompiled.h"
#include "Camera2D.h"

GameObject2D & Camera2D::GetGameObject()
{
	return GameObject;
}

Matrix3x3 Camera2D::GetViewMatrix()
{
	Matrix3x3 tMat = GameObject.GetTransform().GetTMatrix();
	tMat[2][0] = -tMat[2][0];
	tMat[2][1] = -tMat[2][1];

	Matrix3x3 rMat = GameObject.GetTransform().GetRMatrix();
	rMat = rMat.Tranpose();

	return rMat * tMat;
}
