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
	position += direction * speed * App->deltaTime;
}

void Camera::RotateCamera() 
{
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	math::float3 rotation;
	rotation.x = SDL_cosf(degreesToRadians(yaw)) * SDL_cosf(degreesToRadians(pitch));
	rotation.y = SDL_sinf(degreesToRadians(pitch));
	rotation.z = SDL_sinf(degreesToRadians(yaw)) * SDL_cosf(degreesToRadians(pitch));
	front = rotation.Normalized();

	LookAt(position + front);
}

float4x4 Camera::LookAt(math::float3& target)
{
	float4x4 view_matrix;

	// projection
	front = math::float3(target - position); front.Normalize();
	side = math::float3(front.Cross(math::float3(0, 1, 0))); side.Normalize();
	up = math::float3(side.Cross(front));

	view_matrix[0][0] = side.x;		view_matrix[0][1] = side.y;		view_matrix[0][2] = side.z;
	view_matrix[1][0] = up.x;		view_matrix[1][1] = up.y;		view_matrix[1][2] = up.z;
	view_matrix[2][0] = -front.x;	view_matrix[2][1] = -front.y;	view_matrix[2][2] = -front.z;

	view_matrix[0][3] = -side.Dot(position);	view_matrix[1][3] = -up.Dot(position);	view_matrix[2][3] = front.Dot(position);
	view_matrix[3][0] = 0.0f;					view_matrix[3][1] = 0.0f;				view_matrix[3][2] = 0.0f;			view_matrix[3][3] = 1.0f;

	return view_matrix;
}

float4x4 Camera::ProjectionMatrix()
{
	return frustum.ProjectionMatrix();;
}


const void Camera::InitFrustum()
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

const void Camera::UpdatePitchYaw() 
{
	pitch = -math::RadToDeg(SDL_atanf(front.y / front.z));
	yaw = -math::RadToDeg(SDL_atanf(front.x / front.z));

	if (math::IsNan(pitch))
		pitch = 0.0f;

	if (math::IsNan(yaw))
		yaw = 0.0f;
}
