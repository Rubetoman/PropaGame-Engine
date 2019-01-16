#include "ModuleTextures.h"

#include "Application.h"
#include "ModuleResources.h"
#include "ModuleFileManager.h"

#include "ComponentMaterial.h"

#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilut.h>

ModuleTextures::ModuleTextures()
{
}

// Destructor
ModuleTextures::~ModuleTextures()
{
}

// Called before render is available
bool ModuleTextures::Init()
{
	LOG("Init Image library");
	// Initialize IL
	ilInit();
	// Initialize ILU
	iluInit();
	// Initialize ILUT with OpenGL support.
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return true;
}

void ModuleTextures::LoadTexture(std::string path, unsigned& textureID, int& width, int& height) 
{
	unsigned imageID;

	ilGenImages(1, &imageID);

	ilBindImage(imageID);

	path.insert(0, TEXTURES_FOLDER);

	LOG("Loading material %s", path.c_str());

	char* fileBuffer;
	unsigned lenghBuffer = App->file->Load(path.c_str(), &fileBuffer);

	if (ilLoadL(IL_DDS, fileBuffer, lenghBuffer)) 
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage();

		if (!ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE)) 
		{
			LOG("Error: Image conversion failed %s", iluErrorString(ilGetError()));
			return;
		}

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		width = ilGetInteger(IL_IMAGE_WIDTH);
		height = ilGetInteger(IL_IMAGE_HEIGHT);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), width, height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

		delete[] fileBuffer;
		fileBuffer = nullptr;
	}

	ilDeleteImages(1, &imageID);

	LOG("Material creation successful.");
}

void ModuleTextures::Unload(unsigned id) 
{
	if (id != 0u) 
		glDeleteTextures(1, &id);
}