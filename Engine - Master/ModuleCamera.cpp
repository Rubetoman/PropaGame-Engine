#include "ModuleCamera.h"


ModuleCamera::ModuleCamera()
{
	cam_position	= float3(0, 0, 3);
	cam_target		= float3(0, 0, -1);
	cam_up			= float3(0, 1, 0);
	yaw = -90.0f;
	pitch = 0.0f;
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	InitFrustum();
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

float4x4 ModuleCamera::ProjectionMatrix()
{
	return frustum.ProjectionMatrix();;
}


void ModuleCamera::InitFrustum() 
{
	frustum.type = math::FrustumType::PerspectiveFrustum;
	frustum.pos = math::float3::zero;
	frustum.front = -math::float3::unitZ;
	frustum.up = math::float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov * 0.5f)) *(App->window->screen_width / App->window->screen_height);
	// Calculate horizontal first
	//frustum.horizontalFov = math::pi / 4.0f;
	//frustum.verticalFov = 2.0f * atanf(tanf(frustum.horizontalFov * 0.5f)) *((float)App->window->screen_height / (float)App->window->screen_width);
}

void ModuleCamera::SetHorizontalFOV(float& degrees)
{
	frustum.horizontalFov = math::DegToRad(degrees);
	frustum.verticalFov = 2.0f * atanf(tanf(frustum.horizontalFov * 0.5f)) *(App->window->screen_width / App->window->screen_height);
}

void ModuleCamera::SetVerticalFOV(float& degrees)
{
	frustum.verticalFov = math::DegToRad(degrees);
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov * 0.5f)) *(App->window->screen_width / App->window->screen_height);
}

void ModuleCamera::UpdateScreenSize() {
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov * 0.5f)) *(App->window->screen_width / App->window->screen_height);
	frustum.verticalFov = 2.0f * atanf(tanf(frustum.horizontalFov * 0.5f)) *(App->window->screen_width / App->window->screen_height);
}

float4x4 ModuleCamera::LookAt(math::float3& target, math::float3& eye)
{
	float4x4 view_matrix;

	// projection
	math::float3 f(target); f.Normalize();
	math::float3 s(f.Cross(math::float3(0, 1, 0))); s.Normalize();
	math::float3 u(s.Cross(f));

	view_matrix[0][0] = s.x;	view_matrix[0][1] = s.y;	view_matrix[0][2] = s.z;
	view_matrix[1][0] = u.x;	view_matrix[1][1] = u.y;	view_matrix[1][2] = u.z;
	view_matrix[2][0] = -f.x;	view_matrix[2][1] = -f.y;	view_matrix[2][2] = -f.z;

	view_matrix[0][3] = -s.Dot(eye);	view_matrix[1][3] = -u.Dot(eye);	view_matrix[2][3] = f.Dot(eye);
	view_matrix[3][0] = 0.0f;			view_matrix[3][1] = 0.0f;			view_matrix[3][2] = 0.0f;			view_matrix[3][3] = 1.0f;

	return view_matrix;
}

void ModuleCamera::TranslateCameraInput() 
{
	if (App->input->GetKey(SDL_SCANCODE_Q))
	{
		cam_position += cam_up * cam_speed * App->deltaTime;
	}
	else if (App->input->GetKey(SDL_SCANCODE_E))
	{
		cam_position -= cam_up * cam_speed * App->deltaTime;
	}
	else if (App->input->GetKey(SDL_SCANCODE_A))
	{
		cam_position += cam_up.Cross(cam_target).Normalized() * cam_speed * App->deltaTime;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D))
	{
		cam_position -= cam_up.Cross(cam_target).Normalized() * cam_speed * App->deltaTime;
	}
	else if (App->input->GetKey(SDL_SCANCODE_W))
	{
		cam_position += cam_target * cam_speed * App->deltaTime;
	}
	else if (App->input->GetKey(SDL_SCANCODE_S))
	{
		cam_position -= cam_target * cam_speed * App->deltaTime;
	}
}

void ModuleCamera::RotateCameraInput() 
{
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		pitch += cam_rot_speed * App->deltaTime;
		RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		pitch -= cam_rot_speed * App->deltaTime;
		RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		yaw -= cam_rot_speed * App->deltaTime;
		RotateCamera();
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		yaw += cam_rot_speed * App->deltaTime;
		RotateCamera();
	}
}

void ModuleCamera::CameraSpeedInput(float modifier) 
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
	{
		cam_speed *= modifier;
		cam_rot_speed *= modifier;
	}
	else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP)
	{
		cam_speed /= modifier;
		cam_rot_speed /= modifier;
	}
}

void ModuleCamera::RotateCamera() {

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	math::float3 rotation;
	rotation.x = SDL_cosf(degreesToRadians(yaw)) * SDL_cosf(degreesToRadians(pitch));
	rotation.y = SDL_sinf(degreesToRadians(pitch));
	rotation.z = SDL_sinf(degreesToRadians(yaw)) * SDL_cosf(degreesToRadians(pitch));
	cam_target = rotation.Normalized();
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

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	math::float3 front;
	front.x = SDL_cosf(yaw) * SDL_cosf(pitch);
	front.y = SDL_sinf(pitch);
	front.z = SDL_sinf(yaw) * SDL_cosf(pitch);
	cam_target = front.Normalized();
}