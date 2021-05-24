#ifndef _Geom
#define _Geom
#define _USE_MATH_DEFINES
#include <math.h>
#include "VectorRR.h"
#include "Imagenes.h"

//estructura para manejar primitivos con posicion, normal y uv's
struct Vertices {
	float Posx, Posy, Posz;           
	float Normx, Normy, Normz;
	float u, v;
	float Tx, Ty, Tz;
	float Bx, By, Bz;
};

//En honor a nuestros ancestros llamaremos "Maya" a la malla
//estructura que contiene datos de los vertices y sus indices
struct Maya {
	Vertices *maya;
	unsigned int *indices;
};




class Primitivos: public VectorRR, public Imagenes
{
public:
	int cantVert;
	int cantIndices;
public:

	Primitivos() //mis pensamientos!
	{
		cantVert = 0;
		cantIndices = 0;
	}

	//generamos los vertices a traves de coordenadas esfericas
	//conocimiento adquirido en la materia de matematicas para videjuegos I
	Maya BillBoardBasico(float escala)
	{
		//Cargamos la estructura con los espacios de memoria necesarios
		Vertices *verticesxyzSD = new Vertices[4];
		cantVert = 4;
		unsigned int *indices = new unsigned int[6];
		cantIndices = 6;
		//generamos un objeto para poder transportar los punteros
		Maya salida;
		//a darle que es mole de olla!

		verticesxyzSD[0].Posx = -0.5 * escala;
		verticesxyzSD[0].Posy = 0 * escala;
		verticesxyzSD[0].Posz = 0 * escala;

		verticesxyzSD[0].Normx = 0;
		verticesxyzSD[0].Normy = 0;
		verticesxyzSD[0].Normz = 1;

		verticesxyzSD[0].u = 1;
		verticesxyzSD[0].v = 1;

		verticesxyzSD[1].Posx = -0.5 * escala;
		verticesxyzSD[1].Posy = 1 * escala;
		verticesxyzSD[1].Posz = 0 * escala;

		verticesxyzSD[1].Normx = 0;
		verticesxyzSD[1].Normy = 0;
		verticesxyzSD[1].Normz = 1;

		verticesxyzSD[1].u = 1;
		verticesxyzSD[1].v = 0;

		verticesxyzSD[2].Posx = 0.5 * escala;
		verticesxyzSD[2].Posy = 1 * escala;
		verticesxyzSD[2].Posz = 0 * escala;

		verticesxyzSD[2].Normx = 0;
		verticesxyzSD[2].Normy = 0;
		verticesxyzSD[2].Normz = 1;

		verticesxyzSD[2].u = 0;
		verticesxyzSD[2].v = 0;

		verticesxyzSD[3].Posx = 0.5 * escala;
		verticesxyzSD[3].Posy = 0 * escala;
		verticesxyzSD[3].Posz = 0 * escala;

		verticesxyzSD[3].Normx = 0;
		verticesxyzSD[3].Normy = 0;
		verticesxyzSD[3].Normz = 1;

		verticesxyzSD[3].u = 0;
		verticesxyzSD[3].v = 1;

		indices[0] = 0;
		indices[1] = 2;
		indices[2] = 1;
		indices[3] = 0;
		indices[4] = 3;
		indices[5] = 2;

		//una vez generados los damos a conocer a traves del objeto "salida"
		salida.maya = verticesxyzSD;
		salida.indices = indices;

		return salida;
	}

