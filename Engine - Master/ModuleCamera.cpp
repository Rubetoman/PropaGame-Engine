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
	mainCamera->cam_position = float3(0, 0, 3);
	mainCamera->cam_front = float3(0, 0, -1);
	mainCamera->cam_up = float3(0, 1, 0);
	mainCamera->yaw = -90.0f;
	mainCamera->pitch = 0.0f;
	cameras.push_back(mainCamera);

	lastX = App->window->screen_width / 2;
	lastY = App->window->screen_height / 2;
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
	if (App->input->GetKey(SDL_SCANCODE_Q))
	{
		mainCamera->TranslateCamera(mainCamera->cam_up);
	}
	else if (App->input->GetKey(SDL_SCANCODE_E))
	{
		mainCamera->TranslateCamera(-mainCamera->cam_up);
	}
	else if (App->input->GetKey(SDL_SCANCODE_A))
	{
		mainCamera->TranslateCamera(mainCamera->cam_up.Cross(mainCamera->cam_front).Normalized());
	}
	else if (App->input->GetKey(SDL_SCANCODE_D))
	{
		mainCamera->TranslateCamera(-mainCamera->cam_up.Cross(mainCamera->cam_front).Normalized());
	}
	else if (App->input->GetKey(SDL_SCANCODE_W))
	{
		mainCamera->TranslateCamera(mainCamera->cam_front);
	}
	else if (App->input->GetKey(SDL_SCANCODE_S))
	{
		mainCamera->TranslateCamera(-mainCamera->cam_front);
	}
}

void ModuleCamera::RotateCameraInput() 
{
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) 
	{
		mainCamera->pitch += mainCamera->cam_rot_speed * App->deltaTime;
		mainCamera->RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) 
	{
		mainCamera->pitch -= mainCamera->cam_rot_speed * App->deltaTime;
		mainCamera->RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) 
	{
		mainCamera->yaw -= mainCamera->cam_rot_speed * App->deltaTime;
		mainCamera->RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) 
	{
		mainCamera->yaw += mainCamera->cam_rot_speed * App->deltaTime;
		mainCamera->RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_F))
	{
		mainCamera->cam_front = math::float3(0, 0, 0) - mainCamera->cam_position;
		mainCamera->UpdatePitchYaw();
		mainCamera->LookAt(mainCamera->cam_front, mainCamera->cam_position);
	}
}

void ModuleCamera::CameraSpeedInput(float modifier) 
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
	{
		mainCamera->cam_speed *= modifier;
		mainCamera->cam_rot_speed *= modifier;
	}
	else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP)
	{
		mainCamera->cam_speed /= modifier;
		mainCamera->cam_rot_speed /= modifier;
	}
}

void ModuleCamera::MouseUpdate(math::float2& mouse_new_pos)
{
	if (firstMouse)
	{
		lastX = mouse_new_pos.x;
		lastY = mouse_new_pos.y;
		firstMouse = false;
	}

	float xoffset = mouse_new_pos.x - lastX;
	float yoffset = lastY - mouse_new_pos.y;
	lastX = mouse_new_pos.x;
	lastY = mouse_new_pos.y;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	mainCamera->yaw += xoffset;
	mainCamera->pitch += yoffset;

	if (mainCamera->pitch > 89.0f)
		mainCamera->pitch = 89.0f;
	if (mainCamera->pitch < -89.0f)
		mainCamera->pitch = -89.0f;

	math::float3 front;
	front.x = SDL_cosf(mainCamera->yaw) * SDL_cosf(mainCamera->pitch);
	front.y = SDL_sinf(mainCamera->pitch);
	front.z = SDL_sinf(mainCamera->yaw) * SDL_cosf(mainCamera->pitch);
	mainCamera->cam_front = front.Normalized();
}