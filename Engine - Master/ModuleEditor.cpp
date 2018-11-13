#include "ModuleEditor.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

#include "mmgr/mmgr.h"

static void ShowMainMenuBar();
static void ShowAboutWindow();

ModuleEditor::ModuleEditor()
{
	fps_log.resize(100);
	ms_log.resize(100);
	mem_log.resize(100);
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

	ShowMainMenuBar();
	
	//ImGui::ShowDemoWindow();	//Example Window

	if (show_about_window)		{ShowAboutWindow();}
	if (show_app_info_window)	{ShowAppInfoWindow();}
	if (show_textures_window)	{/*ShowTexturesWindow();*/}
	if (show_log_window)		{ShowLogWindow();}
	if (show_properties_window)	{ShowPropertiesWindow();}
	if (show_configuration_window) { ShowConfigurationWindow(); }
	return update;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	Buffer.clear();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

const void ModuleEditor::HandleInputs(SDL_Event& event) {
	ImGui_ImplSDL2_ProcessEvent(&event);
}

static void ShowMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open")) {}
			if (ImGui::MenuItem("Quit", "ALT+F4")) { App->editor->update = UPDATE_STOP; }
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
			if (ImGui::MenuItem("Properties", NULL, &App->editor->show_properties_window)) { App->editor->show_properties_window = true; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Application Info", NULL, &App->editor->show_app_info_window)) { App->editor->show_app_info_window = true; }
			if (ImGui::MenuItem("Configuration", NULL, &App->editor->show_configuration_window)) { App->editor->show_configuration_window = true; }
			//if(ImGui::MenuItem("Texture Options")) { App->editor->show_textures_window = true; }
			if (ImGui::MenuItem("Log", NULL, &App->editor->show_log_window)) { App->editor->show_log_window = true; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About", NULL, &App->editor->show_about_window)) { App->editor->show_about_window = true; }
			ImGui::Separator();
			if (ImGui::MenuItem("View GIT Repository")) { App->editor->ShowInBrowser("https://github.com/Rubetoman/SDL-OpenGL-Engine-V2"); }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

const void ModuleEditor::ShowLogWindow()
{
	ImGui::Begin("Log", &App->editor->show_log_window);   // Pointer to bool variable (close when click on button)
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.9f,0.45f,0.0f,0.7f });
		if (ImGui::Button("Clear"))
		{
			Buffer.clear();
		}
		ImGui::PopStyleColor();


	ImGui::TextUnformatted(Buffer.begin());

	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);

	ScrollToBottom = false;

	ImGui::End();
}

void ModuleEditor::AddLog(const char* logs)
{
	assert(logs != nullptr);
	Buffer.appendf(logs);
	ScrollToBottom = true;
}

static void ShowAboutWindow() 
{
	ImGui::Begin("About", &App->editor->show_about_window);   // Pointer to bool variable (close when click on button)
	ImGui::Text(TITLE);
	ImGui::Text("Version:	BT - 1");
	ImGui::Text("Games are not free of propaganda.");
	ImGui::Text("C/C++ engine made by: ");
	ImGui::NewLine(); ImGui::SameLine(30);
	if (ImGui::Selectable("Ruben Crispin De la Cruz", false, 0, { 165, 13 }))
		App->editor->ShowInBrowser("https://github.com/Rubetoman");
	
	ImGui::NewLine();
	ImGui::Text("Libraries: ");
	
	ImGui::NewLine(); ImGui::SameLine(30);
	std::string glewVersion = (const char*)glewGetString(GLEW_VERSION);
	glewVersion = "Glew " + glewVersion;
	if (ImGui::Selectable(glewVersion.c_str(), false, 0, { 142, 13 }))
		App->editor->ShowInBrowser("http://glew.sourceforge.net/");
	
	ImGui::NewLine(); ImGui::SameLine(30);
	std::string imguiVersion = "ImGui ";
	imguiVersion += ImGui::GetVersion();
	if (ImGui::Selectable(imguiVersion.c_str(), false, 0, { 142, 13 }))
		App->editor->ShowInBrowser("https://github.com/ocornut/imgui");

	ImGui::NewLine(); ImGui::SameLine(30);
	std::string glVersion = (const char*)glGetString(GL_VERSION);
	glVersion = "OpenGL " + glVersion;
	if (ImGui::Selectable(glVersion.c_str(), false, 0, { 310, 13 }))
		App->editor->ShowInBrowser("https://www.opengl.org/");

	ImGui::NewLine(); ImGui::SameLine(30);
	if (ImGui::Selectable("MathGeoLib 1.5.0", false, 0, { 185, 13 }))
		App->editor->ShowInBrowser("https://github.com/juj/MathGeoLib");

	ImGui::NewLine(); ImGui::SameLine(30);
	SDL_version version;
	SDL_GetVersion(&version);
	std::string sdlVersion = "SDL ";
	sdlVersion += std::to_string(version.major) + '.' + std::to_string(version.minor) + '.' + std::to_string(version.patch);
	if (ImGui::Selectable(sdlVersion.c_str(), false, 0, { 135, 13 }))
		App->editor->ShowInBrowser("https://www.libsdl.org/");

	ImGui::NewLine();ImGui::SameLine(30);
	std::string assimpVersion = "Assimp - version ";
	assimpVersion += std::to_string(aiGetVersionMajor()) + '.' + std::to_string(aiGetVersionMinor()) + '.' + std::to_string(aiGetVersionRevision());
	if (ImGui::Selectable(assimpVersion.c_str(), false, 0, { 155, 13 }))
		App->editor->ShowInBrowser("http://www.assimp.org/");

	ImGui::NewLine(); ImGui::SameLine(30);
	std::string devilVersion = "DevIL " + std::to_string(IL_VERSION);
	if (ImGui::Selectable(devilVersion.c_str(), false, 0, { 135, 13 }))
		App->editor->ShowInBrowser("http://openil.sourceforge.net/");

	ImGui::NewLine(); ImGui::SameLine(30);
	if (ImGui::Selectable("mmgr 1.0", false, 0, { 130, 13 }))
		App->editor->ShowInBrowser("http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml");

	// License
	ImGui::NewLine();
	ImGui::Text("MIT License Copyright (c) [2018] [Ruben Crispin]");

	// Close button
	ImGui::NewLine();
	if (ImGui::Button("Close"))
		App->editor->show_about_window = false;
	ImGui::End();
}

