#include "ModuleCamera.h"

#include "Application.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "ModuleTime.h"
#include "ModuleResources.h"
#include "WindowScene.h"

#include "ComponentTransform.h"
#include "ComponentCamera.h"

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	editor_camera_go = new GameObject("Editor Camera");
	editor_camera_go->transform->position = math::float3(0.0f, 0.0f, 3.0f);
	editor_camera_comp = (ComponentCamera*)editor_camera_go->CreateComponent(component_type::Editor_Camera);
	App->window->SetWindowSize(App->window->screen_width, App->window->screen_height, true);
	App->scene->scene_gos.push_back(editor_camera_go);

	return true;
}

bool ModuleCamera::Start()
{
	editor_camera_comp->LookAt(math::float3(0.0f, 0.0f, 0.0f));
	return true;
}

update_status ModuleCamera::PreUpdate() 
{
	if (App->editor->scene->IsFocused())
	{
		TranslateCameraInput();
		RotateCameraInput();
		CameraSpeedInput(3.0f);
	}

	// Check when the mouse is right or left click is up
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP || App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	{
		SDL_ShowCursor(SDL_ENABLE);
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	editor_camera_comp->Update();

	for (auto camera : App->resources->cameras)
	{
		camera->GetComponent(component_type::Camera)->Update();
	}

	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::UpdateScreenSize() 
{
	if (editor_camera_comp != nullptr)
	{
		editor_camera_comp->frustum.horizontalFov = 2.0f * atanf(tanf(editor_camera_comp->frustum.verticalFov * 0.5f)) *(App->window->screen_width / App->window->screen_height);
		editor_camera_comp->frustum.verticalFov = 2.0f * atanf(tanf(editor_camera_comp->frustum.horizontalFov * 0.5f)) *(App->window->screen_width / App->window->screen_height);
	}
}

void ModuleCamera::TranslateCameraInput() 
{
	if (editor_camera_comp == nullptr)
		return;

	float3 movement = float3::zero;

	// Right click + WASD/QE translates the camera
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_Q))
		{
			movement += math::float3::unitY * editor_camera_comp->speed * App->time->real_delta_time;
		}
		else if (App->input->GetKey(SDL_SCANCODE_E))
		{
			movement -= math::float3::unitY * editor_camera_comp->speed * App->time->real_delta_time;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A))
		{
			movement -= editor_camera_comp->frustum.WorldRight() * editor_camera_comp->speed * App->time->real_delta_time;
		}
		else if (App->input->GetKey(SDL_SCANCODE_D))
		{
			movement += editor_camera_comp->frustum.WorldRight() * editor_camera_comp->speed * App->time->real_delta_time;
		}
		else if (App->input->GetKey(SDL_SCANCODE_W))
		{
			movement += editor_camera_comp->frustum.front * editor_camera_comp->speed * App->time->real_delta_time;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S))
		{
			movement -= editor_camera_comp->frustum.front * editor_camera_comp->speed * App->time->real_delta_time;
		}
		editor_camera_go->transform->position += movement;
	}

	// Mouse wheel go forward or backwards
	if (App->input->GetMouseButtonDown(SDL_BUTTON_X1) == KEY_DOWN)
	{
		WheelInputTranslation(App->input->GetMouseWheel());
	}
}

void ModuleCamera::RotateCameraInput() 
{
	if (editor_camera_comp == nullptr)
		return;

	// ALT + mouse left click + mouse move orbit around the loaded mesh
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) 
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			SDL_ShowCursor(SDL_DISABLE);
			editor_camera_comp->Orbit(editor_camera_comp->rotation_speed * App->input->GetMouseMotion().x, editor_camera_comp->rotation_speed * App->input->GetMouseMotion().y);
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) 
		{
			SDL_ShowCursor(SDL_ENABLE);
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_F))
	{
		editor_camera_comp->LookAt(math::float3(0, 0, 0));
	}
	if(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT))
	{
		SDL_ShowCursor(SDL_DISABLE);
		editor_camera_comp->Rotate(editor_camera_comp->rotation_speed * App->input->GetMouseMotion().x, editor_camera_comp->rotation_speed * App->input->GetMouseMotion().y);
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
	{
		SDL_ShowCursor(SDL_ENABLE);
	}
}

void ModuleCamera::CameraSpeedInput(float modifier) 
{
	if (editor_camera_comp == nullptr)
		return;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
	{
		editor_camera_comp->speed *= modifier;
		editor_camera_comp->rotation_speed *= modifier;
	}
	else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP)
	{
		editor_camera_comp->speed /= modifier;
		editor_camera_comp->rotation_speed /= modifier;
	}
}

void ModuleCamera::WheelInputTranslation(const fPoint& wheel_motion)
{
	if (editor_camera_go == nullptr)
		return;
	
	editor_camera_go->transform->position += editor_camera_comp->frustum.front.Mul(wheel_motion.y) * 10 * editor_camera_comp->speed * App->time->real_delta_time;
}

void ModuleCamera::FitCamera(const AABB &boundingBox)
{
	if (editor_camera_go == nullptr || editor_camera_comp == nullptr)
		return;

	math::float3 diagonal = boundingBox.Diagonal();
	math::float3 center = boundingBox.CenterPoint();
	editor_camera_go->transform->position.z = (center.z + diagonal.Length());
	editor_camera_go->transform->position.y = center.y;
	editor_camera_go->transform->position.x = center.x;
	editor_camera_comp->LookAt(math::float3(center.x,center.y,center.z));
}