	//generamos los vertices a traves de coordenadas esfericas
	//conocimiento adquirido en la materia de matematicas para videjuegos I
	Maya Esfera(int stacks, int slices, float radio, float inicio, float final)
	{
		//Cargamos la estructura con los espacios de memoria necesarios
		Vertices *verticesxyzSD = new Vertices[stacks*slices];
		cantVert = stacks * slices;
		unsigned int *indices = new unsigned int[(stacks-1)*(slices-1) * 6];
		cantIndices = (stacks - 1)*(slices - 1) * 6;
		//generamos un objeto para poder transportar los punteros
		Maya salida;
		//a darle que es mole de olla!
		for (int i = 0; i < slices; i++)
		{
			for (int j = 0; j < stacks; j++)
			{
				int indice = (i * stacks + j);
				verticesxyzSD[indice].Posx = radio * cos(((double)j / (stacks - 1))*(M_PI*(final - inicio)) + M_PI * inicio - M_PI / 2.0)*
					cos(2.0*M_PI*(double)i / (slices - 1));
				verticesxyzSD[indice].Posy = radio * sin(((double)j / (stacks - 1))*(M_PI*(final - inicio)) + M_PI * inicio - M_PI / 2.0);
				verticesxyzSD[indice].Posz = radio * cos(((double)j / (stacks - 1))*(M_PI*(final - inicio)) + M_PI * inicio - M_PI / 2.0)*
					sin(2.0*M_PI*(double)i / (slices - 1));

				verticesxyzSD[indice].Normx = cos(((double)j / (stacks - 1))*(M_PI*(final - inicio)) + M_PI * inicio - M_PI / 2.0)*
					cos(2.0*M_PI*(double)i / (slices - 1));
				verticesxyzSD[indice].Normy = sin(((double)j / (stacks - 1))*(M_PI*(final - inicio)) + M_PI * inicio - M_PI / 2.0);
				verticesxyzSD[indice].Normz = cos(((double)j / (stacks - 1))*(M_PI*(final - inicio)) + M_PI * inicio - M_PI / 2.0)*
					sin(2.0*M_PI*(double)i / (slices - 1));

				verticesxyzSD[indice].u = (float)1 * (1.0 - (float)i / (float)(stacks - 1));
				verticesxyzSD[indice].v = (float)1 * (1.0 - (float)j / (float)(slices - 1));
			}
		}

		//ahora la parte mas importante de crear vertices es el algoritmo para unirlos, en este caso sustituiremos
		//a un algoritmo con un un grupo de indices
		int indice = 0;
		for (int i = 0; i < slices - 1; i++)
		{
			for (int j = 0; j < stacks - 1; j++)
			{
				indices[indice++] = i * stacks + j;
				indices[indice++] = (i + 1) * stacks + j + 1;
				indices[indice++] = i * stacks + j + 1;

				indices[indice++] = i * stacks + j;
				indices[indice++] = (i + 1) * stacks + j;
				indices[indice++] = (i + 1) * stacks + j + 1;
			}
		}

		//una vez generados los damos a conocer a traves del objeto "salida"
		salida.maya = verticesxyzSD;
		salida.indices = indices;

		return salida;
	}

