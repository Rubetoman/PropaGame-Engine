#include "ModuleRenderExercise.h"


ModuleRenderExercise::ModuleRenderExercise()
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}

bool ModuleRenderExercise::Init()
{
	// Generate program with vertex and fragment shaders and load it to GL
	program = App->shader->LoadShaders("../default.vs", "../default.fs");

	if (!program) {
		LOG("Error: Program cannot be compiled");
		return false;
	}

	glUseProgram(program);
	float vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vbo;
}

update_status ModuleRenderExercise::Update()
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		0,                  // attribute 0
		3,                  // number of componentes (3 floats)
		GL_FLOAT,           // data type
		GL_FALSE,           // should be normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	//Use shaders loadeds in program
	glUseProgram(program);

	// Uniforms (can be changed from any place by calling newColour variable)
	// Fragment shader coloring
	int fragUnifLocation = glGetUniformLocation(program, "newColor");
	float color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glUniform4fv(fragUnifLocation, 1, color);

	math::float4x4 Model(math::float4x4::identity); // Not moving anything

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &Model[0][0]);
	//glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &App->camera->LookAt(App->camera->target, App->camera->eye, App->camera->up)[0][0]);
	//glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &App->camera->ProjectionMatrix()[0][0]);


	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	if (vbo != 0)
	{
		glDeleteBuffers(1, &vbo);
	}

	return true;
}



