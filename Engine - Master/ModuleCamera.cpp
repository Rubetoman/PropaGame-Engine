#include "ModuleCamera.h"


ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	mainCamera = new Camera();
	mainCamera->position = float3(0, 0, 3);
	mainCamera->front = float3(0, 0, -1);
	mainCamera->up = float3(0, 1, 0);
	mainCamera->yaw = -90.0f;
	mainCamera->pitch = 0.0f;
	cameras.push_back(mainCamera);

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



void ModuleCamera::UpdateScreenSize() 
{
	mainCamera->frustum.horizontalFov = 2.0f * atanf(tanf(mainCamera->frustum.verticalFov * 0.5f)) *(App->window->screen_width / App->window->screen_height);
	mainCamera->frustum.verticalFov = 2.0f * atanf(tanf(mainCamera->frustum.horizontalFov * 0.5f)) *(App->window->screen_width / App->window->screen_height);
}

void ModuleCamera::TranslateCameraInput() 
{
	// Right click + WASD/QE translates the camera
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_Q))
		{
			mainCamera->TranslateCamera(mainCamera->up);
		}
		else if (App->input->GetKey(SDL_SCANCODE_E))
		{
			mainCamera->TranslateCamera(-mainCamera->up);
		}
		else if (App->input->GetKey(SDL_SCANCODE_A))
		{
			mainCamera->TranslateCamera(-mainCamera->side);
		}
		else if (App->input->GetKey(SDL_SCANCODE_D))
		{
			mainCamera->TranslateCamera(mainCamera->side);
		}
		else if (App->input->GetKey(SDL_SCANCODE_W))
		{
			mainCamera->TranslateCamera(mainCamera->front);
		}
		else if (App->input->GetKey(SDL_SCANCODE_S))
		{
			mainCamera->TranslateCamera(-mainCamera->front);
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
	// ALT + mouse left click + mouse move orbit around the loaded mesh
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) 
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			mainCamera->UpdatePitchYaw();
			SDL_ShowCursor(SDL_DISABLE);
			MouseInputTranslation(App->input->GetMousePosition());
			mainCamera->LookAt(math::float3(0, 0, 0));
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
			SDL_ShowCursor(SDL_ENABLE);
			new_click = true;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) 
	{
		mainCamera->pitch += mainCamera->rotation_speed * App->time->real_delta_time;
		mainCamera->RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) 
	{
		mainCamera->pitch -= mainCamera->rotation_speed * App->time->real_delta_time;
		mainCamera->RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) 
	{
		mainCamera->yaw -= mainCamera->rotation_speed * App->time->real_delta_time;
		mainCamera->RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) 
	{
		mainCamera->yaw += mainCamera->rotation_speed * App->time->real_delta_time;
		mainCamera->RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_F))
	{
		mainCamera->UpdatePitchYaw();
		mainCamera->LookAt(math::float3(0, 0, 0));
	}
	else if(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		SDL_ShowCursor(SDL_DISABLE);
		MouseInputRotation(App->input->GetMousePosition());
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP) {
		SDL_ShowCursor(SDL_ENABLE);
		new_click = true;
	}
}

void ModuleCamera::CameraSpeedInput(float modifier) 
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
	{
		mainCamera->speed *= modifier;
		mainCamera->rotation_speed *= modifier;
	}
	else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP)
	{
		mainCamera->speed /= modifier;
		mainCamera->rotation_speed /= modifier;
	}
}

void ModuleCamera::MouseInputTranslation(const iPoint& mouse_position)
{
	if (new_click)
	{
		last_x = mouse_position.x;
		last_y = mouse_position.y;
		new_click = false;
	}

	int x_offset = mouse_position.x - last_x;
	int y_offset = last_y - mouse_position.y;
	last_x = mouse_position.x;
	last_y = mouse_position.y;

	x_offset *= mainCamera->rotation_speed * mouse_sensitivity;
	y_offset *= mainCamera->rotation_speed * mouse_sensitivity;

	mainCamera->position -= mainCamera->side.Mul(x_offset) * mainCamera->speed * App->time->real_delta_time;
	mainCamera->position -= mainCamera->up.Mul(y_offset) * mainCamera->speed * App->time->real_delta_time;
}

void ModuleCamera::MouseInputRotation(const iPoint& mouse_position)
{
	if (new_click)
	{
		last_x = mouse_position.x;
		last_y = mouse_position.y;
		new_click = false;
	}

	int x_offset = mouse_position.x - last_x;
	int y_offset = last_y - mouse_position.y;
	last_x = mouse_position.x;
	last_y = mouse_position.y;

	x_offset *= mainCamera->rotation_speed * mouse_sensitivity;
	y_offset *= mainCamera->rotation_speed * mouse_sensitivity;

	mainCamera->yaw += x_offset;
	mainCamera->pitch += y_offset;

	mainCamera->RotateCamera();
}

void ModuleCamera::WheelInputTranslation(const iPoint& wheel_motion)
{
	mainCamera->position -= mainCamera->side.Mul(wheel_motion.x) * 10 * mainCamera->speed * App->time->real_delta_time;
	mainCamera->position -= mainCamera->front.Mul(-wheel_motion.y) * 10 * mainCamera->speed * App->time->real_delta_time;
}

void ModuleCamera::FitCamera(const AABB &boundingBox)
{
	math::float3 diagonal = boundingBox.Diagonal();
	math::float3 center = boundingBox.CenterPoint();
	mainCamera->position.z = (center.z + diagonal.Length());
	mainCamera->position.y = center.y;
	mainCamera->position.x = center.x;
	mainCamera->LookAt(math::float3(center.x,center.y,center.z));
}