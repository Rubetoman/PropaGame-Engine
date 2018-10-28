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
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
		eye		+=	float3(0.0f, 1.0f, 0.0f);
		target	+=	float3(0.0f, 1.0f, 0.0f);
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
	math::float3 f(target - eye); f.Normalize();
	math::float3 s(f.Cross(up)); s.Normalize();
	math::float3 u(s.Cross(f));

	view_matrix[0][0] = s.x;				view_matrix[0][1] = s.y;				view_matrix[0][2] = s.z;
	view_matrix[1][0] = u.x;				view_matrix[1][1] = u.y;				view_matrix[1][2] = u.z;
	view_matrix[2][0] = -f.x;				view_matrix[2][1] = -f.y;				view_matrix[2][2] = -f.z;

	view_matrix[0][3] = -s.Dot(eye);		view_matrix[1][3] = -u.Dot(eye);		view_matrix[2][3] = f.Dot(eye);
	view_matrix[3][0] = 0.0f;				view_matrix[3][1] = 0.0f;				view_matrix[3][2] = 0.0f;			view_matrix[3][3] = 1.0f;

	return view_matrix;
}