const void ModuleEditor::ShowAppInfoWindow()
{
	ImGui::SetNextWindowSize(ImVec2(350,500), ImGuiCond_FirstUseEver);
	ImGui::Begin("Application Info", &App->editor->show_app_info_window);   // Pointer to bool variable (close when click on button)
	if (ImGui::CollapsingHeader("Performance"))
	{
		// FPS & Miliseconds for each frame
		char title[35];
		sprintf_s(title, 25, "Framerate %0.1f", fps_log[fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 200.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));

		// Memory stats
		sMStats stats = m_getMemoryStatistics();
		mem_log.erase(mem_log.begin());
		mem_log.push_back((float)stats.totalReportedMemory);
		sprintf_s(title, 35, "Memory Consumption (Bytes) %0.1f", mem_log[mem_log.size() - 1]);
		ImGui::PlotHistogram("##memory", &mem_log[0], mem_log.size(), 0, title, 0.0f, (float)stats.peakReportedMemory * 1.2f, ImVec2(310, 100));
		ImGui::Text("Total Reported Mem: %u", stats.totalReportedMemory);
		ImGui::Text("Total Actual Mem: %u", stats.totalActualMemory);
		ImGui::Text("Peak Reported Mem: %u", stats.peakReportedMemory);
		ImGui::Text("Peak Actual Mem: %u", stats.peakActualMemory);
		ImGui::Text("Accumulated Reported Mem: %u", stats.accumulatedReportedMemory);
		ImGui::Text("Accumulated Actual Mem: %u", stats.accumulatedActualMemory);
		ImGui::Text("Accumulated Alloc Unit Count: %u", stats.accumulatedAllocUnitCount);
		ImGui::Text("Total Alloc Unit Count: %u", stats.totalAllocUnitCount);
		ImGui::Text("Peak Alloc Unit Count: %u", stats.peakAllocUnitCount);
	}
	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("CPUs:");
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, { 1,1,0,1 });
		ImGui::Text("%d (Cache: %dB)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
		ImGui::PopStyleColor();

		ImGui::Text("System RAM:");
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, { 1,1,0,1 });
		ImGui::Text("%dGb", SDL_GetSystemRAM() / 1000);
		ImGui::PopStyleColor();

		ImGui::Text("Caps:");
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, { 1,1,0,1 });
		std::string caps;
		if (SDL_Has3DNow())
			caps += "3DNow!, ";
		if (SDL_HasAVX())
			caps += "AVX, ";
		if (SDL_HasAVX2())
			caps += "AVX2, ";
		if (SDL_HasAltiVec())
			caps += "AltiVec, ";
		if (SDL_HasMMX())
			caps += "MMX, ";
		if (SDL_HasRDTSC())
			caps += "RDTSC, ";
		if (caps.size() > 5)
			caps += "\n";
		if (SDL_HasSSE())
			caps += "SSE, ";
		if (SDL_HasSSE2())
			caps += "SSE2, ";
		if (SDL_HasSSE3())
			caps += "SSE3, ";
		if (SDL_HasSSE41())
			caps += "SSE41, ";
		if (SDL_HasSSE42())
			caps += "SSE42, ";
		ImGui::Text(caps.c_str());
		ImGui::PopStyleColor();
	}
	ImGui::End();
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
