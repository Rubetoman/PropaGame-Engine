#include "Application.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"

#include "GL/glew.h"
#include "SDL.h"

ModuleCamera::ModuleCamera()
{
	eye =		float3(0, 0, 5);
	target =	float3(0, 0, 0);
	up =		float3(0, 0, 1);
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
	ChangeCameraSpeed(5.0f);

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

void ModuleCamera::ChangeCameraSpeed(float modifier) 
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