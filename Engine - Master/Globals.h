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

// Transform degrees to rads
#define degreesToRadians(x) x*(3.141592f/180.0f)

// Configuration -----------
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FULLSCREEN false
#define VSYNC true
#define RESIZABLE true
#define BORDERLESS false
#define SCREEN_SIZE 2
#define BRIGHTNESS 1.0f
#define TITLE "PropaGame Engine"

#endif //__GLOBALS_H__