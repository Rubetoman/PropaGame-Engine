#ifndef __GLOBALS_H__
#define __GLOBALS_H__
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};
// Deletes a buffer
#define RELEASE( x ) \
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

// Configuration -----------
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 720
#define FULLSCREEN false
#define VSYNC true
#define RESIZABLE true
#define BORDERLESS false
#define BRIGHTNESS 1.0f
#define TITLE "PropaGame Engine"

// PATHS -----------------
#define ASSETS_FOLDER "Assets/"
#define MODELS_ASSETS_FOLDER "Assets/Models/"
#define TEXTURES_ASSETS_FOLDER "Assets/Textures/"

#define LIBRARY_FOLDER "Library/"
#define SCENES_FOLDER "Library/Scenes/"
#define TEXTURES_FOLDER "Library/Textures/"
#define MESHES_FOLDER "Library/Meshes/"
#define MATERIALS_FOLDER "Library/Materials/"

// TEXTURES --------------
#define CHECKERS_TEXTURE "Assets/Textures/Checkers_Texture.jpg"
#define CAMERA_TEXTURE "Assets/Textures/No_Camera.jpg"

// CUSTOM EXTENSIONS -----
#define SCENES_EXTENSION ".proScene"

#endif //__GLOBALS_H__