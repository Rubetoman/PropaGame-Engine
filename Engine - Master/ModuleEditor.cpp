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

ModuleEditor::ModuleEditor()
{
	editorWindows.push_back(scene = new WindowScene("Scene"));
	editorWindows.push_back(about = new WindowAbout("About"));
	editorWindows.push_back(console = new WindowConsole("console"));
	editorWindows.push_back(performance = new WindowPerformance("performance"));
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


	if (show_textures_window)	{/*ShowTexturesWindow();*/}
	if (show_properties_window)	{ShowPropertiesWindow();}
	if (show_configuration_window) { ShowConfigurationWindow(); }
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
			if (ImGui::MenuItem("Configuration", NULL, &show_configuration_window)) { show_configuration_window = true; }
			//if(ImGui::MenuItem("Texture Options")) { App->editor->show_textures_window = true; }
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

const void ModuleEditor::ShowConfigurationWindow()
{
	ImGui::SetNextWindowSize(ImVec2(350, 500), ImGuiCond_FirstUseEver);
	ImGui::Begin("Configuration", &App->editor->show_configuration_window);   // Pointer to bool variable (close when click on button)
	if (ImGui::CollapsingHeader("Window"))
	{
		//Window size fields (only be able to edit if not in full screen)
		static ImU32 width_step = (ImU32)30;
		static ImU32 height_step = (ImU32)50;
		if (ImGui::InputScalar("Width", ImGuiDataType_U32, &App->window->screen_width, App->window->fullscreen ? NULL : &width_step, NULL, "%u"))
		{
			App->window->SetWindowSize(App->window->screen_width, App->window->screen_height, true);
		}
		if (ImGui::InputScalar("Height", ImGuiDataType_U32, &App->window->screen_height, App->window->fullscreen ? NULL : &height_step, NULL, "%u"))
		{
			App->window->SetWindowSize(App->window->screen_width, App->window->screen_height, true);
		}

		if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen))
			App->window->ToggleFullScreen();
		ImGui::SameLine(110);
		if (ImGui::Checkbox("Resizable", &App->window->resizable))
			App->window->ToggleResizable();
		if (ImGui::Checkbox("VSync", &App->window->vsync))
			App->window->ToggleVSync();
		ImGui::SameLine(110);
		if (ImGui::Checkbox("Borderless", &App->window->borderless))
			App->window->ToggleBorderless();
		ImGui::Separator();

		if (ImGui::SliderFloat("Brightness", &App->window->brightness, 0.0f, 1.0f))
		{
			App->window->SetWindowBrightness(App->window->brightness);
		}
		ImGui::NewLine();
	}
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Text("Camera Position:");
		ImGui::Text("X: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->position.x).c_str());
		ImGui::SameLine(100); ImGui::Text("Y: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->position.y).c_str());
		ImGui::SameLine(200); ImGui::Text("Z: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->position.z).c_str());
		ImGui::NewLine();
		ImGui::Separator();

		// Front, side and up vectors
		ImGui::Text("Camera Vectors:");
		ImGui::Text("Front: ");
		ImGui::Text("X: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->front.x).c_str());
		ImGui::SameLine(100); ImGui::Text("Y: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->front.y).c_str());
		ImGui::SameLine(200); ImGui::Text("Z: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->front.z).c_str());
		ImGui::NewLine();

		ImGui::Text("Side: ");
		ImGui::Text("X: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->side.x).c_str());
		ImGui::SameLine(100); ImGui::Text("Y: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->side.y).c_str());
		ImGui::SameLine(200); ImGui::Text("Z: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->side.z).c_str());
		ImGui::NewLine();

		ImGui::Text("Up: ");
		ImGui::Text("X: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->up.x).c_str());
		ImGui::SameLine(100); ImGui::Text("Y: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->up.y).c_str());
		ImGui::SameLine(200); ImGui::Text("Z: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->up.z).c_str());
		ImGui::NewLine();
		ImGui::Separator();

		ImGui::Text("Camera Vectors:");
		ImGui::Text("Pitch: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->pitch).c_str());
		ImGui::Text("Yaw: "); ImGui::SameLine();
		ImGui::Text(std::to_string(App->camera->mainCamera->yaw).c_str());
		ImGui::NewLine();
		ImGui::Separator();

		ImGui::PushItemWidth(100.0f);
		ImGui::InputFloat("Camera Speed", &App->camera->mainCamera->speed);
		ImGui::InputFloat("Mouse Sensitivity", &App->camera->mouse_sensitivity);
		ImGui::PopItemWidth();
	}
	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("Mouse Position:");
		ImGui::Text("X: %d | Y: %d", App->input->GetMousePosition().x, App->input->GetMousePosition().y);
	}

	if (ImGui::CollapsingHeader("Mesh"))
	{
		if (ImGui::Button("Use Chekers Texture"))
		{
			App->model_loader->ChangeMeshTexture("Checkers_Texture.jpg");
		}
	}
	/*if (ImGui::CollapsingHeader("Textures"))
	{
		ImGui::Text("Loaded textures:");
		for (std::vector<Texture*>::iterator it_m = App->textures->textures.begin(); it_m != App->textures->textures.end(); it_m++)
		{
			Texture* tex = (*it_m);

			if (ImGui::CollapsingHeader(tex->name))
			{
				ImGui::Text("Texture name: %s", tex->name);
				ImGui::Text("Texture path: %s", tex->name);
				ImGui::Text("Texture Size:\n Width: %d | Height: %d", tex->width, tex->height);
				float panelWidth = ImGui::GetWindowContentRegionWidth();
				float conversionFactor = panelWidth / tex->width;
				ImVec2 imageSize = { tex->height *conversionFactor, panelWidth };
				ImGui::Image((ImTextureID)tex->path, imageSize);
			}
			ImGui::NewLine();
		}
	}*/
	ImGui::End();
}

const void ModuleEditor::ShowInBrowser(const char* url) const
{
	assert(url != nullptr);
	ShellExecute(0, "open", url, 0, 0, SW_SHOW);
}
