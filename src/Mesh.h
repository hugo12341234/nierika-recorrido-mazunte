#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>

#include <GL/glew.h>
#include "Perf.h"   // X-68: cuenta llamadas de dibujo y triangulos cuando F9 / NIERIKA_PERF estan activos
#include "Cull.h"   // X-68: descarte por frustum y subida perezosa de la matriz "model"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



#include "Shader.h"

using namespace std;

struct Vertex
{
	// Position
	glm::vec3 Position;
	// Normal
	glm::vec3 Normal;
	// TexCoords
	glm::vec2 TexCoords;
};

struct Texture
{
	GLuint id;
	string type;
	aiString path;
};

class Mesh
{
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	/*  Functions  */
	// Constructor
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		this->calcularEsfera();

		// Now that we have all the required data, set the vertex buffers and its attribute pointers.
		this->setupMesh();
	}

	// Render the mesh
	void Draw(Shader& shader)
	{
		// X-68: descarte por frustum. Si la esfera envolvente de la malla queda fuera de la vision no se envia nada a la GPU
		// (ni texturas, ni uniforms, ni el dibujo): el resultado en pantalla es identico (la GPU la habria recortado igual).
		if (!Cull::Visible(this->centro, this->radio))
		{
			Perf::descartadas++;
			return;
		}
		Cull::Subir();   // X-68: sube la matriz "model" pendiente (subida perezosa, ver Cull.h)

		// X-68: ubicaciones de uniforms y nombres de samplers se calculan una sola vez (antes: un stringstream y hasta 6 glGetUniformLocation por malla y cuadro)
		if (this->locPrograma != shader.Program) this->prepararUniforms(shader);

		GLint tieneRelieve = 0;   // B-12: 1 si la malla trae mapa de relieve (lighting.frag lo lee)
		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			Cull::UniformeI(shader.Program, this->locSampler[i], (int)i);   // sampler -> unidad de textura i (solo si cambia)
			if (this->textures[i].type == "texture_height") tieneRelieve = 1;
			Cull::EnlazarTextura(i, this->textures[i].id);                  // solo si esa unidad tenia otra textura
		}

		Cull::UniformeI(shader.Program, this->locTieneRelieve, tieneRelieve);   // se reinicia en cada malla (0 si no hay relieve)

		// X-56: la malla es el vidrio de una ventana si su textura difusa se llama "vidrio*" (casa_a y casa_b). Se calcula una vez y se guarda.
		if (this->esVidrio < 0)
		{
			this->esVidrio = 0;
			for (size_t k = 0; k < this->textures.size(); k++)
				if (this->textures[k].type == "texture_diffuse" && string(this->textures[k].path.C_Str()).find("vidrio") != string::npos) this->esVidrio = 1;
		}
		Cull::UniformeI(shader.Program, this->locEsVidrio, this->esVidrio);

		// Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
		Cull::UniformeF(shader.Program, this->locShininess, 16.0f);

		// Draw mesh
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		Perf::llamadasDibujo++; Perf::triangulos += (long)(this->indices.size() / 3);
		glBindVertexArray(0);
		// X-68: ya no se desenlazan las texturas al terminar (cada malla enlaza las suyas y Cull recuerda cual hay en cada unidad)
	}

	// X-68: true si la esfera envolvente toca el frustum con la matriz "model" actual (Model::DrawAtuendo lo usa para no preparar mallas invisibles)
	bool EsVisible() const { return Cull::Visible(this->centro, this->radio); }

	// Libera la memoria de GPU de la malla (VAO, VBO, EBO). Se llama a mano al final de main (decision del 19/10:
	// sin destructores, porque Mesh se copia). Las texturas NO se borran aqui: las comparte el Model (ver Model::cleanup)
	void cleanup()
	{
		if (this->EBO) glDeleteBuffers(1, &this->EBO);
		if (this->VBO) glDeleteBuffers(1, &this->VBO);
		if (this->VAO) glDeleteVertexArrays(1, &this->VAO);
		this->VAO = this->VBO = this->EBO = 0;
	}

private:
	/*  Render data  */
	GLuint VAO = 0, VBO = 0, EBO = 0;
	int esVidrio = -1;   // X-56: -1 = sin calcular, 0 = no es vidrio, 1 = vidrio de ventana

	// X-68: esfera envolvente local (centro de la caja y distancia al vertice mas lejano + 0.5 m de holgura: el oleaje del mar mueve los
	// vertices en y hasta la suma de sus amplitudes) y ubicaciones de uniforms para el programa 'locPrograma'
	glm::vec3 centro = glm::vec3(0.0f);
	float radio = 1e9f;   // radio enorme = nunca se descarta (por si la malla no tiene vertices)
	GLuint locPrograma = 0;
	GLint locSampler[16];
	GLint locTieneRelieve = -1, locEsVidrio = -1, locShininess = -1;

	void calcularEsfera()
	{
		if (this->vertices.empty()) return;
		glm::vec3 mn = this->vertices[0].Position, mx = mn;
		for (size_t i = 1; i < this->vertices.size(); i++)
		{
			mn = glm::min(mn, this->vertices[i].Position);
			mx = glm::max(mx, this->vertices[i].Position);
		}
		this->centro = 0.5f * (mn + mx);
		float r2 = 0.0f;
		for (size_t i = 0; i < this->vertices.size(); i++)
		{
			const glm::vec3 d = this->vertices[i].Position - this->centro;
			r2 = glm::max(r2, glm::dot(d, d));
		}
		this->radio = std::sqrt(r2) + 0.5f;
	}

	// Nombres "texture_diffuse1", "texture_height1"... (el N cuenta por tipo, igual que antes) y sus ubicaciones en el programa
	void prepararUniforms(Shader& shader)
	{
		GLuint diffuseNr = 1, specularNr = 1, heightNr = 1;
		for (size_t i = 0; i < 16; i++) this->locSampler[i] = -1;
		for (size_t i = 0; i < this->textures.size() && i < 16; i++)
		{
			const string& name = this->textures[i].type;
			string number;
			if (name == "texture_diffuse") number = std::to_string(diffuseNr++);
			else if (name == "texture_specular") number = std::to_string(specularNr++);
			else if (name == "texture_height") number = std::to_string(heightNr++);
			this->locSampler[i] = glGetUniformLocation(shader.Program, (name + number).c_str());
		}
		this->locTieneRelieve = glGetUniformLocation(shader.Program, "tieneRelieve");
		this->locEsVidrio = glGetUniformLocation(shader.Program, "esVidrio");
		this->locShininess = glGetUniformLocation(shader.Program, "material.shininess");
		this->locPrograma = shader.Program;
	}

	/*  Functions    */
	// Initializes all the buffer objects/arrays
	void setupMesh()
	{
		// Create buffers/arrays
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);
		// Load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		// Set the vertex attribute pointers
		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Normal));
		// Vertex Texture Coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
};