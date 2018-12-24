#include "ModuleCamera.h"

#include "Application.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "ModuleTime.h"
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
	editor_camera_go = App->scene->CreateGameObject("Editor Camera");
	editor_camera_go->transform->position = math::float3(0.0f, 0.0f, 3.0f);
	editor_camera_comp = (ComponentCamera*)editor_camera_go->CreateComponent(component_type::Editor_Camera);
	App->window->SetWindowSize(App->window->screen_width, App->window->screen_height, true);
	//cameras.push_back(mainCamera);

	last_x = App->window->screen_width / 2;
	last_y = App->window->screen_height / 2;
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
		new_click = true;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::DeleteCamera(GameObject* go)
{
	if (go != nullptr)
	{
		ComponentCamera* camera = (ComponentCamera*)go->GetComponent(component_type::Camera);
		int position = camera->GetCameraNumber();
		if (position > -1)
		{
			cameras.erase(cameras.begin() + position);
			camera->CleanUp();
			RELEASE(camera);
		}
		else
			LOG("Error deleting component.");
	}
	else
		LOG("Warning: Component was nullptr.");
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

	// Right click + WASD/QE translates the camera
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_Q))
		{
			editor_camera_comp->TranslateCamera(editor_camera_comp->up);
		}
		else if (App->input->GetKey(SDL_SCANCODE_E))
		{
			editor_camera_comp->TranslateCamera(-editor_camera_comp->up);
		}
		else if (App->input->GetKey(SDL_SCANCODE_A))
		{
			editor_camera_comp->TranslateCamera(-editor_camera_comp->side);
		}
		else if (App->input->GetKey(SDL_SCANCODE_D))
		{
			editor_camera_comp->TranslateCamera(editor_camera_comp->side);
		}
		else if (App->input->GetKey(SDL_SCANCODE_W))
		{
			editor_camera_comp->TranslateCamera(editor_camera_comp->front);
		}
		else if (App->input->GetKey(SDL_SCANCODE_S))
		{
			editor_camera_comp->TranslateCamera(-editor_camera_comp->front);
		}
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
			editor_camera_comp->UpdatePitchYaw();
			SDL_ShowCursor(SDL_DISABLE);
			MouseInputTranslation(App->input->GetMousePosition());
			editor_camera_comp->LookAt(math::float3(0, 0, 0));
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) 
		{
			SDL_ShowCursor(SDL_ENABLE);
			new_click = true;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) 
	{
		editor_camera_comp->pitch += editor_camera_comp->rotation_speed * App->time->real_delta_time;
		editor_camera_comp->RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) 
	{
		editor_camera_comp->pitch -= editor_camera_comp->rotation_speed * App->time->real_delta_time;
		editor_camera_comp->RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) 
	{
		editor_camera_comp->yaw -= editor_camera_comp->rotation_speed * App->time->real_delta_time;
		editor_camera_comp->RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) 
	{
		editor_camera_comp->yaw += editor_camera_comp->rotation_speed * App->time->real_delta_time;
		editor_camera_comp->RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_F))
	{
		editor_camera_comp->UpdatePitchYaw();
		editor_camera_comp->LookAt(math::float3(0, 0, 0));
	}
	if(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT))
	{
		SDL_ShowCursor(SDL_DISABLE);
		MouseInputRotation(App->input->GetMousePosition());
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

void ModuleCamera::MouseInputTranslation(const fPoint& mouse_position)
{
	if (editor_camera_go == nullptr)
		return;

	if (new_click)
	{
		last_x = (float)mouse_position.x;
		last_y = (float)mouse_position.y;
		new_click = false;
	}

	float x_offset = (float)mouse_position.x - last_x;
	float y_offset = last_y - (float)mouse_position.y;
	last_x = (float)mouse_position.x;
	last_y = (float)mouse_position.y;

	x_offset *= editor_camera_comp->rotation_speed * mouse_sensitivity;
	y_offset *= editor_camera_comp->rotation_speed * mouse_sensitivity;

	editor_camera_go->transform->position -= editor_camera_comp->side.Mul(x_offset) * editor_camera_comp->speed * App->time->real_delta_time;
	editor_camera_go->transform->position -= editor_camera_comp->up.Mul(y_offset) * editor_camera_comp->speed * App->time->real_delta_time;
}

void ModuleCamera::MouseInputRotation(const fPoint& mouse_position)
{
	if (editor_camera_go == nullptr)
		return;

	if (new_click)
	{
		last_x = (float)mouse_position.x;
		last_y = (float)mouse_position.y;
		new_click = false;
	}

	float x_offset = (float)mouse_position.x - last_x;
	float y_offset = last_y - (float)mouse_position.y;
	last_x = (float)mouse_position.x;
	last_y = (float)mouse_position.y;

	x_offset *= editor_camera_comp->rotation_speed * mouse_sensitivity;
	y_offset *= editor_camera_comp->rotation_speed * mouse_sensitivity;

	editor_camera_comp->yaw += x_offset;
	editor_camera_comp->pitch += y_offset;

	editor_camera_comp->RotateCamera();
}

void ModuleCamera::WheelInputTranslation(const fPoint& wheel_motion)
{
	if (editor_camera_go == nullptr)
		return;

	editor_camera_go->transform->position -= editor_camera_comp->side.Mul(wheel_motion.x) * 10 * editor_camera_comp->speed * App->time->real_delta_time;
	editor_camera_go->transform->position -= editor_camera_comp->front.Mul(-wheel_motion.y) * 10 * editor_camera_comp->speed * App->time->real_delta_time;
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