#pragma once
#include "GL/glew.h"
#include "MathGeoLib.h"
#include "Application.h"
#include "ModuleTextures.h"
#include <vector>

struct Vertex
{
	float3 position;
	float3 normal;
	float2 texCoords;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<image> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<image> textures) : 
		vertices(vertices), indices(indices), textures(textures)
	{
		this->SetupMesh();
	}

private:
	GLuint VAO, VBO, EBO;

	void SetupMesh()
	{
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		// Vertex positions
	}
};

