#include "ComponentCamera.h"

#include "ModuleCamera.h"
#include "ModuleTime.h"
#include "ModuleEditor.h"
#include "ModuleResources.h"

#include "ComponentTransform.h"

#include "Application.h"
#include "GameObject.h"

ComponentCamera::ComponentCamera(GameObject* go) : Component(go, component_type::Camera)
{
	InitFrustum();
	CreateFrameBuffer();
	if (go != nullptr)
		frustum.pos = go->boundingBox.CenterPoint();

	// Avoid creating another window for editor camera
	if(go != App->camera->editor_camera_go)
		window = App->editor->CreateCameraWindow(*this);
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
		App->resources->DeleteCamera(this);
}

void ComponentCamera::Update()
{
	//TODO: Set camera dirty when changes and recalculate frustum
	if (my_go == nullptr) return;
	if (my_go->transform == nullptr) return;

	math::float4x4 transform = my_go->GetGlobalTransform();
	frustum.pos = transform.TranslatePart();

	// Avoid updating editor camera
	if (type != component_type::Editor_Camera)
	{
		frustum.front = -transform.RotatePart().Mul(math::float3::unitZ).Normalized();
		frustum.up = transform.RotatePart().Mul(math::float3::unitY).Normalized();
	}
}

Component* ComponentCamera::Duplicate()
{
	return new ComponentCamera(*this);
}

void ComponentCamera::Delete()
{
	App->resources->DeleteCamera(this);
	Component::Delete();
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
			//ImGui::SliderFloat("Aspect Ratio", &frustum.AspectRatio(), 0, 179);
			ImGui::SliderFloat("FOV", &frustum.horizontalFov, 0.0f, 3.14f);

			// View mode
			const char* items[] = { "Invalid", "Orthographic", "Perspective" };
			ImGui::Combo("View type", (int*)&frustum.type, items, IM_ARRAYSIZE(items));
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

void ComponentCamera::Rotate(float dx, float dy)
{
	if (dx != 0) 
	{
		math::Quat rotation = math::Quat::RotateY(math::DegToRad(-dx)).Normalized();
		frustum.front = rotation.Mul(frustum.front).Normalized();
		frustum.up = rotation.Mul(frustum.up).Normalized();

		//TODO: Apply changes to transform
		//my_go->transform->SetRotation( my_go->transform->rotation * rotation);

	}

	if (dy != 0) 
	{
		math::Quat rotation = math::Quat::RotateAxisAngle(frustum.WorldRight(), math::DegToRad(-dy)).Normalized();
		math::float3 validUp = rotation.Mul(frustum.up).Normalized();
		// Avoiding gimbal lock
		if (validUp.y > 0.0f) 
		{
			frustum.up = validUp;
			frustum.front = rotation.Mul(frustum.front).Normalized();
		}
		//my_go->transform->SetRotation(my_go->transform->rotation * rotation);
	}
}

void ComponentCamera::Orbit(float dx, float dy) 
{
	// TODO: set up the orbit when no GO is selected in front of the camera
	if (App->editor->hierarchy->selected == nullptr) return;

	AABB& bbox = App->editor->hierarchy->selected->boundingBox;
	math::float3 center = bbox.CenterPoint();

	if (dx != 0) 
	{
		math::Quat rotation = math::Quat::RotateY(math::DegToRad(-dx)).Normalized();
		frustum.pos = rotation.Mul(frustum.pos);
	}

	if (dy != 0) 
	{
		math::Quat rotation = math::Quat::RotateAxisAngle(frustum.WorldRight(), math::DegToRad(-dy)).Normalized();
		math::float3 new_pos = rotation.Mul(frustum.pos);
		if (!(abs(new_pos.x - center.x) < 0.5f && abs(new_pos.z - center.z) < 0.5f)) {
			frustum.pos = new_pos;
		}
	}

	my_go->transform->position = frustum.pos;

	LookAt(center);
}

void ComponentCamera::LookAt(math::float3& target)
{
	math::float3 dir = (target - frustum.pos).Normalized();
	math::float3x3 look = math::float3x3::LookAt(frustum.front, dir, frustum.up, math::float3::unitY);
	frustum.front = look.Mul(frustum.front).Normalized();
	frustum.up = look.Mul(frustum.up).Normalized();
}

#pragma region Frustrum

const void ComponentCamera::InitFrustum()
{
	frustum.type = math::FrustumType::PerspectiveFrustum;
	frustum.pos = my_go->transform->position;
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

#pragma endregion

#pragma region JSON

JSON_value* ComponentCamera::Save(JSON_value* component) const
{
	JSON_value* camera = Component::Save(component);

	camera->AddFloat("speed", speed);
	camera->AddFloat("rotation_speed", rotation_speed);
	//TODO: Save also frustrum

	component->addValue("", camera);

	return camera;
}

void ComponentCamera::Load(JSON_value* component)
{
	Component::Load(component);

	speed = component->GetFloat("speed");
	rotation_speed = component->GetFloat("rotation_speed");
	//TODO: Load also frustrum
}

#pragma endregion
