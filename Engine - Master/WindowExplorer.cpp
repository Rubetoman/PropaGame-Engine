#include "WindowExplorer.h"

#include "Application.h"
#include "ModuleResources.h"
#include "ModuleFileManager.h"
#include "ModuleModelLoader.h"
#include "ModuleScene.h"

#include "MeshImporter.h"
#include "TextureImporter.h"

#include <vector>

WindowExplorer::WindowExplorer(const char* name) : Window(name)
{
	active = true;
}


WindowExplorer::~WindowExplorer()
{
}

void WindowExplorer::Draw() {

	if (!ImGui::Begin("Explorer", &active, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_MenuBar))
	{
		ImGui::End();
		return;
	}

	ImGui::BeginMenuBar();
	ImGui::PushStyleColor(ImGuiCol_Button, { 0.9f,0.45f,0.0f,0.7f });
	if (ImGui::Button("Refresh"))
	{
		App->resources->UpdateFilesList();
	}
	ImGui::PopStyleColor();
	ImGui::EndMenuBar();

	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	bool libraryOpen = ImGui::TreeNodeEx("Library", node_flags, "Library");

	if (libraryOpen) 
	{
		DrawTreeNode("Meshes", false);
		DrawTreeNode("Textures", false);
		DrawTreeNode("Scenes", false);

		ImGui::TreePop();
	}

	bool assetsOpen = ImGui::TreeNodeEx("Assets", node_flags, "Assets");

	if (assetsOpen)
	{
		DrawTreeNode("Assets/Meshes", false);
		DrawTreeNode("Assets/Textures", false);

		ImGui::TreePop();
	}

	ImGui::End();
}

void WindowExplorer::DrawTreeNode(const char* name, bool isLeaf) 
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (isLeaf)
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;

	if (itemSelected == name)
		nodeFlags |= ImGuiTreeNodeFlags_Selected;

	bool resourceOpen = ImGui::TreeNodeEx(name, nodeFlags, name);

	if(isLeaf)
		ClickBehaviour(name);

	if (resourceOpen) 
	{
		if (name == "Meshes") 
		{
			for (std::vector<std::string>::iterator iterator = App->resources->file_meshes->begin(); iterator != App->resources->file_meshes->end(); ++iterator)
			{
				DrawTreeNode((*iterator).c_str(), true);
			}
		}
		else if (name == "Textures") 
		{
			for (std::vector<std::string>::iterator iterator = App->resources->file_textures->begin(); iterator != App->resources->file_textures->end(); ++iterator) 
			{
				DrawTreeNode((*iterator).c_str(), true);
			}
		}
		else if (name == "Scenes") 
		{
			for (std::vector<std::string>::iterator iterator = App->resources->file_scenes->begin(); iterator != App->resources->file_scenes->end(); ++iterator)
			{
				DrawTreeNode((*iterator).c_str(), true);
			}
		}
		else if (name == "Assets/Meshes")
		{
			for (std::vector<std::string>::iterator iterator = App->resources->fbx_meshes->begin(); iterator != App->resources->fbx_meshes->end(); ++iterator)
			{
				DrawTreeNode((*iterator).c_str(), true);
			}
		}
		else if (name == "Assets/Textures")
		{
			for (std::vector<std::string>::iterator iterator = App->resources->external_textures->begin(); iterator != App->resources->external_textures->end(); ++iterator)
			{
				DrawTreeNode((*iterator).c_str(), true);
			}
		}
		ImGui::TreePop();
	}
}

void WindowExplorer::ClickBehaviour(const char* name) 
{
	if (ImGui::IsItemClicked(0))
		itemSelected = name;

	if (ImGui::IsItemClicked(1)) 
	{
		itemSelected = name;
		ImGui::OpenPopup("ContextMenu");
	}

	if (itemSelected == name) 
	{
		if (ImGui::BeginPopup("ContextMenu")) 
		{
			std::string ext;
			App->file->SplitPath(name, nullptr, nullptr, &ext);

			if (ext == "fbx" || ext == "FBX")
			{
				std::string path = MODELS_ASSETS_FOLDER;
				path += name;
				if (ImGui::MenuItem("Import"))
				{
					MeshImporter::ImportFBX(path.c_str());
				}
			}
			else if (ext == "png" || ext == "dds" || ext == "jpg" || ext == "tif" || ext == "tga")
			{
				std::string path = TEXTURES_ASSETS_FOLDER;
				path += name;
				if (ImGui::MenuItem("Import"))
				{
					TextureImporter::Import(path.c_str());
				}
			}
			else if (ext == "proMesh")
			{
				if (ImGui::MenuItem("Load Mesh"))
				{
					App->model_loader->CreateGameObjectFromFile(*name);
				}
				if (ImGui::MenuItem("Delete"))
				{
					std::string nameToRemove = name;
					nameToRemove.insert(0, MESHES_FOLDER);
					App->resources->meshes_dirty = true;
					App->file->Remove(nameToRemove.c_str());
				}
			}
			else if (ext == "proDDS")
			{
				if (ImGui::MenuItem("Delete"))
				{
					std::string nameToRemove = name;
					nameToRemove.insert(0, TEXTURES_FOLDER);
					App->resources->textures_dirty = true;
					App->file->Remove(nameToRemove.c_str());
				}
			}
			else if (ext == "proScene")
			{
				if (ImGui::MenuItem("Delete"))
				{
					App->scene->DeleteScene(name);
					App->resources->scenes_dirty = true;
				}
			}
			ImGui::EndPopup();
		}
	}
}

bool WindowExplorer::IsFocused() const { return focus; }