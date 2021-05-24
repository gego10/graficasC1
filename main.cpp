#include <windows.h>
#include <windowsx.h>
#include "openglclass.h"
#include "graphicsclass.h"
#include "GamepadRR.h"



#pragma comment (lib,"Gdiplus.lib")


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

LPCSTR m_applicationName;
HINSTANCE m_hinstance;
HWND m_hwnd;

OpenGLClass* m_OpenGL;
GraphicsClass* m_Graphics;
GamePadRR* control;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{	
	
	int screenWidth, screenHeight;
	bool result;
	m_OpenGL = 0;
	m_Graphics = 0;
	
	screenWidth = 0;
	screenHeight = 0;

	// Create the OpenGL object.
	m_OpenGL = new OpenGLClass;
	if (!m_OpenGL)
	{
		return false;
	}

	
	// Create the window the application will be using and also initialize OpenGL.
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = (LPCSTR)"Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPCSTR)m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Create a temporary window for the OpenGL extension setup.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, (LPCSTR)m_applicationName, (LPCSTR)m_applicationName, WS_POPUP,
		0, 0, 640, 480, NULL, NULL, m_hinstance, NULL);
	if (m_hwnd == NULL)
	{
		return false;
	}

	// Don't show the window.
	ShowWindow(m_hwnd, SW_HIDE);

	// Initialize a temporary OpenGL window and load the OpenGL extensions.
	result = m_OpenGL->InitializeExtensions(m_hwnd);
	if (!result)
	{
		MessageBox(m_hwnd, (LPCSTR)"Could not initialize the OpenGL extensions.", (LPCSTR)"Error", MB_OK);
		return false;
	}

	// Release the temporary window now that the extensions have been initialized.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, (LPCSTR)m_applicationName, (LPCSTR)m_applicationName, WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
	if (m_hwnd == NULL)
	{
		return false;
	}

	// Initialize OpenGL now that the window has been created.
	result = m_OpenGL->InitializeOpenGL(m_hwnd, screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR, VSYNC_ENABLED);
	if (!result)
	{
		MessageBox(m_hwnd, (LPCSTR)"Could not initialize OpenGL, check if video card supports OpenGL 4.0.",
			(LPCSTR)"Error", MB_OK);
		return false;
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);	

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(m_OpenGL, m_hwnd);
	if (!result)
	{
		return false;
	}

	control = new GamePadRR(1);

	MSG msg;
	bool done;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	SetTimer(m_hwnd, 1000, 20, NULL);
	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
			KillTimer(m_hwnd, 1000);
		}
		else
		{
			bool result;

			// Do the frame processing for the graphics object.
			result = m_Graphics->Frame();
			if (!result)
			{
				return false;
			}
		}

	}

	// Release the graphics object.
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}	

	// Release the OpenGL object.
	if (m_OpenGL)
	{
		m_OpenGL->Shutdown(m_hwnd);
		delete m_OpenGL;
		m_OpenGL = 0;
	}

	if (control)
	{		
		delete control;
		control = 0;
	}

	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass((LPCSTR)m_applicationName, m_hinstance);
	m_hinstance = NULL;	

	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	bool mientras = false;
	switch (umessage)
	{
		// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}	

	case WM_TIMER:
	{
		//////////////////////////////////////////////////////////////////////////////////////
		//aqui va el manejo del angulo para el calculo del vector de luz y el color de la luz
		//////////////////////////////////////////////////////////////////////////////////////
		bool conectado = control->IsConnected();
		if (control->IsConnected())
		{
			//convierto a flotante el valor analogico de tipo entero
			float grados = (float)control->GetState().Gamepad.sThumbLX / 32767.0;
			//debido a que los controles se aguadean con el uso entonces ya no dan el cero
			//en el centro, por eso lo comparo con una ventana de aguadencia de mi control
			if (grados > 0.19 || grados < -0.19)
				m_Graphics->angy += grados * 1.0;

			float velocidad = (float)control->GetState().Gamepad.sThumbLY / 32767.0;
			if (velocidad > 0.19 || velocidad < -0.19)
				m_Graphics->vel = velocidad/2.0;
			else
			{
				if (!mientras)
					m_Graphics->vel = 0;
			}
		}
	}

	case WM_KEYUP:
	{
		mientras = false;
		m_Graphics->vel = 0.0;
	}

	case WM_CHAR:
	{
		switch (wparam)
		{
			case 'd':
			{
				m_Graphics->angy += 0.6;
			}
			break;
			case 'a':
			{
				m_Graphics->angy -= 0.6;
			}
			break;
			case 'w':
			{
				m_Graphics->vel = 0.5;
				mientras = true;
			}
			break;
			case 'x':
			{
				m_Graphics->vel = -0.5;
				mientras = true;
			}
			break;
			default:
				mientras = false;
		}
	}

	// Any other messages send to the default message handler as our application won't make use of them.
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}		
	
	}
}