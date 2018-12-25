#include "ComponentCamera.h"

#include "ModuleCamera.h"
#include "ModuleTime.h"
#include "ComponentTransform.h"

#include "Application.h"
#include "GameObject.h"

ComponentCamera::ComponentCamera(GameObject* go) : Component(go, component_type::Camera)
{
	InitFrustum();
	CreateFrameBuffer();
	if (go != nullptr)
		frustum.pos = go->boundingBox.CenterPoint();
}

ComponentCamera::ComponentCamera(const ComponentCamera& comp) : Component(comp)
{
	//TODO: Copy variables
}

ComponentCamera::~ComponentCamera()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteTextures(1, &renderedTexture);
	if(type != component_type::Editor_Camera)
		App->camera->DeleteCamera(my_go);
}

Component* ComponentCamera::Duplicate()
{
	return new ComponentCamera(*this);
}

bool ComponentCamera::DrawOnInspector()
{
	ImGui::PushID(this);
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Camera Component"))
	{
		bool deleted = Component::DrawOnInspector();
		if (!deleted)
		{
			ImGui::SliderFloat("Near Plane Distance", &frustum.nearPlaneDistance, 0.0f, frustum.farPlaneDistance);
			ImGui::SliderFloat("Far Plane Distance", &frustum.farPlaneDistance, frustum.nearPlaneDistance, 3000.0f);
			//ImGui::SliderInt("Aspect Ratio", (int)&camera->frustum.AspectRatio(), 0, 179);
		}
		else
		{
			ImGui::PopID();
			return deleted;
		}
	}
	ImGui::PopID();
	return false;
}

unsigned ComponentCamera::GetCameraNumber() const
{
	auto pos = std::find(App->camera->cameras.begin(), App->camera->cameras.end(), my_go) - App->camera->cameras.begin();
	if (pos >= my_go->components.size())
	{
		LOG("Warning: %s not found as a camera on list of cameras.", my_go->name);
		return -1;
	}
	return pos;
}

void ComponentCamera::CreateFrameBuffer()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &renderedTexture);
	glDeleteRenderbuffers(1, &rbo);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, App->window->screen_width, App->window->screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, App->window->screen_width, App->window->screen_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG("Error: Framebuffer issue");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ComponentCamera::TranslateCamera(math::float3 direction)
{
	my_go->transform->position += direction * speed * App->time->real_delta_time;
}

void ComponentCamera::RotateCamera()
{
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	math::float3 rotation;
	rotation.x = SDL_cosf(math::DegToRad(yaw)) * SDL_cosf(math::DegToRad(pitch));
	rotation.y = SDL_sinf(math::DegToRad(pitch));
	rotation.z = SDL_sinf(math::DegToRad(yaw)) * SDL_cosf(math::DegToRad(pitch));
	front = rotation.Normalized();

	if(my_go != nullptr && my_go->transform != nullptr)
		LookAt(my_go->transform->position + front);
}

math::float4x4 ComponentCamera::LookAt(math::float3& target)
{
	if (my_go == nullptr || my_go->transform == nullptr)
		return view_matrix;

	math::float3 position = my_go->transform->position;

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

#pragma region Frustrum

const void ComponentCamera::InitFrustum()
{
	frustum.type = math::FrustumType::PerspectiveFrustum;
	frustum.pos = math::float3::zero;
	frustum.front = -math::float3::unitZ;
	frustum.up = math::float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 1000.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov * 0.5f)) *(App->window->screen_width / App->window->screen_height);
	// Calculate horizontal first
	//frustum.horizontalFov = math::pi / 4.0f;
	//frustum.verticalFov = 2.0f * atanf(tanf(frustum.horizontalFov * 0.5f)) *((float)App->window->screen_height / (float)App->window->screen_width);
}

void ComponentCamera::SetFrustum(unsigned& w, unsigned& h)
{
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ((float)w / (float)h));
}

const void ComponentCamera::UpdatePitchYaw()
{
	pitch = -math::RadToDeg(SDL_asinf(-front.y));
	yaw = math::RadToDeg(SDL_atan2f(front.z, front.x));

	if (math::IsNan(pitch))
		pitch = 0.0f;

	if (math::IsNan(yaw))
		yaw = 0.0f;
}

#pragma endregion

#pragma region JSON

JSON_value* ComponentCamera::Save(JSON_value* component) const
{
	JSON_value* camera = Component::Save(component);

	camera->AddVec3("front", front);
	camera->AddVec3("side", side);
	camera->AddVec3("up", up);
	camera->AddFloat("speed", speed);
	camera->AddFloat("rotation_speed", rotation_speed);
	camera->AddVec4x4("proj", proj);
	//TODO: Save also frustrum
	camera->AddVec4x4("view_matrix", view_matrix);
	camera->AddFloat("pitch", pitch);
	camera->AddFloat("yaw", yaw);

	component->addValue("", camera);

	return camera;
}

void ComponentCamera::Load(JSON_value* component)
{
	Component::Load(component);

	front = component->GetVec3("front");
	side = component->GetVec3("side");
	up = component->GetVec3("up");
	speed = component->GetFloat("speed");
	rotation_speed = component->GetFloat("rotation_speed");
	proj = component->GetVec4x4("proj");
	//TODO: Save also frustrum
	view_matrix = component->GetVec4x4("view_matrix");
	pitch = component->GetFloat("pitch");
	yaw = component->GetFloat("yaw");
}

#pragma endregion
