#include "ModuleRender.h"

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	context = SDL_GL_CreateContext(App->window->window);

	glewInit();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_2D);

	glClearDepth(1.0f);
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);

    int width, height;
    SDL_GetWindowSize(App->window->window, &width, &height);
    glViewport(0, 0, width, height);

	// Generate program with vertex and fragment shaders and load it to GL
	program = App->shader->LoadShaders("../default.vs", "../default.fs");
	programText = App->shader->LoadShaders("../texture.vs", "../texture.fs");

	if (!program || !programText) {
		LOG("Error: Program cannot be compiled");
		return false;
	}

	InitQuad();

	return true;
}

update_status ModuleRender::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	glUseProgram(program);
	DrawCoordinates();
	DrawPlane();
	glUseProgram(0);
	//math::float4x4 proj = App->camera->frustum.ProjectionMatrix();
	math::float4x4 proj = App->camera->ProjectionMatrix();
	//math::float4x4 view = App->camera->frustum.ViewMatrix();
	math::float4x4 view = App->camera->LookAt(App->camera->cam_target, App->camera->cam_position, App->camera->cam_up);

	for (unsigned i = 0; i < App->model_loader->meshes.size(); ++i)
	{
		const ModuleModelLoader::mesh& mesh = App->model_loader->meshes[i];

		RenderMesh(mesh, App->model_loader->materials[mesh.material], program,
			App->model_loader->transform, view, proj);
	}
	
	DrawQuad();

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	if (App->editor->io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window

	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
    glViewport(0, 0, width, height); 
}

void ModuleRender::RenderMesh(const ModuleModelLoader::mesh& mesh, const ModuleModelLoader::material& material,
	unsigned program, const math::float4x4& model, const math::float4x4& view, const math::float4x4& proj)
{
	//Use shaders loaded in program
	glUseProgram(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&model);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material.texture0);
	glUniform1i(glGetUniformLocation(program, "texture0"), 0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * mesh.num_vertices));
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * mesh.num_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
	glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

}

void ModuleRender::DrawCoordinates()
{
	glLineWidth(2.0f);

	// red X
	int xAxis = glGetUniformLocation(program, "newColor");
	float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glUniform4fv(xAxis, 1, red);

	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);
	glEnd();

	// green Y
	int yAxis = glGetUniformLocation(program, "newColor");
	float green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	glUniform4fv(yAxis, 1, green);

	glBegin(GL_LINES);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);
	glEnd();

	// blue Z
	int zAxis = glGetUniformLocation(program, "newColor");
	float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	glUniform4fv(zAxis, 1, blue);

	glBegin(GL_LINES);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);
	glEnd();

	glLineWidth(1.0f);
}

void ModuleRender::DrawPlane()
{
	glLineWidth(1.0f);
	int grid = glGetUniformLocation(program, "newColor");
	float cream[4] = { 0.988f, 0.918f, 0.592f, 1.0f };
	glUniform4fv(grid, 1, cream);

	glBegin(GL_LINES);

	float d = 200.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();
}

void ModuleRender::InitQuad()
{
	desatranques.name = "Desatranques Jaen";
	desatranques.path = "desatranques.jpg";
	desatranques.use_mipmap = true;
	sankara.name = "Thomas Sankara";
	sankara.path = "sankara.jpg";
	pazos.name = "Pazos64";
	pazos.path = "pazos.jpg";
	logo.path = "Red Controller Digital.png";
	// Load texture
	texture = App->textures->loadImage(desatranques);

	float vertex_buffer_data[] =
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

	glGenBuffers(1, &quad.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void ModuleRender::DrawQuad()
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quad.vbo);
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
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

