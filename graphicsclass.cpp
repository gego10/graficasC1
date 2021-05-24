////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_OpenGL = 0;
	m_Camera = 0;
	m_LightShader = 0;
	m_LightShaderSky = 0;
	m_BillShader = 0;
	m_Light = 0;
	angx = angy = angz = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(OpenGLClass* OpenGL, HWND hwnd)
{
	bool result;


	// Store a pointer to the OpenGL class object.
	m_OpenGL = OpenGL;

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, 110.0f);

	terreno = new Terreno(hwnd, m_OpenGL, L"terreno.jpg", L"Zacatito.jpg", L"ZacatitoNorm.jpg",
		(float)600, (float)600, 1, 2);

	// Create the light shader object.
	m_LightShader = new LightShaderClass((char*)"light.vs", (char*)"light.ps");
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_OpenGL, hwnd);
	if(!result)
	{
		MessageBox(hwnd, (LPCSTR)"Could not initialize the light shader object.", (LPCSTR)"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(-5.0f, 0.5f, -1.0f);

	sky = new SkyDome(hwnd, m_OpenGL, 32, 32, 256, L"earth.jpg",0);
	// Create the light shader object.
	m_LightShaderSky = new LightShaderClass((char*)"Sky.vs", (char*)"Sky.ps");
	if (!m_LightShaderSky)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShaderSky->Initialize(m_OpenGL, hwnd);
	if (!result)
	{
		MessageBox(hwnd, (LPCSTR)"Could not initialize the light shader object.", (LPCSTR)"Error", MB_OK);
		return false;
	}

	bill = new Billboard(hwnd, m_OpenGL, L"tree.png", 10.0f, 25.0f, 110.0f,3);
	// Create the light shader object.
	m_BillShader = new LightShaderClass((char*)"Bill.vs", (char*)"Bill.ps");
	if (!m_BillShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_BillShader->Initialize(m_OpenGL, hwnd);
	if (!result)
	{
		MessageBox(hwnd, (LPCSTR)"Could not initialize the light shader object.", (LPCSTR)"Error", MB_OK);
		return false;
	}
	
	modelazo = new Modelos(hwnd, m_OpenGL,"black_smith.obj",L"Blksmith.jpg",L"BlacksmithN.png", L"BlacksmithS.png", 5, -100, 50,7);
	  /*casa = new Modelos(hwnd, m_OpenGL, "black_smith.obj", L"Blksmith.jpg", L"BlacksmithN.png", L"BlacksmithS.png", 8, -200, 50, 0);*/
	  /*molino = new Modelos(hwnd, m_OpenGL, "molino.obj", L"tmolino.jpg", L"nmolino.jpg", L"smolino.jpg", 1, -100, 100, 6);*/
	m_ModeloShader = new LightShaderClass((char*)"Modelo.vs", (char*)"Modelo.ps");
	if (!m_ModeloShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_ModeloShader->Initialize(m_OpenGL, hwnd);
	if (!result)
	{
		MessageBox(hwnd, (LPCSTR)"Could not initialize the light shader object.", (LPCSTR)"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown(m_OpenGL);
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the light shader object.
	if (m_LightShaderSky)
	{
		m_LightShaderSky->Shutdown(m_OpenGL);
		delete m_LightShaderSky;
		m_LightShaderSky = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the terreno object.
	if (terreno)
	{
		delete terreno;
		m_Camera = 0;
	}

	// Release the pointer to the OpenGL class object.
	m_OpenGL = 0;

	return;
}


bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += 0.0174532925f * 2.0f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float rotation)
{
	float worldMatrix[16];
	float viewMatrix[16];
	float projectionMatrix[16];
	float lightDirection[3];
	float diffuseLightColor[4];
	float worldMatrixBill[16];


	// Clear the buffers to begin the scene.
	m_OpenGL->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->SetRotation(angx, angy, angz);
	m_Camera->SetYPos(terreno->Superficie(m_Camera->GetXPos(), m_Camera->GetZPos()) + 3);
	m_Camera->Render(vel);

	// Get the world, view, and projection matrices from the opengl and camera objects.
	m_OpenGL->GetWorldMatrix(worldMatrix);
	m_OpenGL->GetWorldMatrix(worldMatrixBill);
	m_Camera->GetViewMatrix(viewMatrix);
	m_OpenGL->GetProjectionMatrix(projectionMatrix);

	//////////////////////////////////////////////////////////
	//actualizar aqui el vector de luz y el color de la misma
	//////////////////////////////////////////////////////////

	// Get the light properties.
	m_Light->GetDirection(lightDirection);
	m_Light->GetDiffuseColor(diffuseLightColor);

	glDisable(GL_DEPTH_TEST);
	m_LightShaderSky->SetShader(m_OpenGL);
	m_LightShaderSky->PonMatriz4x4(m_OpenGL, (char*)"worldMatrix", worldMatrix);
	m_LightShaderSky->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
	m_LightShaderSky->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);
	m_LightShaderSky->Pon1Entero(m_OpenGL, (char*)"cielo", 0);
	m_LightShaderSky->PonVec3(m_OpenGL, (char*)"lightDirection", lightDirection);
	m_LightShaderSky->PonVec4(m_OpenGL, (char*)"diffuseLightColor", diffuseLightColor);
	sky->Render(m_OpenGL);
	glEnable(GL_DEPTH_TEST);
	// Rotate the world matrix by the rotation value so that the triangle will spin.
	//m_OpenGL->MatrixRotationY(worldMatrix, rotation);

	//// Set the light shader as the current shader program and set the matrices that it will use for rendering.
	m_LightShader->SetShader(m_OpenGL);	
	m_LightShader->PonMatriz4x4(m_OpenGL, (char*)"worldMatrix", worldMatrix);
	m_LightShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
	m_LightShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);
	m_LightShader->Pon1Entero(m_OpenGL, (char*)"shaderTexture", 1);
	m_LightShader->Pon1Entero(m_OpenGL, (char*)"shaderTexture2", 2);
	m_LightShader->PonVec3(m_OpenGL, (char*)"lightDirection", lightDirection);
	m_LightShader->PonVec4(m_OpenGL, (char*)"diffuseLightColor", diffuseLightColor);
	// Render the model using the light shader.
	terreno->Render(m_OpenGL);

	//// Set the light shader as the current shader program and set the matrices that it will use for rendering.
	//
	m_OpenGL->MatrixTranslation(worldMatrixBill, bill->x, terreno->Superficie(bill->x, bill->z) - 1, bill->z);	
	float rotay[16];
	m_OpenGL->MatrixRotationY(rotay, -bill->angBill(m_Camera->GetXPos(), m_Camera->GetZPos()));
	m_OpenGL->MatrixMultiply(worldMatrixBill, rotay, worldMatrixBill);
	m_BillShader->SetShader(m_OpenGL);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"worldMatrix", worldMatrixBill);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
	m_BillShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);
	m_BillShader->Pon1Entero(m_OpenGL, (char*)"billtext", 3);
	m_BillShader->PonVec3(m_OpenGL, (char*)"lightDirection", lightDirection);
	m_BillShader->PonVec4(m_OpenGL, (char*)"diffuseLightColor", diffuseLightColor);
	// Render the model using the light shader.
	bill->Render(m_OpenGL);
	
	
	m_ModeloShader->SetShader(m_OpenGL);
	float modmatrix[16];
	m_OpenGL->GetWorldMatrix(modmatrix);
	m_OpenGL->MatrixTranslation(modmatrix, modelazo->x, terreno->Superficie(modelazo->x, modelazo->z), modelazo->z);
	m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"worldMatrix", modmatrix);
	m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
	m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);
	m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"modtext", 7);
	m_ModeloShader->PonVec3(m_OpenGL, (char*)"lightDirection", lightDirection);
	m_ModeloShader->PonVec4(m_OpenGL, (char*)"diffuseLightColor", diffuseLightColor);
	 //Render the model using the light shader.
	modelazo->Render(m_OpenGL);
	/* Present the rendered scene to the screen.*/
	


	//m_ModeloShader->SetShader(m_OpenGL);
	//m_OpenGL->GetWorldMatrix(modmatrix);
	//m_OpenGL->MatrixTranslation(modmatrix, casa->x, terreno->Superficie(casa->x, casa->z), casa->z);
	//m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"worldMatrix", modmatrix);
	//m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
	//m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);
	//m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"modtext", 0);
	//m_ModeloShader->PonVec3(m_OpenGL, (char*)"lightDirection", lightDirection);
	//m_ModeloShader->PonVec4(m_OpenGL, (char*)"diffuseLightColor", diffuseLightColor);

	//casa->Render(m_OpenGL);
	//
	m_OpenGL->EndScene();
	return true;
}


	//m_ModeloShader->SetShader(m_OpenGL);
	//m_OpenGL->GetWorldMatrix(modmatrix);
	//m_OpenGL->MatrixTranslation(modmatrix, molino->x, terreno->Superficie(molino->x, molino->z), molino->z);
	//m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"worldMatrix", modmatrix);
	//m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"viewMatrix", viewMatrix);
	//m_ModeloShader->PonMatriz4x4(m_OpenGL, (char*)"projectionMatrix", projectionMatrix);
	//m_ModeloShader->Pon1Entero(m_OpenGL, (char*)"modtext",6);
	//m_ModeloShader->PonVec3(m_OpenGL, (char*)"lightDirection", lightDirection);
	//m_ModeloShader->PonVec4(m_OpenGL, (char*)"diffuseLightColor", diffuseLightColor);


	//molino->Render(m_OpenGL);
	//// Present the rendered scene to the screen.

