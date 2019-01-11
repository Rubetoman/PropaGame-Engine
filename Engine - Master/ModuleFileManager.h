#ifndef __MODULEFILEMANAGEMENT_H__
#define __MODULEFILEMANAGEMENT_H__

#include "Module.h"

#include <string>

class ModuleFileManager: public Module
{
public:
	ModuleFileManager();
	~ModuleFileManager();

	void manageFile(const char* path);
	std::string getFileExtension(const char* path);
	void splitPath(const char* full_path, std::string* path, std::string* filename, std::string* extension);
	std::string normalizePath(const char * path);
	std::string getFullPath(const char* path, const char* atDirectory, const char* withExtension);
};

#endif // __MODULEFILEMANAGEMENT_H__

