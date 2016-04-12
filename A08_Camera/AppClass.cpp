#include "AppClass.h"

void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Camera - Maximilian Wright"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
    cam = Camera::GetInstance();
	//Setting the position in which the camera is looking and its interest point

    prim = new PrimitiveClass();
    prim->GenerateCone(1.0f, 1.0f, 6, REBLUE);

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();
	//Counting the cumulative time
	static double fRunTime = 0.0f;
	fRunTime += fCallTime;

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

    m_pMeshMngr->Render(); //renders the render list

    prim->Render(cam->GetProjection(false), cam->GetView(), IDENTITY_M4);

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}