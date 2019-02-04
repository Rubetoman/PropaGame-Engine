#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ComponentCamera;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Start() override;
	bool Init() override;
	update_status PreUpdate() override;
	update_status Update() override;
	update_status PostUpdate() override;
	bool CleanUp() override;

	void WindowResized(unsigned width, unsigned height);

	void GenerateFallback();

	void GenBlockUniforms();
	void AddBlockUniforms(const unsigned &shader) const;
	void SetViewUniform(const ComponentCamera &camera) const;
	void SetProjectionUniform(const ComponentCamera &camera) const;

public:

	void* context = nullptr;
	unsigned ubo = 0u;
	unsigned fallback = 0u; // Fallback texture (if any texture of material doesn't have texture use this)

	bool wireframe = false; // If true the scene will be drawn on wireframe mode.
};

#endif
