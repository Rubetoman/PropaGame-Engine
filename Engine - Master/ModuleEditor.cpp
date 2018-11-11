#include "ModuleEditor.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

#include "mmgr/mmgr.h"

static void ShowMainMenuBar();
static void ShowOptionsWindow();
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
	//Example Window
	ShowMainMenuBar();
	ImGui::ShowDemoWindow();
	if (show_options_window)	{ShowOptionsWindow();}
	if (show_about_window)		{ShowAboutWindow();}
	if (show_app_info_window)	{ShowAppInfoWindow();}
	if (show_textures_window)	{/*ShowTexturesWindow();*/}
	if (show_log_window)		{ShowLogWindow();}
	if (show_properties_window)	{ShowPropertiesWindow();}
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
			if (ImGui::MenuItem("Options", NULL, &App->editor->show_options_window)) { App->editor->show_options_window = true; }
			if (ImGui::MenuItem("Application Info", NULL, &App->editor->show_app_info_window)) { App->editor->show_app_info_window = true; }
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

void ModuleEditor::ShowAppInfoWindow()
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

void ModuleEditor::ShowPropertiesWindow()
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

/*void ModuleEditor::ShowTexturesWindow()
{
	ImGui::Begin("Textures", &App->editor->show_textures_window, ImGuiWindowFlags_AlwaysAutoResize);
	Texture* images[] = { &App->exercise->desatranques, &App->exercise->sankara, &App->exercise->pazos };
	static texture* current_item = images[0];
	if (ImGui::BeginCombo("Loaded textures", current_item->name, ImGuiComboFlags_NoArrowButton))
	{
		for (int n = 0; n < IM_ARRAYSIZE(images); n++)
		{
			bool is_selected = (current_item == images[n]);
			if (ImGui::Selectable(images[n]->name, is_selected) && !is_selected) {
				current_item = images[n];
				App->textures->ReloadTexture(*images[n], App->exercise->quadTexture);
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
					App->textures->ReloadTexture(*current_item, App->exercise->quadTexture);
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
					App->textures->ReloadTexture(*current_item, App->exercise->quadTexture);
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
			App->textures->ReloadTexture(*current_item, App->exercise->quadTexture);
		}
	}
	ImGui::End();
}*/

const void ModuleEditor::ShowInBrowser(const char* url) const
{
	ShellExecute(0, "open", url, 0, 0, SW_SHOW);
}
