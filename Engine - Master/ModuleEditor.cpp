#include "ModuleEditor.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"

ModuleEditor::ModuleEditor()
{
}

// Destructor
ModuleEditor::~ModuleEditor()
{
}

// Called before render is available
bool ModuleEditor::Init()
{
	bool ret = true;
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	//ImGui_ImplOpenGL3_Init(glsl_version); CAMBIAR TRAS HACER LOS SHADERS
	ImGui_ImplOpenGL3_Init("#version 130");
	return ret;
}

bool ModuleEditor::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	return true;
}

