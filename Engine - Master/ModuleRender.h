#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void WindowResized(unsigned width, unsigned height);

	void GenerateFallback();

public:

	void* context = nullptr;
	unsigned ubo = 0u;
	unsigned fallback = 0u; // Fallback texture (if any texture of material doesn't have texture use this)
};

#endif
