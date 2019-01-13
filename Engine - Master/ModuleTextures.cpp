#include "ModuleTextures.h"

#include "Application.h"
#include "ModuleResources.h"

#include <assert.h>

using namespace std;

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

// Called before quitting
bool ModuleTextures::CleanUp()
{
	/*for (auto it_m = App->resources->textures.begin(); it_m != App->resources->textures.end();)
	{
		glDeleteTextures(1, &it_m->first->id);
		delete it_m->first;
		App->resources->textures.erase(it_m++);
	}*/
	return true;
}


/*Texture* ModuleTextures::loadTexture(const char* path, bool permanent)
{
	assert(path != nullptr);

	ILuint imageID;				// Create an image ID as a ULuint
	ILboolean success;			// Create a flag to keep track of success/failure

	ilGenImages(1, &imageID); 		// Generate the image ID
	ilBindImage(imageID); 			// Bind the image

	Texture* nTexture = new Texture();

	// SOLVE THIS TEMP VAR COPY
	std::string name = nTexture->name;
	std::string extension = nTexture->extension;

	App->file->SplitPath(path, nullptr, &name, &extension);

	if(PATH_SIZE < sizeof(path))
	{ 
		LOG("Error, loading texture, path too long"); 
		return nTexture;
	}
	strcpy_s(nTexture->path, PATH_SIZE, path);
	
	if (NAME_SIZE < sizeof(name.c_str()))
	{
		LOG("Error, loading texture, name too long");
		return nTexture;
	}
	strcpy_s(nTexture->name, NAME_SIZE, name.c_str());

	if (NAME_SIZE < sizeof(extension.c_str())) 
	{
		LOG("Error, loading texture, extension too long"); 
		return nTexture;
	}
	strcpy_s(nTexture->extension, EXTEN_SIZE, extension.c_str());


	if (!ilLoadImage(path))
	{
		// If we failed to open the image file in the first place...
		LOG("Image couldn't be found on %s, looking on Assets/Models/", path);
		char * newPath = (char *)malloc(1 + strlen("Assets/Models/") + strlen(nTexture->name) + strlen(".") + strlen(nTexture->extension));
		strcpy(newPath, "Assets/Models/");
		strcat(newPath, nTexture->name);
		strcat(newPath, ".");
		strcat(newPath, nTexture->extension);

		if (!ilLoadImage(newPath))
		{
			LOG("Image couldn't be found on %s, looking on Assets/Models/textures/", newPath);
			newPath = (char *)malloc(1 + strlen("Assets/Models/textures/") + strlen(nTexture->name) + strlen(".") + strlen(nTexture->extension));
			strcpy(newPath, "Assets/Models/textures/");
			strcat(newPath, nTexture->name);
			strcat(newPath, ".");
			strcat(newPath, nTexture->extension);

			if (!ilLoadImage(newPath))
			{
				LOG("Image couldn't be found");
				free(newPath);
				return nTexture;
			}
		}
		strcpy_s(nTexture->path, PATH_SIZE, newPath);
		free(newPath);
	}

	// Check the texture wasn't already loaded
	for (std::map<Texture*, int>::iterator it_m = App->resources->textures.begin(); it_m != App->resources->textures.end(); ++it_m)
	{
		if (strcmp((it_m)->first->path, nTexture->path) == 0)
		{
			LOG("%s already loaded.", nTexture->path);
			++it_m->second;
			delete nTexture;
			return it_m->first;
		}
	}

	GLuint textureID = 0;							// Create a texture ID as a GLuint
	glGenTextures(1, &textureID);					// Generate a new texture
	glBindTexture(GL_TEXTURE_2D, textureID);		// Bind the texture to a name

	nTexture->id = textureID;

	// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}

	// Convert the image into a suitable format to work with
	// NOTE: If the image contains alpha channel you can use IL_RGBA, if not use IL_RGB
	int channels = ilGetInteger(IL_IMAGE_CHANNELS);
	if (channels == 3)
	{
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	}
	else if (channels == 4) 
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	}

	// Quit out if we failed the conversion
	if (!success)
	{
		LOG("Image conversion failed");
		exit(-1);
	}

	ILubyte* data = ilGetData();
	// Asign parameters to image
	nTexture->width = ilGetInteger(IL_IMAGE_WIDTH);
	nTexture->height = ilGetInteger(IL_IMAGE_HEIGHT);
	switch (ImageInfo.Format)
	{
		case IL_COLOUR_INDEX: nTexture->format = "Colour_index"; break;
		case IL_RGB: nTexture->format = "RGB"; break;
		case IL_RGBA: nTexture->format = "RGBA"; break;
		case IL_BGR: nTexture->format = "BGR"; break;
		case IL_BGRA: nTexture->format = "BGRA"; break;
		case IL_LUMINANCE: nTexture->format = "Luminance"; break;
		default: nTexture->format = "Unknown"; break;
	}

	// Set texture clamping method
	switch (nTexture->wrap_mode)
	{
	case clamp_to_boder:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
		break;
	case repeat:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		break;
	case mirrored:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
		break;
	default:
	case clamp:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);
		break;
	}

	// Set texture interpolation method to use linear interpolation (no MIPMAPS)
	switch (nTexture->resize_mode)
	{
	case nearest:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if(!nTexture->use_mipmap) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		break;
	default:
	case linear:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (!nTexture->use_mipmap) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	// Specify the texture specification
	glTexImage2D(GL_TEXTURE_2D, 		// Type of texture
		0,								// Pyramid level (for mip-mapping) - 0 is the top level
		ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
		ilGetInteger(IL_IMAGE_WIDTH),	// Image width
		ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
		0,								// Border width in pixels (can either be 1 or 0)
		ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
		GL_UNSIGNED_BYTE,				// Image data type
		ilGetData());					// The actual image data itself
	
	// Set smaller resize mode
	if (nTexture->use_mipmap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	ilDeleteImages(1, &imageID);		// Because we have already copied image data into texture data we can release memory used by image.
	glBindTexture(GL_TEXTURE_2D, textureID);

	if(permanent)
		App->resources->textures.insert(std::pair<Texture*, unsigned>(nTexture, -1));
	else
		App->resources->textures.insert(std::pair<Texture*, unsigned>(nTexture,1));

	LOG("Texture %s created successfully.", nTexture->name);
	return nTexture;					// Return the GLuint to the texture so you can use it!
}*/

/*bool ModuleTextures::unloadTexture(Texture* texture)
{
	bool deleted = false;
	if (texture != nullptr)
	{
		for (auto it_m = App->resources->textures.begin(); it_m != App->resources->textures.end();)
		{
			if (it_m->first->id == texture->id)
			{
				// Avoid deleting permanent textures
				if (it_m->second < 0)
					return false;

				// Check if the texture is used by another GO
				if (it_m->second < 2)
				{
					App->resources->textures.erase(it_m++);
					deleted = true;
					break;
				}
				else
				{
					--it_m->second;
					return false;
				}
			}
			else
			{
				++it_m;
			}
		}
		glDeleteTextures(1, &texture->id);
		delete texture;
	}
	return deleted;
}

void ModuleTextures::ReloadTexture(Texture& new_texture, Texture& texture) 
{
	glDeleteTextures(1, &texture.id);

	texture = loadTexture(new_texture.path);

	if (texture == -1) 
	{
		LOG("Error: Texture cannot be loaded");
	}

}*/

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
	}

	ilDeleteImages(1, &imageID);
	LOG("Material creation successful.");
}

void ModuleTextures::Unload(unsigned id) 
{
	if (id != 0u) 
		glDeleteTextures(1, &id);
}