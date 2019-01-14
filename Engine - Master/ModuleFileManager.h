#ifndef __MODULEFILEMANAGEMENT_H__
#define __MODULEFILEMANAGEMENT_H__

#include "Module.h"

#include <string>
#include <vector>

class ModuleFileManager: public Module
{
public:
	ModuleFileManager();
	~ModuleFileManager();

	bool Init() override;
	bool CleanUp() override;

	bool Exists(const char* pathAndFileName) const;

	bool MakeDirectory(const char* path);
	bool AddPath(const char* path);
	bool IsDirectory(const char* pathAndFileName) const;
	void GetFilesFromDirectory(const char* directory, std::vector<std::string>& fileList) const;

	void manageFile(char* path);

	void LoadMeshFileToScene(const char& path);

	std::string getFileExtension(const char* path);
	void SplitPath(const char* full_path, std::string* path, std::string* filename, std::string* extension);
	std::string NormalizePath(const char * path);
	std::string getFullPath(const char* path, const char* atDirectory, const char* withExtension);

	// Files
	unsigned Save(const char* pathAndFileName, const void* buffer, unsigned size, bool append) const;
	unsigned Load(const char* pathAndFileName, char** buffer) const;

public:
	char* basePath = nullptr;
};

#endif // __MODULEFILEMANAGEMENT_H__

