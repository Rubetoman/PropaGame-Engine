#include "Camera.h"

Camera::Camera()
{
	InitFrustum();
}


Camera::~Camera()
{
}

void Camera::TranslateCamera(math::float3 direction)
{
	cam_position += direction * cam_speed * App->deltaTime;
	//LookAt(cam_position, (cam_position + cam_front));
}

void Camera::RotateCamera() {

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	math::float3 rotation;
	rotation.x = SDL_cosf(degreesToRadians(yaw)) * SDL_cosf(degreesToRadians(pitch));
	rotation.y = SDL_sinf(degreesToRadians(pitch));
	rotation.z = SDL_sinf(degreesToRadians(yaw)) * SDL_cosf(degreesToRadians(pitch));
	cam_front = rotation.Normalized();
}

float4x4 Camera::LookAt(math::float3& target, math::float3& eye)
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

float4x4 Camera::ProjectionMatrix()
{
	return frustum.ProjectionMatrix();;
}


void Camera::InitFrustum()
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

void Camera::SetHorizontalFOV(float& degrees)
{
	frustum.horizontalFov = math::DegToRad(degrees);
	frustum.verticalFov = 2.0f * atanf(tanf(frustum.horizontalFov * 0.5f)) *(App->window->screen_width / App->window->screen_height);
}

void Camera::SetVerticalFOV(float& degrees)
{
	frustum.verticalFov = math::DegToRad(degrees);
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov * 0.5f)) *(App->window->screen_width / App->window->screen_height);
}

void Camera::UpdatePitchYaw() 
{
	pitch = -math::RadToDeg(SDL_atanf(cam_front.y / cam_front.z));
	yaw = -math::RadToDeg(SDL_atanf(cam_front.x / cam_front.z));

	if (math::IsNan(pitch))
		pitch = 0.0f;

	if (math::IsNan(yaw))
		yaw = 0.0f;
}
