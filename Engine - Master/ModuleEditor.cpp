#include "ModuleEditor.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleTime.h"
#include "ModuleScene.h"

#include "Window.h"
#include "WindowScene.h"
#include "WindowAbout.h"
#include "WindowConsole.h"
#include "WindowHardware.h"
#include "WindowConfiguration.h"

#include "debugdraw.h"


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
	editorWindows.push_back(hardware = new WindowHardware("hardware"));
	editorWindows.push_back(configuration = new WindowConfiguration("configuration"));
	editorWindows.push_back(hierarchy = new WindowHierarchy("hierarchy"));
	editorWindows.push_back(inspector = new WindowInspector("inspector"));

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup style
	ImGui::StyleColorsDark();

	// Set default name for the scene
	strcpy(temp_name, "Untitled");

	return true;
}

update_status ModuleEditor::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	CreateDockSpace();

	// Update performance
	configuration->fps_log.erase(configuration->fps_log.begin());
	configuration->fps_log.push_back(App->time->FPS);
	configuration->ms_log.erase(configuration->ms_log.begin());
	configuration->ms_log.push_back(App->time->real_delta_time * 1000.0f);

	// Update game performance
	configuration->fps_game_log.erase(configuration->fps_game_log.begin());
	configuration->fps_game_log.push_back(App->time->FPS);
	configuration->ms_game_log.erase(configuration->ms_game_log.begin());
	configuration->ms_game_log.push_back(App->time->delta_time * App->time->time_scale * 1000.0f);

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
	ShowMainMenuBar();

	if (show_scene_save_popup)
		SceneSavePopup();
	if (show_scene_load_popup)
		SceneLoadPopup();

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

void ModuleEditor::DrawDebugReferences()
{
	if (show_grid)
	{
		dd::xzSquareGrid(-500.0f, 500.0f, 0.0f, 1.0f, math::float3(0.65f, 0.65f, 0.65f));
	}

	if (show_axis)
	{
		float axis_size = 5.0f;
		dd::axisTriad(math::float4x4::identity, axis_size*0.125f, axis_size*1.25f, 0, true);
	}

	//dd::sphere(App->models->light.pos, math::float3(1.0f, 1.0f, 1.0f), 0.05f);
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
			ImGui::Separator();
			if (ImGui::MenuItem("New Scene"))
			{
				strcpy(temp_name, "Untitled");
				App->scene->NewScene();
			}
			if (ImGui::MenuItem("Save Scene")) 
			{ 
				if (App->scene->name.empty())
				{
					show_scene_save_popup = true;
				}
				else
				{
					App->scene->SaveScene(App->scene->name.c_str());
				}
			}
			if (ImGui::MenuItem("Save Scene As..."))
			{
				if (!App->scene->name.empty())
				{
					strcpy(temp_name, App->scene->name.c_str());
				}
				else
				{
					strcpy(temp_name, "Untitled");
				}		
				show_scene_save_popup = true;
			}
			if (ImGui::MenuItem("Load Scene"))
			{
				strcpy(temp_name, "");
				show_scene_load_popup = true;
			}
			ImGui::Separator();
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
		if (ImGui::BeginMenu("Game Object"))
		{
			if (ImGui::MenuItem("Create Empty")) 
			{ 
				App->scene->CreateGameObject("GameObject"); 
			}
			if (ImGui::MenuItem("Create Sphere"))
			{
				App->model_loader->CreateSphere("sphere", math::float3(0.0f, 0.0f, 0.0f), Quat::identity, math::float3(1.0f, 1.0f, 1.0f), 20, 20, float4(0.f, 0.0f, 0.5f, 1.0f));
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Scene", NULL, scene->isActive())) { scene->toggleActive(); }
			if (ImGui::MenuItem("Inspector", NULL, inspector->isActive())) { inspector->toggleActive(); }
			if (ImGui::MenuItem("Hierarchy", NULL, hierarchy->isActive())) { hierarchy->toggleActive(); }
			ImGui::Separator();
			if (ImGui::MenuItem("Configuration", NULL, configuration->isActive())) { configuration->toggleActive(); }
			ImGui::Separator();

			if (ImGui::MenuItem("Hardware Info", NULL, hardware->isActive())) { hardware->toggleActive(); }
			ImGui::Separator();
			if (ImGui::MenuItem("Console", NULL, console->isActive())) { console->toggleActive(); }
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
		// Show Play/Pause/Stop/Step buttons (depending on current Game State)
		if (App->time->game_running == Game_State::Stoped)
		{
			if (ImGui::Button("Play"))
			{
				App->scene->SaveScene("temporalScene");
				App->time->Start_Game();
			}
		}
		else
		{
			if (ImGui::Button("Stop"))
			{
				App->time->Stop_Game();
				App->scene->LoadScene("temporalScene");
				App->scene->DeleteScene("temporalScene");
			}
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

void ModuleEditor::SceneSavePopup()
{
	ImGui::OpenPopup("Save Scene As");
	if (ImGui::BeginPopupModal("Save Scene As", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Scene name:\n");
		
		ImGui::InputText(".proScene", temp_name, 64);
		ImGui::Separator();

		if (ImGui::Button("Save", ImVec2(120, 0))) 
		{
			App->scene->name = temp_name;
			App->scene->SaveScene(App->scene->name.c_str());
			show_scene_save_popup = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			show_scene_save_popup = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void ModuleEditor::SceneLoadPopup()
{
	ImGui::OpenPopup("Load Scene");
	if (ImGui::BeginPopupModal("Load Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Scene name:\n");

		ImGui::InputText(".proScene", temp_name, 64);
		ImGui::Separator();

		if (ImGui::Button("Load", ImVec2(120, 0)))
		{
			App->scene->LoadScene(temp_name);
			show_scene_load_popup = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			show_scene_load_popup = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void ModuleEditor::ShowInBrowser(const char* url) const
{
	assert(url != nullptr);
	ShellExecute(0, "open", url, 0, 0, SW_SHOW);
}

void ModuleEditor::HandleInputs(SDL_Event& event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}