#include "TextureImporter.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleFileManager.h"
#include "ModuleResources.h"

TextureImporter::TextureImporter()
{
}

TextureImporter::~TextureImporter()
{
}

void TextureImporter::Import(const char* path)
{
	bool result = false;

	char* fileBuffer = nullptr;
	unsigned lenghBuffer = App->file->Load(path, &fileBuffer);

	if (fileBuffer) {
		ILuint ImageName;
		ilGenImages(1, &ImageName);
		ilBindImage(ImageName);

		if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)fileBuffer, lenghBuffer)) 
		{
			ilEnable(IL_FILE_OVERWRITE);
			ILuint   size;
			ILubyte *data;
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
			size = ilSaveL(IL_DDS, NULL, 0);

			if (size > 0) 
			{
				data = new ILubyte[size];
				if (ilSaveL(IL_DDS, data, size) > 0) 
				{
					std::string fileName;
					std::string fileExtension;
					App->file->SplitPath(path, nullptr, &fileName, &fileExtension);

					fileName.insert(0, TEXTURES_FOLDER);
					fileName.append(".proDDS");

					result = App->file->Save(fileName.c_str(), data, size, false);
				}

				delete[] data;
				data = nullptr;
			}

			ilDeleteImages(1, &ImageName);
		}
	}
	App->resources->UpdateTexturesList();
}