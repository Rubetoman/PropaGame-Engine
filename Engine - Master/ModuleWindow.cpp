#include "ModuleWindow.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"

#include "ComponentCamera.h"

#include "GL/glew.h"

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		screen_width = SCREEN_WIDTH;
		screen_height = SCREEN_HEIGHT;
		//SetWindowSize(screen_width, screen_height, true);

		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;
		
		//Set window settings
		if(fullscreen) flags |= SDL_WINDOW_FULLSCREEN;
		if(resizable) flags |= SDL_WINDOW_RESIZABLE;
		if (borderless) flags |= SDL_WINDOW_BORDERLESS;
		SDL_GL_SetSwapInterval(vsync);

		std::string windowTitle = "Untitled* - ";
		windowTitle += TITLE;
		window = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, flags);

		if(window == nullptr)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != nullptr)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::ToggleFullScreen() 
{
	Uint32 fullscreenFlag = SDL_WINDOW_FULLSCREEN;
	bool isFullscreen = SDL_GetWindowFlags(window) & fullscreenFlag;
	SDL_SetWindowFullscreen(window, isFullscreen ? 0 : fullscreenFlag);
	SDL_ShowCursor(isFullscreen);

	unsigned w;	unsigned h;
	if (isFullscreen)
	{
		w = previous_width;
		h = previous_height;
	}
	else
	{
		SDL_GetDesktopDisplayMode(0, &DM);
		w = DM.w; h = DM.h;
		previous_width = screen_width;
		previous_height = screen_height;
	}
	SetWindowSize(w, h, true);
}

void ModuleWindow::ToggleVSync()
{
	SDL_GL_SetSwapInterval(!SDL_GL_GetSwapInterval());
}

void ModuleWindow::ToggleResizable()
{
	Uint32 resizableFlag = SDL_WINDOW_RESIZABLE;
	bool isResizable = SDL_GetWindowFlags(window) & resizableFlag;
	SDL_SetWindowResizable(window, (SDL_bool)!isResizable);
}

void ModuleWindow::ToggleBorderless()
{
	Uint32 resizableFlag = SDL_WINDOW_BORDERLESS;
	bool isResizable = SDL_GetWindowFlags(window) & resizableFlag;
	SDL_SetWindowBordered(window, (SDL_bool)!borderless);
}

void ModuleWindow::SetWindowBrightness(const int value)
{
	SDL_SetWindowBrightness(window, brightness);
}

void ModuleWindow::SetWindowSize(unsigned& w, unsigned& h, const bool useSDL)
{
	App->window->screen_width = w;
	App->window->screen_height = h;
	if (useSDL)
		SDL_SetWindowSize(window, screen_width, screen_height);
}

