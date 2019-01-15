#include "WindowExplorer.h"

#include "Application.h"
#include "ModuleResources.h"
#include "ModuleFileManager.h"

#include <vector>

WindowExplorer::WindowExplorer(const char* name) : Window(name)
{
	active = true;
}


WindowExplorer::~WindowExplorer()
{
}

void WindowExplorer::Draw() {

	if (!ImGui::Begin("Explorer", &active)) 
	{
		ImGui::End();
		return;
	}

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
		/*else if (name == "Assets/Textures")
		{
			for (std::vector<std::string>::iterator iterator = App->resources->fbx_meshes->begin(); iterator != App->resources->fbx_meshes->end(); ++iterator)
			{
				DrawTreeNode((*iterator).c_str(), true);
			}
		}*/

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

	if (itemSelected == name) {
		if (ImGui::BeginPopup("ContextMenu")) 
		{

			if (ImGui::MenuItem("Delete")) 
			{
				std::string ext;
				App->file->SplitPath(name, nullptr, nullptr, &ext);
				std::string nameToRemove = name;

				if (ext == "proMesh") 
				{
					nameToRemove.insert(0, "/Library/Meshes/");
					App->resources->removeMesh = true;
					App->resources->itemToDelete = true;
				}
				else if (ext == "proDDS") 
				{
					nameToRemove.insert(0, "/Library/Textures/");
					App->resources->removeMesh = false;
					App->resources->itemToDelete = true;
				}
				App->file->Remove(nameToRemove.c_str());
			}
			ImGui::EndPopup();
		}
	}
}

bool WindowExplorer::IsFocused() const { return focus; }