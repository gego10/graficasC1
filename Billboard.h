#ifndef _billb
#define _billb
#include "Imagenes.h"
#include "VectorRR.h"
#include "Geometrias.h"


class Billboard : public Primitivos
{

public:
	unsigned int billbTextura;
	VectorRR Up, derecha;
	float x, y, z, escala;
	VectorRR BillCam;
	//En honor a nuestros ancestros llamaremos "Maya" a la malla
	//e suna estructura que contiene a los indices y vertices de la figura
	Maya billbo;
	//variables locales de la clase para contener los stacks y los slices de la esfera
	int st, sl;
	OpenGLClass* sale;
	//el nombre numerico de la textura en cuestion, por lo pronto una
	unsigned int billboTextura;
	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;
	unsigned int m_textureID;

	Billboard(HWND hWnd, OpenGLClass* OpenGL, const wchar_t textura[], float escala, float x, float z, int bill)
	{
		this->escala = escala;
		this->x = x;
		this->y = y;
		this->z = z;
		sale = OpenGL;
		Up.X = 0;
		Up.Y = 1;
		Up.Z = 0;
		//este vector es el que nos dara el ancho del billboard
		//cada vez que lo giremos segun la camara

		Carga(textura);
		// Set the unique texture unit in which to store the data.
		OpenGL->glActiveTexture(GL_TEXTURE0 + bill);

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

		billbo = BillBoardBasico(escala);

		// Allocate an OpenGL vertex array object.
		OpenGL->glGenVertexArrays(1, &m_vertexArrayId);

		// Bind the vertex array object to store all the buffers and vertex attributes we create here.
		OpenGL->glBindVertexArray(m_vertexArrayId);

		// Generate an ID for the vertex buffer.
		OpenGL->glGenBuffers(1, &m_vertexBufferId);

		// Bind the vertex buffer and load the vertex (position, texture, and normal) data into the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glBufferData(GL_ARRAY_BUFFER, cantVert * sizeof(Vertices), billbo.maya, GL_STATIC_DRAW);

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
			billbo.indices, GL_STATIC_DRAW);


	}

	~Billboard()
	{
		// Now that the buffers have been loaded we can release the array data.
		delete[] billbo.maya;
		billbo.maya = 0;

		delete[] billbo.indices;
		billbo.indices = 0;
		glDeleteTextures(1, &billbTextura);
		Shutdown(sale);
	}

	void Render(OpenGLClass* OpenGL)
	{
		//habilitamos el culling para reducir tiempo de procesamiento de las texturas
		//las texturas se analizan pixel a pixel para determinar que se imprimen o no
		//por lo que aunque no la veamos nos costo, por eso la eliminamos.	
		RenderBuffers(OpenGL);
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

	float angBill(float xc, float zc)
	{
		float vecx = xc - x;
		float vecz = zc - z;

		float ang = acos(vecz / (sqrt(vecx * vecx + vecz * vecz)));

		return ang;
	}

};

#endif 