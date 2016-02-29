#include "MyPrimitive.h"
#include <cmath>
MyPrimitive::MyPrimitive() { }
MyPrimitive::MyPrimitive(const MyPrimitive& other) { }
MyPrimitive& MyPrimitive::operator=(const MyPrimitive& other) { return *this; }
MyPrimitive::~MyPrimitive(void) { super::Release(); }
void MyPrimitive::CompileObject(vector3 a_v3Color)
{
	m_uVertexCount = static_cast<int> (m_lVertexPos.size());
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		AddVertexColor(a_v3Color);
	}
	
	CompleteTriangleInfo(true);
	CompileOpenGL3X();

}
//C--D
//|\ |
//| \|
//A--B
//This will make the triang A->B->C and then the triang C->B->D
void MyPrimitive::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyPrimitive::GeneratePlane(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;

	vector3 pointA(-fValue, -fValue, 0.0f); //0
	vector3 pointB(fValue, -fValue, 0.0f); //1
	vector3 pointC(fValue, fValue, 0.0f); //2
	vector3 pointD(-fValue, fValue, 0.0f); //3

	vector3 pointE(fValue, -fValue, -0.001f); //1
	vector3 pointF(-fValue, -fValue, -0.001f); //0
	vector3 pointG(fValue, fValue, -0.001f); //2
	vector3 pointH(-fValue, fValue, -0.001f); //3

											  //F
	AddQuad(pointA, pointB, pointD, pointC);
	//Double sided
	AddQuad(pointE, pointF, pointG, pointH);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	vector3 top(0.0f, a_fHeight / 2, 0.0f);
	vector3 bottom(0.0f, -a_fHeight / 2, 0.0f);
	vector3* points = new vector3[a_nSubdivisions];
	
	//setting up points
	for (int i = 0; i < a_nSubdivisions; i++) {
		float angle = 360.0f / a_nSubdivisions * i;
		angle = (angle / 360) * 2 * PI;
		float x = cos(angle);
		float z = sin(angle);
		points[i] = vector3(x * a_fRadius, -a_fHeight / 2, z * a_fRadius);
	}

	//add vertices to mesh
	for (int k = 0; k < a_nSubdivisions; k++) {
		//side
		AddVertexPosition(top);
		AddVertexPosition(points[(k + 1) % a_nSubdivisions]);
		AddVertexPosition(points[k]);

		//bottom
		AddVertexPosition(bottom);
		AddVertexPosition(points[k]);
		AddVertexPosition(points[(k + 1) % a_nSubdivisions]);
	}

	//deallocate
	delete[] points;

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	vector3 top(0.0f, a_fHeight / 2, 0.0f);
	vector3 bottom(0.0f, -a_fHeight / 2, 0.0f);
	vector3* upper = new vector3[a_nSubdivisions];
	vector3* lower = new vector3[a_nSubdivisions];

	//setting upper and lower points
	for (int i = 0; i < a_nSubdivisions; i++) {
		float angle = 360.0f / a_nSubdivisions * i;
		angle = (angle / 360) * 2 * PI;
		float x = cos(angle);
		float z = sin(angle);
		upper[i] = vector3(x * a_fRadius, a_fHeight / 2, z * a_fRadius);
		lower[i] = vector3(x * a_fRadius, -a_fHeight / 2, z * a_fRadius);
	}

	//add vertices to mesh
	for (int k = 0; k < a_nSubdivisions; k++) {
		//top
		AddVertexPosition(top);
		AddVertexPosition(upper[(k + 1) % a_nSubdivisions]);
		AddVertexPosition(upper[k]);
		//bottom
		AddVertexPosition(bottom);
		AddVertexPosition(lower[k]);
		AddVertexPosition(lower[(k + 1) % a_nSubdivisions]);
		//side quads
		AddQuad(upper[k], upper[(k + 1) % a_nSubdivisions], lower[k], lower[(k + 1) % a_nSubdivisions]);
	}

	//deallocate
	delete[] upper;
	delete[] lower;

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	vector3* upperOuter = new vector3[a_nSubdivisions];
	vector3* upperInner = new vector3[a_nSubdivisions];
	vector3* lowerOuter = new vector3[a_nSubdivisions];
	vector3* lowerInner = new vector3[a_nSubdivisions];

	//setting upper and lower (inner/outer) points
	for (int i = 0; i < a_nSubdivisions; i++) {
		float angle = 360.0f / a_nSubdivisions * i;
		angle = (angle / 360) * 2 * PI;
		float x = cos(angle);
		float z = sin(angle);
		upperOuter[i] = vector3(x * a_fOuterRadius, a_fHeight / 2, z * a_fOuterRadius);
		upperInner[i] = vector3(x * a_fInnerRadius, a_fHeight / 2, z * a_fInnerRadius);
		lowerOuter[i] = vector3(x * a_fOuterRadius, -a_fHeight / 2, z * a_fOuterRadius);
		lowerInner[i] = vector3(x * a_fInnerRadius, -a_fHeight / 2, z * a_fInnerRadius);
	}

	//add vertices to mesh
	for (int k = 0; k < a_nSubdivisions; k++) {
		//outside quads
		AddQuad(upperOuter[k], upperOuter[(k + 1) % a_nSubdivisions], lowerOuter[k], lowerOuter[(k + 1) % a_nSubdivisions]);
		//inner quads
		AddQuad(lowerInner[k], lowerInner[(k + 1) % a_nSubdivisions], upperInner[k], upperInner[(k + 1) % a_nSubdivisions]);
		//top quads
		AddQuad(upperInner[k], upperInner[(k + 1) % a_nSubdivisions], upperOuter[k], upperOuter[(k + 1) % a_nSubdivisions]);
		//bottom quads
		AddQuad(lowerOuter[k], lowerOuter[(k + 1) % a_nSubdivisions], lowerInner[k], lowerInner[(k + 1) % a_nSubdivisions]);
	}

	//deallocate
	delete[] upperOuter;
	delete[] upperInner;
	delete[] lowerOuter;
	delete[] lowerInner;

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius <= a_fInnerRadius + 0.1f)
		return;

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 25)
		a_nSubdivisionsA = 25;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 25)
		a_nSubdivisionsB = 25;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	AddQuad(point0, point1, point3, point2);

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 12)
		a_nSubdivisions = 12;

	Release();
	Init();

	//Your code starts here
	float* radii = new float[a_nSubdivisions];
	vector3* current = new vector3[a_nSubdivisions];
	vector3* next = new vector3[a_nSubdivisions];
	vector3 top(0, a_fRadius, 0);
	vector3 bottom(0, -a_fRadius, 0);
	float segHeight = a_fRadius * 2 / (a_nSubdivisions - 1);

	//set radiuses
	for (int i = 0; i < a_nSubdivisions / 2; i++) {
		//radii[i] = radii[a_nSubdivisions - i - 1] = i / (a_nSubdivisions / 2.0f);
		radii[i] = radii[a_nSubdivisions - i - 1] = sqrt(pow(a_fRadius, 2) - pow(a_fRadius - (segHeight * i), 2));
	}

	if (a_nSubdivisions % 2 != 0) {
		radii[a_nSubdivisions / 2] = a_fRadius;
	}

	//setting current and next points
	for (int a = 0; a < a_nSubdivisions -2; a++) {
		for (int j = 0; j < a_nSubdivisions; j++) {
			float angle = 360.0f / a_nSubdivisions * j;
			angle = (angle / 360) * 2 * PI;
			float x = cos(angle);
			float z = sin(angle);
			current[j] = vector3(cos(angle) * radii[a], a_fRadius - (a * segHeight), sin(angle) * radii[a]);
			next[j] = vector3(cos(angle) * radii[a+1], a_fRadius - ((a+1) * segHeight), sin(angle) * radii[a+1]);
		}
		for (int k = 0; k < a_nSubdivisions; k++) {
			//add first quads
			if (a == 1) {
				AddQuad(top, top, next[k], next[(k + 1) % a_nSubdivisions]);
			}
			//add last quads
			if (a == a_nSubdivisions - 3) {
				AddQuad(next[k], next[(k + 1) % a_nSubdivisions], bottom, bottom);
			}
			//add current quads
			AddQuad(current[k], current[(k + 1) % a_nSubdivisions], next[k], next[(k + 1) % a_nSubdivisions]);
		}
	}
	


	//debug
	for (int k = 0; k < a_nSubdivisions; k++) {
		std::cout << radii[k] << std::endl;
	}

	//Your code ends here
	CompileObject(a_v3Color);
}