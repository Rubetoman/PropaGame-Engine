#ifndef __MATERIALIMPORTER_H__
#define __MATERIALIMPORTER_H__

#include <string>

class TextureImporter
{
public:
	TextureImporter();
	~TextureImporter();

	static void Import(const char* path);
};

#endif /*__MATERIALIMPORTER_H__*/