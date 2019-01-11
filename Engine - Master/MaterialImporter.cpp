#include "MaterialImporter.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleFileManager.h"

MaterialImporter::MaterialImporter()
{
}

MaterialImporter::~MaterialImporter()
{
}

void MaterialImporter::ImportMaterial(const char* path, std::string& output_file)
{
	ILuint imageID;
	ILboolean success;
	ILenum error;

	ilGenImages(1, &imageID); 		// Generate the image ID
	ilBindImage(imageID); 			// Bind the image
	success = ilLoadImage(path);
	if (success)
	{

		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}
		ILuint size;
		ILubyte* data = ilGetData();
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
		size = ilSaveL(IL_DDS, NULL, 0);	// Get the size of the data buffer
		data = new ILubyte[size];// allocate data buffer
		if (ilSaveL(IL_DDS, data, size) > 0)
		{
			// Save to buffer with the ilSaveIL function
			App->file->SaveFileData((char*)data, size, output_file);
		}
		RELEASE_ARRAY(data);
	}
}