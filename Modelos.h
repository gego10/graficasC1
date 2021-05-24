#ifndef _modelo
#define _modelo
#include "Imagenes.h"
#include "VectorRR.h"
#include "Geometrias.h"
#include <vector>


class Modelos : public Primitivos
{

public:
	unsigned int ModTextura;
	VectorRR Up, derecha;
	float x, y, z, escala;

	//En honor a nuestros ancestros llamaremos "Maya" a la malla
	//e suna estructura que contiene a los indices y vertices de la figura
	Maya modelo;
	int canti, cantv;
	//variables locales de la clase para contener los stacks y los slices de la esfera
	int st, sl;
	OpenGLClass* sale;
	//el nombre numerico de la textura en cuestion, por lo pronto una	
	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;
	unsigned int m_textureID;

	Modelos(HWND hWnd, OpenGLClass* OpenGL, const char nombreFile[], const wchar_t textura[], const wchar_t normMap[], const wchar_t specMap[], float escala, float x, float z, float numModelo)
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

		//aqui debe de agregarse la extraccion de la textura del modelo en
		//en caso de que haya

		Carga(textura);
		// Set the unique texture unit in which to store the data.
		OpenGL->glActiveTexture(GL_TEXTURE0 + numModelo + 0);

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

		Carga(normMap);
		// Set the unique texture unit in which to store the data.
		OpenGL->glActiveTexture(GL_TEXTURE0 + numModelo + 1);

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

		Carga(specMap);
		// Set the unique texture unit in which to store the data.
		OpenGL->glActiveTexture(GL_TEXTURE0 + numModelo + 2);

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

		modeLoad(nombreFile);
		// Allocate an OpenGL vertex array object.
		OpenGL->glGenVertexArrays(1, &m_vertexArrayId);

		// Bind the vertex array object to store all the buffers and vertex attributes we create here.
		OpenGL->glBindVertexArray(m_vertexArrayId);

		// Generate an ID for the vertex buffer.
		OpenGL->glGenBuffers(1, &m_vertexBufferId);

