#include "ComponentCamera.h"

#include "ModuleCamera.h"

#include "Application.h"
#include "GameObject.h"

ComponentCamera::ComponentCamera(GameObject* go) : Component(go, component_type::Camera)
{
	camera = new Camera();
	camera->InitFrustum();
	CreateFrameBuffer();
	if (go != nullptr)
		camera->frustum.pos = go->boundingBox.CenterPoint();
}

ComponentCamera::ComponentCamera(const ComponentCamera& comp) : Component(comp)
{
	camera = new Camera();
	camera = comp.camera;
}

ComponentCamera::~ComponentCamera()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteTextures(1, &renderedTexture);
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
			ImGui::SliderFloat("Near Plane Distance", &camera->frustum.nearPlaneDistance, 0.0f, camera->frustum.farPlaneDistance);
			ImGui::SliderFloat("Far Plane Distance", &camera->frustum.farPlaneDistance, camera->frustum.nearPlaneDistance, 3000.0f);
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