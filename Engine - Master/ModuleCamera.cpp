#include "ModuleCamera.h"


ModuleCamera::ModuleCamera()
{
	eye		= float3(0, 0, 5);
	target	= float3(0, 0, 0);
	up		= float3(0, 0, 1);
	pitch	= 0.0f;
	yaw		= 0.0f;
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	InitProyection();
	return true;
}

update_status ModuleCamera::PreUpdate() 
{
	TranslateCameraInput();
	CameraSpeedInput(5.0f);

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		RotateCamera(NegativePitch);
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		RotateCamera(PositivePitch);
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		RotateCamera(PositiveYaw);
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		RotateCamera(NegativeYaw);
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

float4x4 ModuleCamera::ProjectionMatrix()
{
	float4x4 proMatrix;
	proMatrix = frustum.ProjectionMatrix();

	return proMatrix;
}


void ModuleCamera::InitProyection() 
{
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f)) *(SCREEN_WIDTH / SCREEN_HEIGHT);
}

float4x4 ModuleCamera::LookAt(math::float3& target, math::float3& eye, math::float3& up)
{
	float4x4 view_matrix;

	// projection
	forward_v = float3(target - eye); forward_v.Normalize();
	side_v =	float3(forward_v.Cross(up)); side_v.Normalize();
	up_v =		float3 (side_v.Cross(forward_v));

	view_matrix[0][0] = side_v.x;			view_matrix[0][1] = side_v.y;			view_matrix[0][2] = side_v.z;
	view_matrix[1][0] = up_v.x;				view_matrix[1][1] = up_v.y;				view_matrix[1][2] = up_v.z;
	view_matrix[2][0] = -forward_v.x;		view_matrix[2][1] = -forward_v.y;		view_matrix[2][2] = -forward_v.z;

	view_matrix[0][3] = -side_v.Dot(eye);	view_matrix[1][3] = -up_v.Dot(eye);		view_matrix[2][3] = forward_v.Dot(eye);
	view_matrix[3][0] = 0.0f;				view_matrix[3][1] = 0.0f;				view_matrix[3][2] = 0.0f;					view_matrix[3][3] = 1.0f;

	return view_matrix;
}

void ModuleCamera::TranslateCameraInput() 
{
	if (App->input->GetKey(SDL_SCANCODE_Q))
	{
		eye += float3(0.0f, 1.0f, 0.0f) * cam_speed;
		target += float3(0.0f, 1.0f, 0.0f) * cam_speed;
	}
	else if (App->input->GetKey(SDL_SCANCODE_E))
	{
		eye -= float3(0.0f, 1.0f, 0.0f) * cam_speed;
		target -= float3(0.0f, 1.0f, 0.0f) * cam_speed;
	}
	else if (App->input->GetKey(SDL_SCANCODE_A))
	{
		eye -= side_v * cam_speed;
		target -= side_v * cam_speed;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D))
	{
		eye += side_v * cam_speed;
		target += side_v * cam_speed;
	}
	else if (App->input->GetKey(SDL_SCANCODE_W))
	{
		eye += forward_v * cam_speed;
		target += forward_v * cam_speed;
	}
	else if (App->input->GetKey(SDL_SCANCODE_S))
	{
		eye -= forward_v * cam_speed;
		target -= forward_v * cam_speed;
	}
}

void ModuleCamera::CameraSpeedInput(float modifier) 
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
	{
		cam_speed *= modifier;
	}
	else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP)
	{
		cam_speed /= modifier;
	}
}

void ModuleCamera::RotateCamera(CameraRotation camera_rot) {
	math::float3 direction;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	switch (camera_rot) {
	case PositivePitch:
		pitch += cam_speed;
		forward_v.y = SDL_sin(degreesToRadians(pitch));
		forward_v.x = SDL_cos(degreesToRadians(pitch));
		forward_v.z = SDL_cos(degreesToRadians(pitch));
		forward_v.Normalize();
		target += forward_v * cam_speed;
		break;
	case NegativePitch:
		pitch -= cam_speed * 2;
		forward_v.y = SDL_sin(degreesToRadians(pitch));
		forward_v.x = SDL_cos(degreesToRadians(pitch));
		forward_v.z = SDL_cos(degreesToRadians(pitch));
		forward_v.Normalize();
		target -= forward_v * cam_speed;
		break;
	case PositiveYaw:
		yaw += cam_speed;
		forward_v.y = SDL_cos(degreesToRadians(pitch)) * SDL_cos(degreesToRadians(yaw));
		forward_v.x = SDL_sin(degreesToRadians(pitch));
		forward_v.z = SDL_cos(degreesToRadians(pitch)) * SDL_sin(degreesToRadians(yaw));
		forward_v.Normalize();
		target += forward_v * cam_speed;
		eye += forward_v * cam_speed;
		break;
	case NegativeYaw:
		yaw -= cam_speed;
		forward_v.y = SDL_cos(degreesToRadians(pitch)) * SDL_cos(degreesToRadians(yaw));
		forward_v.x = SDL_sin(degreesToRadians(pitch));
		forward_v.z = SDL_cos(degreesToRadians(pitch)) * SDL_sin(degreesToRadians(yaw));
		forward_v.Normalize();
		target -= forward_v * cam_speed;
		eye -= forward_v * cam_speed;
		break;
	}
}

void ModuleCamera::MouseUpdate(float2& mouse_new_pos)
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
	forward_v = front.Normalized();
}