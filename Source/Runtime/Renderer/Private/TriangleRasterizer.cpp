#include "Precompiled.h"
#include "TriangleRasterizer.h"

TriangleRasterizer::TriangleRasterizer()
{
	RecalcBounds();
}

TriangleRasterizer::TriangleRasterizer(Vertex InVertex0, Vertex InVertex1, Vertex InVertex2)
{
	VertexBuffer[0] = InVertex0;
	VertexBuffer[1] = InVertex1;
	VertexBuffer[2] = InVertex2;

	bHasColor = true;
	bHasUV = true;

	RecalcBounds();
}

void TriangleRasterizer::RecalcBounds()
{

	Vector2 minPos = Vector2(INFINITY, INFINITY);
	Vector2 maxPos = Vector2(-INFINITY, -INFINITY);

	for (int i = 0; i < 3; i++)
	{
		if (VertexBuffer[i].Position.X < minPos.X) minPos.X = VertexBuffer[i].Position.X;
		if (VertexBuffer[i].Position.Y < minPos.Y) minPos.Y = VertexBuffer[i].Position.Y;
		if (VertexBuffer[i].Position.X > maxPos.X) maxPos.X = VertexBuffer[i].Position.X;
		if (VertexBuffer[i].Position.Y > maxPos.Y) maxPos.Y = VertexBuffer[i].Position.Y;
	}

	U = VertexBuffer[1].Position - VertexBuffer[0].Position;
	V = VertexBuffer[2].Position - VertexBuffer[0].Position;

	UU = U.Dot(U);
	UV = U.Dot(V);
	VV = V.Dot(V);
	InvDenom = 1.f / (UU * VV - UV * UV);

	TopLeft = ScreenPoint(minPos);
	BottomRight = ScreenPoint(maxPos);
}
