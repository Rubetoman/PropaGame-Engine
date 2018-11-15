#include "ModuleEditor.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "Window.h"
#include "WindowScene.h"
#include "WindowAbout.h"
#include "WindowConsole.h"
#include "WindowPerformance.h"
#include "WindowConfiguration.h"

ModuleEditor::ModuleEditor()
{
	editorWindows.push_back(scene = new WindowScene("Scene"));
	editorWindows.push_back(about = new WindowAbout("About"));
	editorWindows.push_back(console = new WindowConsole("console"));
	editorWindows.push_back(performance = new WindowPerformance("performance"));
	editorWindows.push_back(configuration = new WindowConfiguration("configuration"));
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

	CreateDockSpace();

	// Update performance
	performance->fps_log.erase(performance->fps_log.begin());
	performance->fps_log.push_back(App->FPS);
	performance->ms_log.erase(performance->ms_log.begin());
	performance->ms_log.push_back(App->deltaTime * 1000);

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update()
{
	ShowMainMenuBar();
	
	//ImGui::ShowDemoWindow();	//Example Window

	if (show_properties_window)	{ShowPropertiesWindow();}
	return update;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	for (std::list<Window*>::iterator it_p = editorWindows.begin(); it_p != editorWindows.end(); it_p++)
	{
		RELEASE((*it_p));
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
	for (std::list<Window*>::iterator it_p = editorWindows.begin(); it_p != editorWindows.end(); it_p++)
	{
		if ((*it_p)->isActive())
		{
			ImGui::SetNextWindowSizeConstraints({ 10,10 }, { (float)App->window->screen_width, (float)App->window->screen_height });
			(*it_p)->Draw();
		}
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

const void ModuleEditor::HandleInputs(SDL_Event& event) {
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void ModuleEditor::CreateDockSpace()
{
	ImGui::SetNextWindowPos({ 0,0 });
	ImGui::SetNextWindowSize({ (float)App->window->screen_width, (float)App->window->screen_height });
	ImGui::SetNextWindowBgAlpha(0.0f);

	//TODO: change this to a simple define
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", NULL, windowFlags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspaceId = ImGui::GetID("DockSpace");
	ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
}

const void ModuleEditor::ShowMainMenuBar()
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
			ImGui::Separator();
			if (ImGui::MenuItem("Properties", NULL, &show_properties_window)) { show_properties_window = true; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Performance", NULL, performance->isActive())) { performance->toggleActive(); }
			if (ImGui::MenuItem("Console", NULL, console->isActive())) { console->toggleActive(); }
			if (ImGui::MenuItem("Configuration", NULL, configuration->isActive())) { configuration->toggleActive(); }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About", NULL, about->isActive())) { about->toggleActive(); }
			ImGui::Separator();
			if (ImGui::MenuItem("View GIT Repository")) { ShowInBrowser("https://github.com/Rubetoman/SDL-OpenGL-Engine-V2"); }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

const void ModuleEditor::ShowPropertiesWindow()
{
	ImGui::Begin("Properties", &show_properties_window, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_AlwaysVerticalScrollbar);

	if (App->model_loader->meshes.size() > 0)
	{
		ImGui::Text("Current loaded file has %d meshes", App->model_loader->meshes.size());
		ImGui::NewLine();
		int count = 0;
		for (std::vector<ModuleModelLoader::mesh*>::iterator it_m = App->model_loader->meshes.begin(); it_m != App->model_loader->meshes.end(); it_m++)
		{
			ModuleModelLoader::mesh* mesh = (*it_m);
			ImGui::Text("Mesh name: %s", mesh->name.c_str());

			ImGui::PushID("Transformation" + count);
			if (ImGui::CollapsingHeader("Transformation"))
			{
				ImGui::Text("Position:\n X: %f | Y: %f | Z: %f", mesh->position.x, mesh->position.y, mesh->position.z);
				//float3 rot = mesh->rotation.ToEulerXYZ();
				//rot *= 180 / pi;
				//ImGui::Text("Rotation:\n X: %f | Y: %f | Z: %f", rot.x, rot.y, rot.z);
				ImGui::Text("Scale:\n X: %f | Y: %f | Z: %f", mesh->scale.x, mesh->scale.y, mesh->scale.z);
			}
			ImGui::PopID();
			ImGui::PushID("Geometry" + count);
			if (ImGui::CollapsingHeader("Geometry"))
			{
				ImGui::Text("Triangles Count: %d", mesh->num_indices / 3);
				ImGui::Text("Vertices Count: %d", mesh->num_vertices);
				ImGui::Text("Mesh size:\n X: %f | Y: %f | Z: %f", mesh->boundingBox.Size().x, mesh->boundingBox.Size().y, mesh->boundingBox.Size().z);
			}
			ImGui::PopID();
			ImGui::PushID("Texture" + count);
			if (ImGui::CollapsingHeader("Texture"))
			{
				if (mesh->texture != 0)
				{
					ImGui::Text("Texture Size:\n Width: %d | Height: %d", mesh->texWidth, mesh->texHeight);
					float panelWidth = ImGui::GetWindowContentRegionWidth();
					float conversionFactor = panelWidth / mesh->texWidth;
					ImVec2 imageSize = { mesh->texHeight *conversionFactor, panelWidth };
					ImGui::Image((ImTextureID)mesh->texture, imageSize);
				}
				else
					ImGui::Text("No texture");
			}
			ImGui::PopID();

			count++;
			ImGui::NewLine();
		}
	}
	else
		ImGui::Text("No meshes loaded");

	ImGui::End();
}

const void ModuleEditor::ShowInBrowser(const char* url) const
{
	assert(url != nullptr);
	ShellExecute(0, "open", url, 0, 0, SW_SHOW);
}
