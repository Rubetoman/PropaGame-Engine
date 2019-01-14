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

	ImGui::End();
}

void WindowExplorer::DrawTreeNode(const char* name, bool isLeaf) {
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (isLeaf)
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;

	if (itemSelected == name)
		nodeFlags |= ImGuiTreeNodeFlags_Selected;

	bool resourceOpen = ImGui::TreeNodeEx(name, nodeFlags, name);

	//ClickBehaviour(name);

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

			/*for (std::vector<std::string>::iterator iterator = App->library->fileScenesList->begin(); iterator != App->library->fileScenesList->end(); ++iterator) 
			{
				DrawTreeNode((*iterator).c_str(), true);
			}*/
		}

		ImGui::TreePop();
	}
}

bool WindowExplorer::IsFocused() const { return focus; }