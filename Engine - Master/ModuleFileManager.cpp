#include "ModuleFileManager.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleTextures.h"

#include "MaterialImporter.h"

#include "SDL.h"
#include "physfs.h"
#include <assert.h>
#include <fstream>

ModuleFileManager::ModuleFileManager()
{
}


ModuleFileManager::~ModuleFileManager()
{
}

bool ModuleFileManager::Init()
{
	basePath = SDL_GetPrefPath("PropaGame", "");
	PHYSFS_init(basePath);
	SDL_free(basePath);

	AddPath(".");

	AddPath("../Game/");

	PHYSFS_setWriteDir(".");

	if (!Exists(ASSETS_FOLDER)) 
		PHYSFS_mkdir(ASSETS_FOLDER);

	if (!Exists(TEXTURES_FOLDER))
		PHYSFS_mkdir(TEXTURES_FOLDER);

	if (!Exists(MESHES_FOLDER))
		PHYSFS_mkdir(MESHES_FOLDER);

	if (!Exists(SCENES_FOLDER))
		PHYSFS_mkdir(SCENES_FOLDER);

	if (!Exists(MATERIALS_FOLDER))
		PHYSFS_mkdir(MATERIALS_FOLDER);

	return true;
}

bool ModuleFileManager::CleanUp() 
{
	PHYSFS_deinit();
	return true;
}

bool ModuleFileManager::Exists(const char* pathAndFileName) const
{
	return PHYSFS_exists(pathAndFileName) != 0;
}

bool ModuleFileManager::MakeDirectory(const char* path)
{
	return PHYSFS_mkdir(path) != 0;
}

bool ModuleFileManager::AddPath(const char* path)
{
	bool result = false;

	if (PHYSFS_mount(path, nullptr, 1) == 0) 
	{
		LOG("Error adding a path: %s\n", PHYSFS_getLastError());
	}
	else {
		result = true;
	}

	return result;
}

bool ModuleFileManager::IsDirectory(const char* pathAndFileName) const
{
	return PHYSFS_isDirectory(pathAndFileName) != 0;
}

void ModuleFileManager::GetFilesFromDirectory(const char* directory, std::vector<std::string>& fileList) const
{
	char **enumeratedFIles = PHYSFS_enumerateFiles(directory);
	char **iterator;

	std::string dir(directory);

	for (iterator = enumeratedFIles; *iterator != nullptr; iterator++) 
	{
		if (!PHYSFS_isDirectory((dir + *iterator).c_str()))
			fileList.push_back(*iterator);
	}

	PHYSFS_freeList(enumeratedFIles);
}

void ModuleFileManager::manageFile(char* path)
{
	assert(path != nullptr);

	std::string extension = getFileExtension(path);

	if (extension == "fbx" || extension == "FBX")
	{
		//App->model_loader->LoadMesh(path);
	}
	else if (extension == "png" || extension == "dds" || extension == "jpg" || extension == "tif")
	{
		//App->textures->loadTexture(path, true);
		TextureImporter::Import(path);
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
	SplitPath(path, nullptr, &filename, &extension);
	return extension;
}

void ModuleFileManager::SplitPath(const char* full_path, std::string* path, std::string* filename, std::string* extension)
{
	std::string str = NormalizePath(full_path);
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

std::string ModuleFileManager::NormalizePath(const char* path)
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
		App->file->SplitPath(path, nullptr, &full_path, nullptr);

	if (atDirectory != nullptr)
		full_path = atDirectory + full_path;
	if (withExtension != nullptr)
		full_path += withExtension;

	return full_path.c_str();
}

unsigned ModuleFileManager::Save(const char* pathAndFileName, const void* buffer, unsigned size, bool append) const
{
	unsigned result = 0u;

	PHYSFS_file* fsFile = (append) ? PHYSFS_openAppend(pathAndFileName) : PHYSFS_openWrite(pathAndFileName);

	if (fsFile != nullptr)
	{
		unsigned written = (unsigned)PHYSFS_write(fsFile, (const void*)buffer, 1, size);

		if (written == size)
		{
			result = written;
			LOG("File %s saved successfully", pathAndFileName);
		}
		else
		{
			LOG("Error while writing to file %s: %s", pathAndFileName, PHYSFS_getLastError());
		}

		if (PHYSFS_close(fsFile) == 0)
			LOG("Error closing file %s: %s", pathAndFileName, PHYSFS_getLastError());
	}
	else
	{
		LOG("Error opening file %s: %s", pathAndFileName, PHYSFS_getLastError());
	}

	return result;
}

unsigned ModuleFileManager::Load(const char* pathAndFileName, char** buffer) const
{
	unsigned result = 0u;

	PHYSFS_file* fsFile = PHYSFS_openRead(pathAndFileName);

	if (fsFile != nullptr)
	{
		PHYSFS_sint32 size = (PHYSFS_sint32)PHYSFS_fileLength(fsFile);

		if (size > 0)
		{
			*buffer = new char[size];
			unsigned readed = (unsigned)PHYSFS_read(fsFile, *buffer, 1, size);

			if (readed != size)
			{
				LOG("Error reading from file %s: %s\n", pathAndFileName, PHYSFS_getLastError());
				delete[] buffer;
				buffer = nullptr;
			}
			else
			{
				result = readed;
			}
		}

		if (PHYSFS_close(fsFile) == 0)
			LOG("Error closing file %s: %s\n", pathAndFileName, PHYSFS_getLastError());
	}
	else
	{
		const char* error = PHYSFS_getLastError();
		LOG("Error opening file %s: %s\n", pathAndFileName, error);
	}

	return result;
}