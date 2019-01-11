#ifndef __MATERIALIMPORTER_H__
#define __MATERIALIMPORTER_H__

#include "Importer.h"
#include <string>

class MaterialImporter : public Importer
{
public:
	MaterialImporter();
	~MaterialImporter();

	bool Import(const char* file, const char* path, std::string& output_file) override = 0;

	void ImportMaterial(const char* path, std::string& output_file);
};

#endif /*__MATERIALIMPORTER_H__*/