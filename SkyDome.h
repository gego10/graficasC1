#ifndef _Sky
#define _Sky
#include "Geometrias.h"
#include "Imagenes.h"

class SkyDome :public Primitivos
{
public:
	//En honor a nuestros ancestros llamaremos "Maya" a la malla
	//e suna estructura que contiene a los indices y vertices de la figura
	Maya cuadro;
	//variables locales de la clase para contener los stacks y los slices de la esfera
	int st, sl;
	OpenGLClass* sale;
	//el nombre numerico de la textura en cuestion, por lo pronto una
	unsigned int esferaTextura;
	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;
	unsigned int m_textureID;
	//El constructor llama al metodo Esfera de la clase geometrias que generara los vertices
	//normales y uvs de la misma, nos regresa la estructura Maya.
	SkyDome(HWND hWnd, OpenGLClass* OpenGL, int stacks, int slices, float radio, const wchar_t nombre[],int num)
	{
		this->hWnd = hWnd;
		sale = OpenGL;
		cuadro = Esfera(stacks, slices, radio, 0.5, 1);
		st = stacks;
		sl = slices;
		//cargamos la textura de la figura
		Carga(nombre);
		int an = Ancho();
		int alt = Alto();
		// Set the unique texture unit in which to store the data.
		OpenGL->glActiveTexture(GL_TEXTURE0 + num);

		// Generate an ID for the texture.
		glGenTextures(1, &m_textureID);

		// Bind the texture as a 2D texture.
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		// Load the image data into the texture unit.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Ancho(), Alto(), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, Dir_Imagen());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Set the texture filtering.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// Generate mipmaps for the texture.
		OpenGL->glGenerateMipmap(GL_TEXTURE_2D);
		Descarga();
		

		// Allocate an OpenGL vertex array object.
		OpenGL->glGenVertexArrays(1, &m_vertexArrayId);

		// Bind the vertex array object to store all the buffers and vertex attributes we create here.
		OpenGL->glBindVertexArray(m_vertexArrayId);

		// Generate an ID for the vertex buffer.
		OpenGL->glGenBuffers(1, &m_vertexBufferId);

		// Bind the vertex buffer and load the vertex (position, texture, and normal) data into the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glBufferData(GL_ARRAY_BUFFER, cantVert * sizeof(Vertices), cuadro.maya, GL_STATIC_DRAW);

		// Enable the three vertex array attributes.
		OpenGL->glEnableVertexAttribArray(0);  // Vertex position.
		OpenGL->glEnableVertexAttribArray(1);  // Normals.
		OpenGL->glEnableVertexAttribArray(2);  // Texture coordinates.
		OpenGL->glEnableVertexAttribArray(3);  // Tangent.
		OpenGL->glEnableVertexAttribArray(4);  // BiNormals.

		// Specify the location and format of the position portion of the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertices), 0);

		// Specify the location and format of the normal vector portion of the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (3 * sizeof(float)));

		// Specify the location and format of the texture coordinate portion of the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (6 * sizeof(float)));

		// Specify the location and format of the texture coordinate portion of the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (8 * sizeof(float)));

		// Specify the location and format of the texture coordinate portion of the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (11 * sizeof(float)));

		// Generate an ID for the index buffer.
		OpenGL->glGenBuffers(1, &m_indexBufferId);

		// Bind the index buffer and load the index data into it.
		OpenGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
		OpenGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, cantIndices * sizeof(unsigned int),
			cuadro.indices, GL_STATIC_DRAW);

		// Now that the buffers have been loaded we can release the array data.
		delete[] cuadro.maya;
		cuadro.maya = 0;

		delete[] cuadro.indices;
		cuadro.indices = 0;

		
	}

	~SkyDome()
	{
		//nos aseguramos de disponer de los recursos previamente reservados
		delete cuadro.maya;
		delete cuadro.indices;
		glDeleteTextures(1, &esferaTextura);
		Shutdown(sale);
	}

	
	void Render(OpenGLClass* OpenGL)
	{
		//habilitamos el culling para reducir tiempo de procesamiento de las texturas
		//las texturas se analizan pixel a pixel para determinar que se imprimen o no
		//por lo que aunque no la veamos nos costo, por eso la eliminamos.
		
		glDisable(GL_DEPTH_TEST);
		// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
		RenderBuffers(OpenGL);
		//glFrontFace(GL_CW);
		//glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		return;
	}

	void RenderBuffers(OpenGLClass* OpenGL)
	{
		// Bind the vertex array object that stored all the information about the vertex and index buffers.
		OpenGL->glBindVertexArray(m_vertexArrayId);

		// Render the vertex buffer using the index buffer.
		glDrawElements(GL_TRIANGLES, cantIndices, GL_UNSIGNED_INT, 0);

		return;
	}

	void Shutdown(OpenGLClass* OpenGL)
	{

		// Release the vertex and index buffers.
		ShutdownBuffers(OpenGL);

		return;
	}

	void ShutdownBuffers(OpenGLClass* OpenGL)
	{
		// Disable the two vertex array attributes.
		OpenGL->glDisableVertexAttribArray(0);
		OpenGL->glDisableVertexAttribArray(1);

		// Release the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, 0);
		OpenGL->glDeleteBuffers(1, &m_vertexBufferId);

		// Release the index buffer.
		OpenGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		OpenGL->glDeleteBuffers(1, &m_indexBufferId);

		// Release the vertex array object.
		OpenGL->glBindVertexArray(0);
		OpenGL->glDeleteVertexArrays(1, &m_vertexArrayId);

		return;
	}
};
#endif