	Maya Plano(int vertx, int vertz, float anchof, float profz)
	{
		//Cargamos la estructura con los espacios de memoria necesarios
		Vertices *verticesxyzSD = new Vertices[vertx*vertz];
		unsigned int *indices = new unsigned int[(vertx - 1)*(vertz - 1) * 6];
		cantVert = vertx * vertz;
		cantIndices = (vertx - 1)*(vertz - 1) * 6;
		//es la separacion entre vertices, se le resta 1 para que el lado correcto
		//imagine que el ancho es de 10 y tiene 10 vertices, entonces le daria un deltax
		//de 1, si los vertices van de 0 a 9 entonces la posicion del ultimo vertice
		//seria 9, si le divide entre vertx -1 le dara 1.1111, y el ultimo vertice sera 10
		float deltax = anchof / (vertx - 1);
		float deltaz = profz / (vertz - 1);

		//crea los vertices
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				verticesxyzSD[z*vertx + x].Posx = (float)x*deltax;
				verticesxyzSD[z*vertx + x].Posy = 0.0;
				verticesxyzSD[z*vertx + x].Posz = (float)z*deltaz;

				//carga las normales con cero
				verticesxyzSD[z*vertx + x].Normx = 0.0;
				verticesxyzSD[z*vertx + x].Normy = 1.0;
				verticesxyzSD[z*vertx + x].Normz = 0.0;
			}
		}

		//calcula los uv's
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				verticesxyzSD[z*vertx + x].u = (float)x / (vertx - 1);
				verticesxyzSD[z*vertx + x].v = (float)z / (vertz - 1);
			}
		}

		VectorRR aux;
		//crea las normales
		for (int z = 0; z < (vertz - 1); z++)
		{
			for (int x = 0; x < (vertx - 1); x++)
			{
				Copia(aux, GenNormal(&verticesxyzSD[z*vertx + x].Posx, &verticesxyzSD[(z + 1)*vertx + (x + 1)].Posx,
					&verticesxyzSD[z*vertx + (x + 1)].Posx));

				SumaNormal(&verticesxyzSD[z*vertx + x].Normx, &aux.X);
				SumaNormal(&verticesxyzSD[(z + 1)*vertx + (x + 1)].Normx, &aux.X);
				SumaNormal(&verticesxyzSD[z*vertx + (x + 1)].Normx, &aux.X);

				Copia(aux, GenNormal(&verticesxyzSD[z*vertx + x].Posx, &verticesxyzSD[(z + 1)*vertx + x].Posx,
					&verticesxyzSD[(z + 1)*vertx + (x + 1)].Posx));

				SumaNormal(&verticesxyzSD[z*vertx + x].Normx, &aux.X);
				SumaNormal(&verticesxyzSD[(z + 1)*vertx + x].Normx, &aux.X);
				SumaNormal(&verticesxyzSD[(z + 1)*vertx + (x + 1)].Normx, &aux.X);
			}
		}

		//Normaliza las normales
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				Normaliza(&verticesxyzSD[z*vertx + x].Normx);
			}
		}

		//ahora la parte mas importante de crear vertices es el algoritmo para unirlos, en este caso sustituiremos
		//a un algoritmo con un un grupo de indices
		int indice = 0;
		for (int i = 0; i < vertz - 1; i++)
		{
			for (int j = 0; j < vertx - 1; j++)
			{
				indices[indice++] = i * vertz + j;
				indices[indice++] = (i + 1) * vertz + j + 1;
				indices[indice++] = i * vertz + j + 1;

				indices[indice++] = i * vertz + j;
				indices[indice++] = (i + 1) * vertz + j;
				indices[indice++] = (i + 1) * vertz + j + 1;
			}
		}

		//generamos un objeto para poder transportar los punteros

		Maya salida;

		salida.maya = verticesxyzSD;
		salida.indices = indices;

		return salida;
	}


	Maya Plano(int vertx, int vertz, float anchof, float profz, unsigned char *altura, float tile = 1)
	{
		//Cargamos la estructura con los espacios de memoria necesarios
		Vertices *verticesxyzSD = new Vertices[vertx*vertz];
		unsigned int *indices = new unsigned int[(vertx-1)*(vertz-1) * 6];
		cantVert = vertx * vertz;
		cantIndices = (vertx - 1)*(vertz - 1) * 6;
		//es la separacion entre vertices, se le resta 1 para que el lado correcto
		//imagine que el ancho es de 10 y tiene 10 vertices, entonces le daria un deltax
		//de 1, si los vertices van de 0 a 9 entonces la posicion del ultimo vertice
		//seria 9, si le divide entre vertx -1 le dara 1.1111, y el ultimo vertice sera 10
		float deltax = anchof / (vertx - 1);
		float deltaz = profz / (vertz - 1);

		//crea los vertices
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				verticesxyzSD[z*vertx + x].Posx = (float)x*deltax - anchof / 2.0;
				verticesxyzSD[z*vertx + x].Posy = (float)altura[z*vertx * 4 + x * 4] / 6.0;
				verticesxyzSD[z*vertx + x].Posz = (float)z*deltaz - profz / 2.0;

				//carga las normales con cero
				verticesxyzSD[z*vertx + x].Normx = 0.0;
				verticesxyzSD[z*vertx + x].Normy = 0.0;
				verticesxyzSD[z*vertx + x].Normz = 0.0;

				//carga las tangent con cero
				verticesxyzSD[z * vertx + x].Tx = 0.0;
				verticesxyzSD[z * vertx + x].Ty = 0.0;
				verticesxyzSD[z * vertx + x].Tz = 0.0;

				//carga las binormales con cero
				verticesxyzSD[z * vertx + x].Bx = 0.0;
				verticesxyzSD[z * vertx + x].By = 0.0;
				verticesxyzSD[z * vertx + x].Bz = 0.0;
			}
		}

		//calcula los uv's
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				verticesxyzSD[z*vertx + x].u =  (float)(x*tile) / (vertx - 1);
				verticesxyzSD[z*vertx + x].v =  (float)(z*tile) / (vertz - 1);
			}
		}

		VectorRR aux;
		//crea las normales
		for (int z = 0; z < (vertz - 1); z++)
		{
			for (int x = 0; x < (vertx - 1); x++)
			{
				Copia(aux, GenNormal(&verticesxyzSD[z*vertx + x].Posx, &verticesxyzSD[z*vertx + (x + 1)].Posx,
					&verticesxyzSD[(z + 1)*vertx + (x + 1)].Posx));

				SumaNormal(&verticesxyzSD[z*vertx + x].Normx, &aux.X);
				SumaNormal(&verticesxyzSD[(z + 1)*vertx + (x + 1)].Normx, &aux.X);
				SumaNormal(&verticesxyzSD[z*vertx + (x + 1)].Normx, &aux.X);

				Copia(aux, GenNormal(&verticesxyzSD[z*vertx + x].Posx, &verticesxyzSD[(z + 1)*vertx + x + 1].Posx,
					&verticesxyzSD[(z + 1)*vertx + x].Posx));

				SumaNormal(&verticesxyzSD[z*vertx + x].Normx, &aux.X);
				SumaNormal(&verticesxyzSD[(z + 1)*vertx + x].Normx, &aux.X);
				SumaNormal(&verticesxyzSD[(z + 1)*vertx + (x + 1)].Normx, &aux.X);
			}
		}		

		VectorRR tangente;
		//crea las  tangentes
		for (int z = 0; z < (vertz - 1); z++)
		{
			for (int x = 0; x < (vertx - 1); x++)
			{
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// primer triangulo
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				tangente.X = verticesxyzSD[z * vertx + (x + 1)].Posx - verticesxyzSD[z * vertx + x].Posx;
				tangente.Y = verticesxyzSD[z * vertx + (x + 1)].Posy - verticesxyzSD[z * vertx + x].Posy;
				tangente.Z = verticesxyzSD[z * vertx + (x + 1)].Posz - verticesxyzSD[z * vertx + x].Posz;

				VectorRR aux;
				aux.X = verticesxyzSD[z * vertx + x].Normx;
				aux.Y = verticesxyzSD[z * vertx + x].Normy;
				aux.Z = verticesxyzSD[z * vertx + x].Normz;

				float dotazo = Punto(aux, tangente);
				
				VectorRR aux2;
				aux2.X = dotazo * aux.X;
				aux2.Y = dotazo * aux.Y;
				aux2.Z = dotazo * aux.Z;

				tangente.X = tangente.X - aux2.X;
				tangente.Y = tangente.Y - aux2.Y;
				tangente.Z = tangente.Z - aux2.Z;

				SumaNormal(&verticesxyzSD[z * vertx + x].Tx, &tangente.X);
				SumaNormal(&verticesxyzSD[(z + 1) * vertx + (x + 1)].Tx, &tangente.X);
				SumaNormal(&verticesxyzSD[z * vertx + (x + 1)].Tx, &tangente.X);
				
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// segundo triangulo
			    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				tangente.X = verticesxyzSD[(z + 1) * vertx + (x + 1)].Posx - verticesxyzSD[z * vertx + x].Posx;
				tangente.Y = verticesxyzSD[(z + 1) * vertx + (x + 1)].Posy - verticesxyzSD[z * vertx + x].Posy;
				tangente.Z = verticesxyzSD[(z + 1) * vertx + (x + 1)].Posz - verticesxyzSD[z * vertx + x].Posz;

				SumaNormal(&verticesxyzSD[z * vertx + x].Tx, &tangente.X);
				SumaNormal(&verticesxyzSD[(z + 1) * vertx + x].Tx, &tangente.X);
				SumaNormal(&verticesxyzSD[(z + 1) * vertx + (x + 1)].Tx, &tangente.X);				
			}
		}

		//Normaliza las normales
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				Normaliza(&verticesxyzSD[z * vertx + x].Normx);
				Normaliza(&verticesxyzSD[z * vertx + x].Tx);				
			}
		}

		VectorRR bitangente, tan , norm;
		//crea las  tangentes
		for (int z = 0; z < (vertz - 1); z++)
		{
			for (int x = 0; x < (vertx - 1); x++)
			{				
				bitangente.X = -(verticesxyzSD[z * vertx + x].Ty * verticesxyzSD[z * vertx + x].Normz - verticesxyzSD[z * vertx + x].Tz * 
					verticesxyzSD[z * vertx + x].Normy);
				bitangente.Y = -(verticesxyzSD[z * vertx + x].Tz * verticesxyzSD[z * vertx + x].Normx- verticesxyzSD[z * vertx + x].Tx * 
					verticesxyzSD[z * vertx + x].Normz);
				bitangente.Z = -(verticesxyzSD[z * vertx + x].Tx * verticesxyzSD[z * vertx + x].Normy - verticesxyzSD[z * vertx + x].Ty * 
					verticesxyzSD[z * vertx + x].Normx);

				SumaNormal(&verticesxyzSD[z * vertx + x].Bx, &bitangente.X);				
			}
		}

		//Normaliza las normales
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				Normaliza(&verticesxyzSD[z * vertx + x].Bx);				
			}
		}

		//ahora la parte mas importante de crear vertices es el algoritmo para unirlos, en este caso sustituiremos
		//a un algoritmo con un un grupo de indices
		int indice = 0;
		for (int i = 0; i < vertz - 1; i++)
		{
			for (int j = 0; j < vertx - 1; j++)
			{
				indices[indice++] = i * vertz + j;
				indices[indice++] = (i + 1) * vertz + j + 1;
				indices[indice++] = i * vertz + j + 1;

				indices[indice++] = i * vertz + j;
				indices[indice++] = (i + 1) * vertz + j;
				indices[indice++] = (i + 1) * vertz + j + 1;
			}
		}

		//generamos un objeto salida para poder transportar los punteros

		Maya salida;

		salida.maya = verticesxyzSD;
		salida.indices = indices;

		return salida;
	}
};
#endif