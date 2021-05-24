#ifndef _terreno
#define _terreno
#include "Geometrias.h"
#include "openglclass.h"
#include <gl/glu.h>

class Terreno : public Primitivos
{

private:
	float anchof;
	float proff;
	float deltax, deltaz;
	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;
	OpenGLClass* sale;
public:

	Maya terreno;
	int verx, verz;
	//el nombre numerico de la textura en cuestion, por lo pronto una
	unsigned int planoTextura;
	unsigned int m_textureID;

	Terreno(HWND hWnd, OpenGLClass* OpenGL, const wchar_t* alturas, const wchar_t* textura, const wchar_t* textura2,
		float ancho, float prof, int numtext, int numtext2)
	{
		anchof = ancho;
		proff = prof;
		//cargamos la textura de la figura
		Carga(alturas);
		//en caso del puntero de la imagen sea nulo se brica esta opcion
		terreno = Plano(Ancho(), Alto(), ancho, prof, Dir_Imagen(), 100);
		deltax = anchof / Ancho();
		deltaz = proff / Alto();
		verx = Ancho();
		verz = Alto();
		//disponemos la textura del gdi.
		Descarga();
		sale = OpenGL;
		// Allocate an OpenGL vertex array object.
		OpenGL->glGenVertexArrays(1, &m_vertexArrayId);

		// Bind the vertex array object to store all the buffers and vertex attributes we create here.
		OpenGL->glBindVertexArray(m_vertexArrayId);

		// Generate an ID for the vertex buffer.
		OpenGL->glGenBuffers(1, &m_vertexBufferId);

		// Bind the vertex buffer and load the vertex (position, texture, and normal) data into the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glBufferData(GL_ARRAY_BUFFER, cantVert * sizeof(Vertices), terreno.maya, GL_STATIC_DRAW);

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
			terreno.indices, GL_STATIC_DRAW);

		

		delete[] terreno.indices;
		terreno.indices = 0;

		Carga(textura);
		// Set the unique texture unit in which to store the data.
		OpenGL->glActiveTexture(GL_TEXTURE0 + numtext);

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

		Carga(textura2);
		// Set the unique texture unit in which to store the data.
		OpenGL->glActiveTexture(GL_TEXTURE0 + numtext2);

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
	}

	Terreno()
	{}

	~Terreno()
	{
		// Now that the buffers have been loaded we can release the array data.
		delete[] terreno.maya;
		terreno.maya = 0;
		//nos aseguramos de disponer de los recursos previamente reservados			
		glDeleteTextures(1, &planoTextura);		
		Shutdown(sale);
	}
	

	float Superficie(float x, float z)
	{
		//obtenemos el indice pero podria incluir una fraccion
		float indicefx = (x + anchof / 2) / deltax;
		float indicefz = (z + proff / 2) / deltaz;
		//nos quedamos con solo la parte entera del indice
		int indiceix = (int)indicefx;
		int indiceiz = (int)indicefz;
		//nos quedamos con solo la fraccion del indice
		float difx = indicefx - indiceix;
		float difz = indicefz - indiceiz;

		float altura;
		float D;

		//el cuadro del terreno esta formado por dos triangulos, si difx es mayor que dify 
		//entonces estamos en el triangulo de abajo en caso contrario arriba
		if (difx > difz)
		{
			//obtenemos el vector 1 de dos que se necesitan
			VectorRR v1(terreno.maya[indiceix + 1 + (indiceiz + 1) * verx].Posx - terreno.maya[indiceix + indiceiz * verx].Posx,
				terreno.maya[indiceix + 1 + (indiceiz + 1) * verx].Posy - terreno.maya[indiceix + indiceiz * verx].Posy,
				terreno.maya[indiceix + 1 + (indiceiz + 1) * verx].Posz - terreno.maya[indiceix + indiceiz * verx].Posz);
			//obtenemos el vector 2 de dos
			VectorRR v2(terreno.maya[indiceix + 1 + indiceiz * verx].Posx - terreno.maya[indiceix + indiceiz * verx].Posx,
				terreno.maya[indiceix + 1 + indiceiz * verx].Posy - terreno.maya[indiceix + indiceiz * verx].Posy,
				terreno.maya[indiceix + 1 + indiceiz * verx].Posz - terreno.maya[indiceix + indiceiz * verx].Posz);

			//con el producto punto obtenemos la normal y podremos obtener la ecuacion del plano
			//la parte x de la normal nos da A, la parte y nos da B y la parte z nos da C
			VectorRR normalPlano = Cruz(v1, v2);
			//entonces solo falta calcular D
			D = -1 * (normalPlano.X * terreno.maya[indiceix + indiceiz * verx].Posx +
				normalPlano.Y * terreno.maya[indiceix + indiceiz * verx].Posy +
				normalPlano.Z * terreno.maya[indiceix + indiceiz * verx].Posz);
			//sustituyendo obtenemos la altura de contacto en el terreno
			altura = ((-normalPlano.X * x - normalPlano.Z * z - D) / normalPlano.Y);
		}
		else
		{
			VectorRR v1(terreno.maya[indiceix + (indiceiz + 1) * verx].Posx - terreno.maya[indiceix + indiceiz * verx].Posx,
				terreno.maya[indiceix + (indiceiz + 1) * verx].Posy - terreno.maya[indiceix + indiceiz * verx].Posy,
				terreno.maya[indiceix + (indiceiz + 1) * verx].Posz - terreno.maya[indiceix + indiceiz * verx].Posz);

			VectorRR v2(terreno.maya[indiceix + 1 + (indiceiz + 1) * verx].Posx - terreno.maya[indiceix + indiceiz * verx].Posx,
				terreno.maya[indiceix + 1 + (indiceiz + 1)* verx].Posy - terreno.maya[indiceix + indiceiz * verx].Posy,
				terreno.maya[indiceix + 1 + (indiceiz + 1)* verx].Posz - terreno.maya[indiceix + indiceiz * verx].Posz);


			VectorRR normalPlano = Cruz(v1, v2);

			D = -1 * (normalPlano.X * terreno.maya[indiceix + indiceiz * verx].Posx +
				normalPlano.Y * terreno.maya[indiceix + indiceiz * verx].Posy +
				normalPlano.Z * terreno.maya[indiceix + indiceiz * verx].Posz);

			altura = ((-normalPlano.X * x - normalPlano.Z * z - D) / normalPlano.Y);
		}

		return altura;

	}

	///////////////////////////////////////////////////////////////////////////////////////////
	   

	void Shutdown(OpenGLClass* OpenGL)
	{

		// Release the vertex and index buffers.
		ShutdownBuffers(OpenGL);

		return;
	}


	void Render(OpenGLClass* OpenGL)
	{
		// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
		RenderBuffers(OpenGL);

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
	
	void RenderBuffers(OpenGLClass* OpenGL)
	{
		// Bind the vertex array object that stored all the information about the vertex and index buffers.
		OpenGL->glBindVertexArray(m_vertexArrayId);

		// Render the vertex buffer using the index buffer.
		glDrawElements(GL_TRIANGLES, cantIndices, GL_UNSIGNED_INT, 0);

		return;
	}	
};

#endif 