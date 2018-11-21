#include "ModuleEditor.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleTime.h"
#include "Window.h"
#include "WindowScene.h"
#include "WindowAbout.h"
#include "WindowConsole.h"
#include "WindowPerformance.h"
#include "WindowConfiguration.h"
#include "WindowProperties.h"

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
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

	// Add EditorWindows
	editorWindows.push_back(scene = new WindowScene("Scene"));
	editorWindows.push_back(about = new WindowAbout("About"));
	editorWindows.push_back(console = new WindowConsole("console"));
	editorWindows.push_back(performance = new WindowPerformance("performance"));
	editorWindows.push_back(configuration = new WindowConfiguration("configuration"));
	editorWindows.push_back(properties = new WindowProperties("properties"));

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

	CreateDockSpace();

	// Update performance
	performance->fps_log.erase(performance->fps_log.begin());
	performance->fps_log.push_back(App->time->FPS);
	performance->ms_log.erase(performance->ms_log.begin());
	performance->ms_log.push_back(App->time->real_delta_time * 1000.0f);

	// Update game performance
	configuration->fps_game_log.erase(configuration->fps_game_log.begin());
	configuration->fps_game_log.push_back(App->time->FPS);
	configuration->ms_game_log.erase(configuration->ms_game_log.begin());
	configuration->ms_game_log.push_back(App->time->delta_time * 1000.0f);

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
	ShowMainMenuBar();

	//ImGui::ShowDemoWindow();	//Example Window
	return update;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	for (std::list<Window*>::iterator it = editorWindows.begin(); it != editorWindows.end(); ++it)
	{
		RELEASE((*it));
	}

	editorWindows.clear();
	console = nullptr;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleEditor::Draw()
{
	for (std::list<Window*>::iterator it = editorWindows.begin(); it != editorWindows.end(); ++it)
	{
		if ((*it)->isActive())
		{
			ImGui::SetNextWindowSizeConstraints({ 10,10 }, { (float)App->window->screen_width, (float)App->window->screen_height });
			(*it)->Draw();
		}
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ModuleEditor::HandleInputs(SDL_Event& event) 
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void ModuleEditor::CreateDockSpace() const
{
	ImGui::SetNextWindowPos({ 0,0 });
	ImGui::SetNextWindowSize({ (float)App->window->screen_width, (float)App->window->screen_height });
	ImGui::SetNextWindowBgAlpha(0.0f);

	//TODO: change this to a simple define
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", 0, windowFlags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspaceId = ImGui::GetID("DockSpace");
	ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
}

void ModuleEditor::ShowMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open")) {}
			if (ImGui::MenuItem("Quit", "ALT+F4")) { update = UPDATE_STOP; }
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
			if (ImGui::MenuItem("Scene", NULL, scene->isActive())) { scene->toggleActive(); }
			ImGui::Separator();
			if (ImGui::MenuItem("Performance", NULL, performance->isActive())) { performance->toggleActive(); }
			if (ImGui::MenuItem("Console", NULL, console->isActive())) { console->toggleActive(); }
			if (ImGui::MenuItem("Configuration", NULL, configuration->isActive())) { configuration->toggleActive(); }
			if (ImGui::MenuItem("Model Info", NULL, properties->isActive())) { properties->toggleActive(); }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About", NULL, about->isActive())) { about->toggleActive(); }
			ImGui::Separator();
			if (ImGui::MenuItem("View GIT Repository")) { ShowInBrowser("https://github.com/Rubetoman/SDL-OpenGL-Engine-V2"); }
			ImGui::EndMenu();
		}
		ImGui::SameLine(300); ImGui::Separator();
		if (App->time->game_running == Game_State::Stoped)
		{
			if (ImGui::Button("Play"))
				App->time->Start_Game();
		}
		else
		{
			if (ImGui::Button("Stop"))
				App->time->Stop_Game();
		}
		if (ImGui::Button("Pause"))
		{
			if(App->time->game_running == Game_State::Running)
				App->time->Pause_Game(true);
			else if(App->time->game_running == Game_State::Paused)
				App->time->Pause_Game(false);
		}
		// Step
		if (App->time->step_frame)
		{
			App->time->Pause_Game(true);
			App->time->step_frame = false;
		}
		if (App->time->game_running != Game_State::Running)
		{
			if (ImGui::Button("Step"))
			{
				App->time->step_frame = true;
				App->time->Pause_Game(false);
			}
		}
		ImGui::Separator();
		ImGui::EndMainMenuBar();
	}
}

void ModuleEditor::ShowInBrowser(const char* url) const
{
	assert(url != nullptr);
	ShellExecute(0, "open", url, 0, 0, SW_SHOW);
}
