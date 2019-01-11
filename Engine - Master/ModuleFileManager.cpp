#include "ModuleFileManager.h"

#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleTextures.h"

#include <assert.h>
#include <fstream>

ModuleFileManager::ModuleFileManager()
{
}


ModuleFileManager::~ModuleFileManager()
{
}

void ModuleFileManager::manageFile(char* path)
{
	assert(path != nullptr);

	std::string extension = getFileExtension(path);

	if (extension == "fbx" || extension == "FBX")
	{
		App->model_loader->CleanUp();
		App->model_loader->LoadMesh(path);
	}
	else if (extension == "png" || extension == "dds" || extension == "jpg" || extension == "tif")
	{
		App->textures->loadTexture(path, true);
	}
	else
	{
		LOG("File format not supported");
	}
}

std::string ModuleFileManager::getFileExtension(const char* path)
{
	assert(path != nullptr);
	std::string filename = path;
	std::string extension = path;
	splitPath(path, nullptr, &filename, &extension);
	return extension;
}

void ModuleFileManager::splitPath(const char* full_path, std::string* path, std::string* filename, std::string* extension)
{
	std::string str = normalizePath(full_path);
	unsigned pos_slash = str.find_last_of('/');
	unsigned pos_dot = str.find_last_of('.');

	if (path != nullptr)
	{
		if (pos_slash < str.length())
			*path = str.substr(0, pos_slash + 1);
		else
			path->clear();
	}
	if (filename != nullptr)
	{
		if (pos_slash < str.length())
			*filename = str.substr(pos_slash + 1, pos_dot - pos_slash - 1);
		else
			*filename = str.substr(0, pos_dot);
	}
	if (extension != nullptr)
	{
		if (pos_dot < str.length())
			*extension = str.substr(pos_dot + 1);
		else
			extension->clear();
	}
}

std::string ModuleFileManager::normalizePath(const char* path)
{
	assert(path != nullptr);

	std::string str = path;

	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == '\\')
			str[i] = '/';
	}

	return str.c_str();
}

std::string ModuleFileManager::getFullPath(const char* path, const char* atDirectory, const char* withExtension)
{
	std::string full_path = path;
	if (atDirectory != nullptr || withExtension != nullptr)
		App->file->splitPath(path, nullptr, &full_path, nullptr);

	if (atDirectory != nullptr)
		full_path = atDirectory + full_path;
	if (withExtension != nullptr)
		full_path += withExtension;

	return full_path.c_str();
}

bool ModuleFileManager::SaveFileData(const char* data, unsigned int size, std::string& output_file)
{
	std::ofstream outfile;
	outfile.open(output_file, std::ios::binary | std::ios::out);
	outfile.write(data, size);
	RELEASE_ARRAY(data);
	outfile.close();
	return true;
}

bool ModuleFileManager::LoadFileData(std::string& myfile, char* &data)
{
	std::ifstream file(myfile, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	data = new char[size];
	if (file.read(data, size))
	{
		return true;
	}
	return false;
}