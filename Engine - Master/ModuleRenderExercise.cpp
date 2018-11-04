#include "ModuleRenderExercise.h"

ModuleRenderExercise::ModuleRenderExercise()
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}

bool ModuleRenderExercise::Init()
{
	/*desatranques.name = "Desatranques Jaen";
	desatranques.path = "desatranques.jpg";
	desatranques.use_mipmap = true;
	sankara.name = "Thomas Sankara";
	sankara.path = "sankara.jpg";
	pazos.name = "Pazos64";
	pazos.path = "pazos.jpg";
	logo.path = "Red Controller Digital.png";
	// Load texture
	texture = App->textures->loadImage(desatranques);

	if (!texture) {
		LOG("Error: texture cannot be loaded");
		return false;
	}

	// Generate program with vertex and fragment shaders and load it to GL
	//program = App->shader->LoadShaders("../default.vs", "../default.fs");
	//programText = App->shader->LoadShaders("../texture.vs", "../texture.fs");

	if (!program || !programText) {
		LOG("Error: Program cannot be compiled");
		return false;
	}
	*/
	// Paint quad
	/*float vertex_buffer_data[] =
	{
		// positions
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,

		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,

		// uvs
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vbo;*/
	return true;
}

update_status ModuleRenderExercise::Update()
{
	/*glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		0,                  // attribute 0
		3,                  // number of componentes (3 floats)
		GL_FLOAT,           // data type
		GL_FALSE,           // should be normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Enable for textures
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 6) // buffer offset
	);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(programText, "texture0"), 0);

	//Use shaders loaded in program
	glUseProgram(program);
	
	// Draw plane and reference
	DrawCoordinates();
	DrawPlane();

	// Uniforms (can be changed from any place by calling newColour variable)
	// Fragment shader coloring
	int fragUnifLocation = glGetUniformLocation(program, "newColor");
	float color[4] = { 0.651f, 0.008f, 0.008f, 1.0f };
	glUniform4fv(fragUnifLocation, 1, color);
	

	math::float4x4 Model(math::float4x4::identity); // Not moving anything

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &App->camera->LookAt(App->camera->cam_target, App->camera->cam_position, App->camera->cam_up)[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &App->camera->ProjectionMatrix()[0][0]);

	glUseProgram(programText);
	glUniformMatrix4fv(glGetUniformLocation(programText, "model"), 1, GL_TRUE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programText, "view"), 1, GL_TRUE, &App->camera->LookAt(App->camera->cam_target, App->camera->cam_position, App->camera->cam_up)[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programText, "proj"), 1, GL_TRUE, &App->camera->ProjectionMatrix()[0][0]);


	glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/

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




