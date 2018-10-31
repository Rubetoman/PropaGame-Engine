#include "ModuleTextures.h"

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
	return true;
}

GLuint ModuleTextures::loadImage(image image)
{
	ILuint imageID;				// Create an image ID as a ULuint
	ILboolean success;			// Create a flag to keep track of success/failure

	ilGenImages(1, &imageID); 		// Generate the image ID
	ilBindImage(imageID); 			// Bind the image

	if (ilLoadImage(image.path))
	{
		GLuint textureID = 0;							// Create a texture ID as a GLuint
		glGenTextures(1, &textureID);					// Generate a new texture
		glBindTexture(GL_TEXTURE_2D, textureID);		// Bind the texture to a name

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
		image.width = ilGetInteger(IL_IMAGE_WIDTH);
		image.height = ilGetInteger(IL_IMAGE_HEIGHT);
		switch (ImageInfo.Format)
		{
			case IL_COLOUR_INDEX: image.format = "Colour_index"; break;
			case IL_RGB: image.format = "RGB"; break;
			case IL_RGBA: image.format = "RGBA"; break;
			case IL_BGR: image.format = "BGR"; break;
			case IL_BGRA: image.format = "BGRA"; break;
			case IL_LUMINANCE: image.format = "Luminance"; break;
			default: image.format = "Unknown"; break;
		}

		// Set smaller resize mode
		if (image.use_mipmap)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
			glGenerateTextureMipmap(textureID);
		}

		// Set texture clamping method
		switch (image.wrap_mode)
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
		switch (image.resize_mode) 
		{
		case nearest:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			/*if(!image.use_mipmap)*/ glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			break;
		default:
		case linear:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			/*if (!image.use_mipmap)*/ glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

		glBindTexture(GL_TEXTURE_2D, 0);
		ilDeleteImages(1, &imageID);		// Because we have already copied image data into texture data we can release memory used by image.

		LOG("Texture creation successful.");
		return textureID;					// Return the GLuint to the texture so you can use it!
	}
	 
	// If we failed to open the image file in the first place...
	LOG("Image load failed");
	return 0;
}

void ModuleTextures::unloadImage(unsigned id)
{
	if (id != 0)
	{
		glDeleteTextures(1, &id);
	}
}
