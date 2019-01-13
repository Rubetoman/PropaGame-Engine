#ifndef __MATERIALIMPORTER_H__
#define __MATERIALIMPORTER_H__

#include <string>

class MaterialImporter
{
public:
	MaterialImporter();
	~MaterialImporter();

	static void Import(const char* path);
};

#endif /*__MATERIALIMPORTER_H__*/