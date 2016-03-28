#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("SLERP - Jamie Lamoureux"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
	//Setting the position in which the camera is looking and its interest point
	m_pCameraMngr->SetPositionTargetAndView(vector3(12.12f, 28.52f, 11.34f), ZERO_V3, REAXISY);

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);

	//Loading the models
	m_pMeshMngr->LoadModel("Planets\\00_Sun.obj", "Sun");
	m_pMeshMngr->LoadModel("Planets\\03_Earth.obj", "Earth");
	m_pMeshMngr->LoadModel("Planets\\03A_Moon.obj", "Moon");
	m_pMeshMngr->LoadModel("Planets\\02_Venus.obj", "Venus");

	//Setting the days duration
	m_fDay = 1;
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();
	//Counting the cumulative time
	static double fRunTime = 0.0f;
	fRunTime += fCallTime;

	//Earth Orbit
	double fEarthHalfOrbTime = 182.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fEarthHalfRevTime = 0.5f * m_fDay; // Move for Half a day
	float fVenusHalfRevTime = 58.375 * m_fDay;
	float fMoonHalfOrbTime = 14.0f * m_fDay; //Moon's orbit is 28 earth days, so half the time for half a route
	float fVenusHalfOrbTime = 112.5f;

	//Matrices
	matrix4 m4Sun = IDENTITY_M4;
	matrix4 m4Earth = m4Sun;
	matrix4 m4Moon = m4Sun;
	matrix4 m4Venus = m4Sun;
	m4Sun *= glm::scale(vector3(5.936));

	//Calculating percentages
	float earthWhole;
	float earthPercent = modff((float)fRunTime / fEarthHalfOrbTime, &earthWhole);
	float spinWhole;
	float spinPercent = modff((float)fRunTime / fEarthHalfRevTime, &spinWhole);
	float moonWhole;
	float moonPercent = modff((float)fRunTime / fMoonHalfOrbTime, &moonWhole);
	float venusWhole;
	float venusPercent = modff((float)fRunTime / fVenusHalfOrbTime, &venusWhole);
	float venusRevWhole;
	float venusRevPercent = modff((float)fRunTime / fVenusHalfRevTime, &venusRevWhole);

	//Quaternions for rotation
	quaternion rotStart = quaternion(vector3(0));
	quaternion rotHalf = quaternion(vector3(0,PI,0));
	quaternion rotWhole = quaternion(vector3(0, PI * 2, 0));

	//Earth rotate/translate
	if((int)earthWhole % 2 == 0)
		m4Earth *= glm::mat4_cast(glm::mix(rotStart, rotHalf, earthPercent)) * glm::translate(11.0f, 0.0f, 0.0f);
	else
		m4Earth *= glm::mat4_cast(glm::mix(rotHalf, rotWhole, earthPercent)) * glm::translate(11.0f, 0.0f, 0.0f);	

	//Moon rotate/translate
	if((int)moonWhole %2 == 0)
		m4Moon = m4Earth * glm::mat4_cast(glm::mix(rotStart, rotHalf, moonPercent)) * glm::translate(2.0f, 0.0f, 0.0f);
	else
		m4Moon = m4Earth * glm::mat4_cast(glm::mix(rotHalf, rotWhole, moonPercent)) * glm::translate(2.0f, 0.0f, 0.0f);

	//Earth revolve
	if ((int)spinWhole % 2 == 0)
		m4Earth *= glm::mat4_cast(glm::mix(rotStart, rotHalf, spinPercent));
	else
		m4Earth *= glm::mat4_cast(glm::mix(rotHalf, rotWhole, spinPercent));

	//Moon revolve
	if((int)moonWhole % 2 == 0)
		m4Moon *= glm::mat4_cast(glm::mix(rotWhole, rotHalf, moonPercent));
	else
		m4Moon *= glm::mat4_cast(glm::mix(rotHalf, rotStart, moonPercent));

	//Venus rotate/translate
	if ((int)earthWhole % 2 == 0)
		m4Venus *= glm::mat4_cast(glm::mix(rotStart, rotHalf, venusPercent)) * glm::translate(7.95739644964f, 0.0f, 0.0f);
	else
		m4Venus *= glm::mat4_cast(glm::mix(rotHalf, rotWhole, venusPercent)) * glm::translate(7.95739644964f, 0.0f, 0.0f);

	//Venus revolve
	if ((int)venusRevWhole % 2 == 0)
		m4Venus *= glm::mat4_cast(glm::mix(rotStart, rotHalf, venusRevPercent));
	else
		m4Venus *= glm::mat4_cast(glm::mix(rotHalf, rotWhole, venusRevPercent));

	//Scaling the matrices
	m4Earth *= glm::scale(vector3(0.524));
	m4Moon *= glm::scale(vector3(0.27 * 0.524));
	m4Venus *= glm::scale(vector3(0.524 / 1.05270828652));

	//Setting the matrices
	m_pMeshMngr->SetModelMatrix(m4Sun, "Sun");
	m_pMeshMngr->SetModelMatrix(m4Earth, "Earth");
	m_pMeshMngr->SetModelMatrix(m4Venus, "Venus");
	m_pMeshMngr->SetModelMatrix(m4Moon, "Moon");

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");
	
	static int nEarthOrbits = earthWhole * 2;
	static int nEarthRevolutions = spinWhole * 2;
	static int nMoonOrbits = moonWhole * 2;

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("Time:");
	m_pMeshMngr->PrintLine(std::to_string(fRunTime));

	m_pMeshMngr->Print("Day:");
	m_pMeshMngr->PrintLine(std::to_string(m_fDay));

	m_pMeshMngr->Print("E_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthOrbits));

	m_pMeshMngr->Print("E_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthRevolutions));

	m_pMeshMngr->Print("M_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nMoonOrbits));

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default:
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}