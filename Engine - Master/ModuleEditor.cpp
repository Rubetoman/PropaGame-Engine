#include "ModuleEditor.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"

static void ShowMainMenuBar();
static void ShowOptionsWindow();
static void ShowLogWindow();
static void ShowAboutWindow();

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

	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Window

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup style
	ImGui::StyleColorsDark();

	return true;
}

update_status ModuleEditor::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
	//Example Window
	ShowMainMenuBar();
	ImGui::ShowDemoWindow();
	
	if (show_about_window) {
		ShowAboutWindow();
	}


	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleEditor::HandleInputs(SDL_Event& event) {
	ImGui_ImplSDL2_ProcessEvent(&event);
}

static void ShowMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open")) {}
			if (ImGui::MenuItem("Quit", "ALT+F4")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Options")) { App->editor->show_options_window = true; }
			if (ImGui::MenuItem("Log")) { App->editor->show_log_window = true; }
			if (ImGui::MenuItem("Hardware Info")) { ShellExecute(0, 0, "https://github.com/Rubetoman/SDL-OpenGL-Engine-V2", 0, 0, SW_SHOW); }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About")) { App->editor->show_about_window = true; }
			ImGui::Separator();
			if (ImGui::MenuItem("View GIT Repository")) { ShellExecute(0, 0, "https://github.com/Rubetoman/SDL-OpenGL-Engine-V2", 0, 0, SW_SHOW); }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

static void ShowOptionsWindow() 
{
	/*ImGui::Begin("About", &App->editor->show_options_window);   // Pointer to bool variable (close when click on button)
	if (ImGui::Checkbox("Fullscreen", &fullscreen))
		App->window->SetFullScreen(fullscreen);

	ImGui::SameLine();
	if (ImGui::Checkbox("Resizable", &resizable))
		App->window->SetResizable(resizable);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Restart to apply");*/
}

static void ShowLogWindow()
{

}

static void ShowAboutWindow() 
{
	ImGui::Begin("About", &App->editor->show_about_window);   // Pointer to bool variable (close when click on button)
	ImGui::Text("PropaGame Engine");
	ImGui::Text("Version:	BT - 1");
	ImGui::Text("Propaganda takes over games");
	ImGui::Text("Made by Ruben Crispin De la Cruz");
	ImGui::Text("Libraries: ");
	ImGui::Text("	- glew-2.1.0");
	ImGui::Text("	- imgui v1.65 (with branching)");
	ImGui::Text("	- OpenGL 3.0");
	ImGui::Text("	- MathGeoLib");
	ImGui::Text("	- SDL 2.0.8");
	ImGui::Text("MIT License Copyright (c) [2018] [Ruben Crispin]");

	if (ImGui::Button("Close"))
		App->editor->show_about_window = false;
	ImGui::End();
}

