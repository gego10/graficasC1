////////////////////////////////////////////////////////////////////////////////
// Filename: lightshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHTSHADERCLASS_H_
#define _LIGHTSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <fstream>
using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "openglclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: LightShaderClass
////////////////////////////////////////////////////////////////////////////////
class LightShaderClass
{
public:
	LightShaderClass(char* nombre, char* nombre2);
	LightShaderClass(const LightShaderClass&);
	~LightShaderClass();

	bool Initialize(OpenGLClass*, HWND);
	void Shutdown(OpenGLClass*);
	void SetShader(OpenGLClass*);	
	bool PonMatriz4x4(OpenGLClass*, char*, float*);
	bool Pon1Entero(OpenGLClass* OpenGL, char* nombre, int);
	bool PonVec3(OpenGLClass* OpenGL, char* nombre, float* vector);
	bool PonVec4(OpenGLClass* OpenGL, char* nombre, float* vector);

private:
	bool InitializeShader(char*, char*, OpenGLClass*, HWND);
	char* LoadShaderSourceFile(char*);
	void OutputShaderErrorMessage(OpenGLClass*, HWND, unsigned int, char*);
	void OutputLinkerErrorMessage(OpenGLClass*, HWND, unsigned int);
	void ShutdownShader(OpenGLClass*);

private:
	unsigned int m_vertexShader;
	unsigned int m_fragmentShader;
	unsigned int m_shaderProgram;
	char* nombreVs;
	char* nombrePS;
};

#endif