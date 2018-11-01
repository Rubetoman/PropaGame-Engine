#include "ModuleEditor.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleRenderExercise.h"

static void ShowMainMenuBar();
static void ShowOptionsWindow();
static void ShowLogWindow();
static void ShowAboutWindow();

ModuleEditor::ModuleEditor()
{
	fps_log.resize(100);
	ms_log.resize(100);
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

	// Update performance
	fps_log.erase(fps_log.begin());
	fps_log.push_back(App->FPS);
	ms_log.erase(ms_log.begin());
	ms_log.push_back(App->deltaTime * 1000);


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
	if (show_performance_window) {
		ShowPerformanceWindow();
	}
	if (show_textures_window) {
		ShowTexturesWindow();
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
			if (ImGui::MenuItem("Performance")) { App->editor->show_performance_window = true; }
			if(ImGui::MenuItem("Texture Options")) { App->editor->show_textures_window = true; }
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

void ModuleEditor::ShowPerformanceWindow()
{
	ImGui::Begin("Performance", &App->editor->show_performance_window);   // Pointer to bool variable (close when click on button)
	char title[25];
	sprintf_s(title, 25, "Framerate %0.1f", fps_log[fps_log.size() - 1]);
	ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 200.0f, ImVec2(310, 100));
	sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
	ImGui::PlotHistogram("##framerate", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	ImGui::End();
}

void ModuleEditor::ShowTexturesWindow()
{
	ImGui::Begin("Textures", &App->editor->show_textures_window, ImGuiWindowFlags_AlwaysAutoResize);
	image* images[] = { &App->exercise->desatranques, &App->exercise->sankara, &App->exercise->pazos };
	static image* current_item = images[0];
	if (ImGui::BeginCombo("Loaded textures", current_item->name, ImGuiComboFlags_NoArrowButton))
	{
		for (int n = 0; n < IM_ARRAYSIZE(images); n++)
		{
			bool is_selected = (current_item == images[n]);
			if (ImGui::Selectable(images[n]->name, is_selected)) {
				current_item = images[n];
				App->textures->ReloadImage(*images[n], App->exercise->texture);
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Texture information")) {
		ImGui::InputText("Format", current_item->format, sizeof(current_item->format));
		ImGui::InputInt("Width", &current_item->width, 0, 0);
		ImGui::InputInt("Height", &current_item->height, 0, 0);
	}
	if (ImGui::CollapsingHeader("Texture config")) {
		//Show resize options
		ImGui::Separator();
		//Show wrap options
		ImGui::Separator();
		//Show mipmap options
	}
	ImGui::End();
}
