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
	
	if (show_options_window) {
		ShowOptionsWindow();
	}
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
	ImGui::Begin("Window Options", &App->editor->show_options_window);   // Pointer to bool variable (close when click on button)
	//Window size fields (only be able to edit if not in full screen)
	static ImU32 width_step = (ImU32)30;
	static ImU32 height_step = (ImU32)50;
	ImGui::InputScalar("Width", ImGuiDataType_U32, &App->window->screen_width, App->window->fullscreen ? NULL : &width_step, NULL, "%u");
	ImGui::InputScalar("Height", ImGuiDataType_U32, &App->window->screen_height, App->window->fullscreen ? NULL : &height_step, NULL, "%u");

	if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen))
		App->window->ToggleFullScreen();
	ImGui::SameLine();
	if (ImGui::Checkbox("Resizable", &App->window->resizable))
		App->window->ToggleResizable();
	if (ImGui::Checkbox("VSync", &App->window->vsync))
		App->window->ToggleVSync();
	ImGui::End();
}

static void ShowLogWindow()
{

}

static void ShowAboutWindow() 
{
	ImGui::Begin("About", &App->editor->show_about_window);   // Pointer to bool variable (close when click on button)
	ImGui::Text(TITLE);
	ImGui::Text("Version:	BT - 1");
	ImGui::Text("Games are not free of propaganda");
	ImGui::Text("C/C++ engine made by Ruben Crispin De la Cruz");
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
	texture* images[] = { &App->exercise->desatranques, &App->exercise->sankara, &App->exercise->pazos };
	static texture* current_item = images[0];
	if (ImGui::BeginCombo("Loaded textures", current_item->name, ImGuiComboFlags_NoArrowButton))
	{
		for (int n = 0; n < IM_ARRAYSIZE(images); n++)
		{
			bool is_selected = (current_item == images[n]);
			if (ImGui::Selectable(images[n]->name, is_selected) && !is_selected) {
				current_item = images[n];
				App->textures->ReloadImage(*images[n], App->exercise->quadTexture);
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
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Texture config")) {
		//Show resize options
		const char* resize_options[] = { "nearest", "linear" };
		const char* current_resize = current_item->getResizeMode();
		if (ImGui::BeginCombo("Resize Options", current_resize, ImGuiComboFlags_NoArrowButton))
		{
			for (int i = 0; i < IM_ARRAYSIZE(resize_options); i++)
			{
				bool is_selected = (current_resize == resize_options[i]);
				if (ImGui::Selectable(resize_options[i], current_resize) && !is_selected)			// Reload texture if resize option has change
				{
					current_item->setResizeMode(resize_options[i]);
					App->textures->ReloadImage(*current_item, App->exercise->quadTexture);
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		//Show wrap options
		const char* wrap_options[] = { "clamp to boder", "clamp", "repeat", "mirrored" };
		const char* current_wrap = current_item->getWrapMode();
		if (ImGui::BeginCombo("Wrap Options", current_wrap, ImGuiComboFlags_NoArrowButton))
		{
			for (int i = 0; i < IM_ARRAYSIZE(wrap_options); i++)
			{
				bool is_selected = (current_wrap == wrap_options[i]);
				if (ImGui::Selectable(wrap_options[i], current_wrap) && !is_selected) // Reload texture if wrap option has change
				{
					current_item->setWrapMode(wrap_options[i]);
					App->textures->ReloadImage(*current_item, App->exercise->quadTexture);
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::Separator();
		//Show mipmap options
		bool mipmap = current_item->use_mipmap;
		ImGui::Checkbox("Use Mipmap", &mipmap);
		if (mipmap != current_item->use_mipmap)
		{
			current_item->use_mipmap = mipmap;
			App->textures->ReloadImage(*current_item, App->exercise->quadTexture);
		}
	}
	ImGui::End();
}