		// Bind the vertex buffer and load the vertex (position, texture, and normal) data into the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glBufferData(GL_ARRAY_BUFFER, cantv * sizeof(Vertices), modelo.maya, GL_STATIC_DRAW);

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
		OpenGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, canti * sizeof(unsigned int),
			modelo.indices, GL_STATIC_DRAW);


	}

	~Modelos()
	{

		glDeleteTextures(1, &ModTextura);
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
		glDrawElements(GL_TRIANGLES, canti, GL_UNSIGNED_INT, 0);

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
		OpenGL->glDisableVertexAttribArray(2);
		OpenGL->glDisableVertexAttribArray(3);
		OpenGL->glDisableVertexAttribArray(4);

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

	bool modeLoad(const char nombreFileModel[])
	{
		struct vector3 {
			float x, y, z;
		};

		struct vector2 {
			float u, v;
		};

		vector<vector3> vertices;
		vector<vector2> uvs;
		vector<vector3> normales;
		vector<int> vertexIndices;
		vector<int> uvIndices;
		vector<int> normalIndices;

		errno_t err;
		FILE* file;
		err = fopen_s(&file, nombreFileModel, "r");
		if (file == NULL) {
			printf("Impossible to open the file !\n");
			return false;
		}

		while (1)
		{
			char lineHeader[128];
			// Lee la primera palabra de la línea
			int res = fscanf_s(file, "%s", lineHeader, 128);
			if (res == EOF)
				break; // EOF = End Of File, es decir, el final del archivo. Se finaliza el ciclo.

			if (strcmp(lineHeader, "v") == 0) {
				vector3 vertex;
				fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0) {
				vector2 uv;
				fscanf_s(file, "%f %f\n", &uv.u, &uv.v);
				uvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0) {
				vector3 normal;
				fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				normales.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					return false;
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
		}

		//modelo.maya = new Vertices[vertices.size()];
		//modelo.indices = new unsigned int[vertexIndices.size()];

		vector<Vertices> tempoVertices;
		vector<unsigned int> tempoIndices;

		for (int i = 0; i < vertexIndices.size(); i++)
		{

			Vertices aux;
			aux.Posx = vertices[vertexIndices[i] - 1].x * escala;
			aux.Posy = vertices[vertexIndices[i] - 1].y * escala;
			aux.Posz = vertices[vertexIndices[i] - 1].z * escala;
			aux.Normx = normales[normalIndices[i] - 1].x;
			aux.Normy = normales[normalIndices[i] - 1].y;
			aux.Normz = normales[normalIndices[i] - 1].z;
			aux.u = uvs[uvIndices[i] -1 ].u;
			aux.v = 1 - uvs[uvIndices[i] -1].v; //por que esta volteada la textura? no lo se, por eso le reste de 1
			//para enderezarla

			tempoVertices.push_back(aux);
		}

		modelo.maya = new Vertices[tempoVertices.size()];
		modelo.indices = new unsigned int[tempoVertices.size()];

		for (int i = 0; i < tempoVertices.size(); i++)
		{
			modelo.maya[i] = tempoVertices[i];

			modelo.indices[i] = i;
		}

		cantv = tempoVertices.size();
		canti = tempoVertices.size();

		for (int i = 0; i < vertexIndices.size(); i += 3)
		{
			vector3 edge1;
			edge1.x = modelo.maya[vertexIndices[i] - 1 + 2].Posx - modelo.maya[vertexIndices[i] - 1].Posx;
			edge1.y = modelo.maya[vertexIndices[i] - 1 + 2].Posy - modelo.maya[vertexIndices[i] - 1].Posy;
			edge1.z = modelo.maya[vertexIndices[i] - 1 + 2].Posz - modelo.maya[vertexIndices[i] - 1].Posz;
			vector3 edge2;
			edge2.x = modelo.maya[vertexIndices[i] - 1 + 1].Posx - modelo.maya[vertexIndices[i] - 1].Posx;
			edge2.y = modelo.maya[vertexIndices[i] - 1 + 1].Posy - modelo.maya[vertexIndices[i] - 1].Posy;
			edge2.z = modelo.maya[vertexIndices[i] - 1 + 1].Posz - modelo.maya[vertexIndices[i] - 1].Posz;


			float d1 = sqrt(modelo.maya[vertexIndices[i] - 1 + 2].u * modelo.maya[vertexIndices[i] - 1 + 2].u
				+ modelo.maya[vertexIndices[i] - 1 + 2].v * modelo.maya[vertexIndices[i] - 1 + 2].v);
			float d2 = sqrt(modelo.maya[vertexIndices[i] - 1].u * modelo.maya[vertexIndices[i] - 1].u
				+ modelo.maya[vertexIndices[i] - 1].v * modelo.maya[vertexIndices[i] - 1].v);
			float d3 = sqrt(modelo.maya[vertexIndices[i] - 1 + 1].u * modelo.maya[vertexIndices[i] - 1 + 1].u
				+ modelo.maya[vertexIndices[i] - 1 + 1].v * modelo.maya[vertexIndices[i] - 1 + 1].v);

			vector2 deltaUV1;
			float ddu1, ddu2;
			float ddv1, ddv2;
			float ddu3, ddv3;

			if (d1 > 0.0001)
				ddu1 = modelo.maya[vertexIndices[i] - 1 + 2].u / d1;
			else
				ddu1 = 1000;

			if (d2 > 0.0001)
				ddu2 = modelo.maya[vertexIndices[i] - 1].u / d2;
			else
				ddu2 = 1000;

			if (d1 > 0.0001)
				ddv1 = modelo.maya[vertexIndices[i] - 1 + 2].v / d1;
			else
				ddv1 = 1000;

			if (d2 > 0.0001)
				ddv2 = modelo.maya[vertexIndices[i] - 1].v / d2;
			else
				ddv2 = 1000;

			if (d3 > 0.0001)
				ddu3 = modelo.maya[vertexIndices[i] - 1 + 1].u / d3;
			else
				ddu3 = 1000;

			if (d3 > 0.0001)
				ddv3 = modelo.maya[vertexIndices[i] - 1 + 1].v / d3;
			else
				ddv3 = 1000;

			deltaUV1.u = ddu1 - ddu2;
			deltaUV1.v = ddv1 - ddv2;
			vector2 deltaUV2;
			deltaUV2.u = ddu3 - ddu2;
			deltaUV2.v = ddv3 - ddv2;


			float denom = (deltaUV1.u * deltaUV2.v - deltaUV2.u * deltaUV1.v);
			if (abs(denom) < 0.0001)
				denom = 0.001;
			float f = 1.0f / denom;

			vector3 tangent;
			tangent.x = f * (deltaUV2.v * edge1.x - deltaUV1.v * edge2.x);
			tangent.y = f * (deltaUV2.v * edge1.y - deltaUV1.v * edge2.y);
			tangent.z = f * (deltaUV2.v * edge1.z - deltaUV1.v * edge2.z);
			Normaliza((float*)&tangent);


			modelo.maya[vertexIndices[i] - 1].Tx = 0;
			modelo.maya[vertexIndices[i] - 1].Ty = 0;
			modelo.maya[vertexIndices[i] - 1].Tz = 1;

			modelo.maya[vertexIndices[i] - 1 + 1].Tx = 0;
			modelo.maya[vertexIndices[i] - 1 + 1].Ty = 0;
			modelo.maya[vertexIndices[i] - 1 + 1].Tz = 1;

			modelo.maya[vertexIndices[i] - 1 + 2].Tx = 0;
			modelo.maya[vertexIndices[i] - 1 + 2].Ty = 0;
			modelo.maya[vertexIndices[i] - 1 + 2].Tz = 1;

			if (i > 3068)
			{
				int p = 0;
			}

		}




		int u = 0;
	}

};

#endif 