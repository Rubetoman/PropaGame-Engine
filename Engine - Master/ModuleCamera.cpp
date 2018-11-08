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
	TranslateCameraInput();
	RotateCameraInput();
	CameraSpeedInput(5.0f);

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



void ModuleCamera::UpdateScreenSize() {
	mainCamera->frustum.horizontalFov = 2.0f * atanf(tanf(mainCamera->frustum.verticalFov * 0.5f)) *(App->window->screen_width / App->window->screen_height);
	mainCamera->frustum.verticalFov = 2.0f * atanf(tanf(mainCamera->frustum.horizontalFov * 0.5f)) *(App->window->screen_width / App->window->screen_height);
}

void ModuleCamera::TranslateCameraInput() 
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
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
			mainCamera->TranslateCamera(mainCamera->up.Cross(mainCamera->front).Normalized());
		}
		else if (App->input->GetKey(SDL_SCANCODE_D))
		{
			mainCamera->TranslateCamera(-mainCamera->up.Cross(mainCamera->front).Normalized());
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
}

void ModuleCamera::RotateCameraInput() 
{
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) 
	{
		mainCamera->pitch += mainCamera->rotation_speed * App->deltaTime;
		mainCamera->RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) 
	{
		mainCamera->pitch -= mainCamera->rotation_speed * App->deltaTime;
		mainCamera->RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) 
	{
		mainCamera->yaw -= mainCamera->rotation_speed * App->deltaTime;
		mainCamera->RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) 
	{
		mainCamera->yaw += mainCamera->rotation_speed * App->deltaTime;
		mainCamera->RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_F))
	{
		mainCamera->UpdatePitchYaw();
		mainCamera->LookAt(math::float3(0, 0, 0));
	}
	else if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		SDL_ShowCursor(SDL_DISABLE);
		MouseInputRotation(App->input->GetMousePosition());
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
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