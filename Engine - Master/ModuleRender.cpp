#include "ModuleRender.h"

#include "ModuleScene.h"
#include "ModuleDebugDraw.h"
#include "ModuleResources.h"
#include "ModuleCamera.h"

#include "ComponentTransform.h"
#include "ComponentCamera.h"

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");

	// Init SDL
	LOG("Initializing SDL...");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	context = SDL_GL_CreateContext(App->window->window);
	if (context == nullptr)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	int width = App->window->screen_width;
	int	height = App->window->screen_height;
	SDL_GetWindowSize(App->window->window, &width, &height);

	// Init Glew
	LOG("Initializing Glew...");
	GLenum error = glewInit();
	if (error != GL_NO_ERROR)
	{
		LOG("Error initializing glew!");
		return false;
	}

	// Init OpenGL
	LOG("Initializing OpenGL...");
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_2D);

	glClearDepth(1.0f);
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	glViewport(0, 0, width, height);

	LOG("Renderer context creation successful.");
	return true;
}

update_status ModuleRender::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, App->camera->editor_camera_comp->fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Draw References
	App->editor->DrawDebugReferences();

	// Draw Scene
	ComponentCamera* editor_camera = App->camera->editor_camera_comp;
	math::float4x4 proj = editor_camera->frustum.ProjectionMatrix();
	math::float4x4 view = editor_camera->frustum.ViewMatrix();
	App->scene->Draw(view, proj, *editor_camera);
	
	// Draw debug draw
	App->debug_draw->Draw(editor_camera, editor_camera->fbo, App->window->screen_height, App->window->screen_width);

	// CAMERAS
	for (auto &cameraGO : App->resources->cameras)
	{
		if (cameraGO->active)
		{
			ComponentCamera* camera = (ComponentCamera*)cameraGO->GetComponent(component_type::Camera);
			if (camera != nullptr && camera->active)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glBindFramebuffer(GL_FRAMEBUFFER, camera->fbo);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				// Draw Scene
				proj = camera->frustum.ProjectionMatrix();
				//view = camera->LookAt(cameraGO->transform->position + camera->front);
				math::float4x4 view = camera->frustum.ViewMatrix();
				App->scene->Draw(view, proj, *camera);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}
	}
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	// Draw editor
	App->editor->Draw();
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window

	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
    glViewport(0, 0, width, height); 
	App->window->SetWindowSize(width, height, false);
	App->camera->editor_camera_comp->CreateFrameBuffer();
}
