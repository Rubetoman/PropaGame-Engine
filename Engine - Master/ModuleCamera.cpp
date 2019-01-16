#include "ModuleCamera.h"

#include "Application.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "ModuleTime.h"
#include "ModuleResources.h"

#include "WindowScene.h"

#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"

#include "GameObject.h"
#include "Quadtree.h"
#include "debugdraw.h"
#include "ImGuizmo/ImGuizmo.h"

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	editor_camera_go = new GameObject("Editor Camera");
	editor_camera_go->transform->position = math::float3(0.0f, 0.0f, 3.0f * App->editor->scale);
	editor_camera_comp = (ComponentCamera*)editor_camera_go->CreateComponent(component_type::Editor_Camera);
	App->window->SetWindowSize(App->window->screen_width, App->window->screen_height, true);
	App->scene->scene_gos.push_back(editor_camera_go);

	return true;
}

bool ModuleCamera::Start()
{
	editor_camera_comp->LookAt(math::float3(0.0f, 0.0f, 0.0f));
	return true;
}

update_status ModuleCamera::PreUpdate() 
{
	if (App->editor->scene->IsFocused())
	{
		TranslateCameraInput();
		RotateCameraInput();
		CameraSpeedInput(3.0f);
	}

	// Check when the mouse is right or left click is up
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP || App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	{
		SDL_ShowCursor(SDL_ENABLE);
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	editor_camera_comp->Update();

	for (auto camera : App->resources->cameras)
	{
		camera->Update();
	}

	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::UpdateScreenSize() 
{
	if (editor_camera_comp != nullptr)
	{
		editor_camera_comp->frustum.horizontalFov = 2.0f * atanf(tanf(editor_camera_comp->frustum.verticalFov * 0.5f)) *(App->window->screen_width / App->window->screen_height);
		editor_camera_comp->frustum.verticalFov = 2.0f * atanf(tanf(editor_camera_comp->frustum.horizontalFov * 0.5f)) *(App->window->screen_width / App->window->screen_height);
	}
}

void ModuleCamera::TranslateCameraInput() 
{
	if (editor_camera_comp == nullptr)
		return;

	math::float3 movement = math::float3::zero;
	float magnitude = editor_camera_comp->speed * App->editor->scale * App->time->real_delta_time;

	// Right click + WASD/QE translates the camera
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_Q))
		{
			movement += math::float3::unitY * magnitude;
		}
		else if (App->input->GetKey(SDL_SCANCODE_E))
		{
			movement -= math::float3::unitY * magnitude;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A))
		{
			movement -= editor_camera_comp->frustum.WorldRight() * magnitude;
		}
		else if (App->input->GetKey(SDL_SCANCODE_D))
		{
			movement += editor_camera_comp->frustum.WorldRight() * magnitude;
		}
		else if (App->input->GetKey(SDL_SCANCODE_W))
		{
			movement += editor_camera_comp->frustum.front * magnitude;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S))
		{
			movement -= editor_camera_comp->frustum.front * magnitude;
		}
		editor_camera_go->transform->position += movement;
	}

	// Mouse wheel go forward or backwards
	if (App->input->GetMouseButtonDown(SDL_BUTTON_X1) == KEY_DOWN)
	{
		WheelInputTranslation(App->input->GetMouseWheel());
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && !App->editor->scene->gui_click && App->input->GetKey(SDL_SCANCODE_LALT) != KEY_REPEAT)
	{
		App->editor->selectedGO = MousePick();
	}
}

void ModuleCamera::RotateCameraInput() 
{
	if (editor_camera_comp == nullptr)
		return;

	float magnitude = editor_camera_comp->rotation_speed;

	// ALT + mouse left click + mouse move orbit around the loaded mesh
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) 
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			SDL_ShowCursor(SDL_DISABLE);
			math::float3 center = math::float3::zero;
			if (App->editor->selectedGO != nullptr)
				center = App->editor->selectedGO->GetCenter();

			editor_camera_comp->Orbit(center, App->input->GetMouseMotion().x * magnitude, App->input->GetMouseMotion().y * magnitude);
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) 
		{
			SDL_ShowCursor(SDL_ENABLE);
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_F))
	{
		GameObject* selected = App->editor->selectedGO;
		if (selected != nullptr)
			editor_camera_comp->LookAt(selected->GetCenter());
		else
			editor_camera_comp->LookAt(math::float3(0.0f, 0.0f, 0.0f));
	}
	if(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT))
	{
		SDL_ShowCursor(SDL_DISABLE);
		editor_camera_comp->Rotate(App->input->GetMouseMotion().x * magnitude, App->input->GetMouseMotion().y * magnitude);
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
	{
		SDL_ShowCursor(SDL_ENABLE);
	}
}

void ModuleCamera::CameraSpeedInput(float modifier) 
{
	if (editor_camera_comp == nullptr)
		return;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
	{
		editor_camera_comp->speed *= modifier;
		editor_camera_comp->rotation_speed *= modifier;
	}
	else if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP)
	{
		editor_camera_comp->speed /= modifier;
		editor_camera_comp->rotation_speed /= modifier;
	}
}

void ModuleCamera::WheelInputTranslation(const fPoint& wheel_motion)
{
	if (editor_camera_go == nullptr)
		return;

	float magnitude = editor_camera_comp->speed * App->editor->scale * App->time->real_delta_time;
	editor_camera_go->transform->position += editor_camera_comp->frustum.front.Mul(wheel_motion.y) * 10 * magnitude;
}

void ModuleCamera::FitCamera(const AABB &boundingBox)
{
	if (editor_camera_go == nullptr || editor_camera_comp == nullptr)
		return;

	math::float3 diagonal = boundingBox.Diagonal();
	math::float3 center = boundingBox.CenterPoint();
	editor_camera_go->transform->position.z = (center.z + diagonal.Length());
	editor_camera_go->transform->position.y = center.y;
	editor_camera_go->transform->position.x = center.x;
	editor_camera_comp->LookAt(math::float3(center.x,center.y,center.z));
}

GameObject* ModuleCamera::MousePick()
{
	const fPoint mousePosition = App->input->GetMousePosition();

	float normalized_x = mousePosition.x * App->window->screen_width - App->editor->scene->viewport.x;
	float normalized_y = mousePosition.y * App->window->screen_height - App->editor->scene->viewport.y;
	normalized_x = (normalized_x / (App->window->screen_width / 2)) - 1.0f;
	normalized_y = 1.0f - (normalized_y / (App->window->screen_height / 2));

	raycast = editor_camera_comp->frustum.UnProjectLineSegment(normalized_x, normalized_y);

	hitGOs.clear();
	App->scene->quadtree->Intersect(hitGOs, raycast);

	for (auto mesh : App->resources->meshes)
	{
		if (!mesh->my_go->static_GO && mesh->mesh.num_vertices > 0 && raycast.Intersects(mesh->my_go->ComputeBBox()))
		{
			hitGOs.push_back(mesh->my_go);
		}
	}

	// Check which GO is nearest
	float minDistance = raycast.Length();
	GameObject* nearest_hit_GO = nullptr;
	for (auto go : hitGOs)
	{
		Mesh* mesh = &go->mesh_comp->mesh;
		ComponentTransform* transform = go->transform;

		if (mesh != nullptr && transform != nullptr)
		{
			math::LineSegment localTransformPikingLine(raycast);
			localTransformPikingLine.Transform(transform->my_go->GetGlobalTransform().Inverted());

			math::Triangle triangle;
			for (unsigned i = 0; i < mesh->num_indices; i += 3)
			{
				triangle.a = { mesh->vertices[mesh->indices[i] * 3], mesh->vertices[mesh->indices[i] * 3 + 1], mesh->vertices[mesh->indices[i] * 3 + 2] };
				triangle.b = { mesh->vertices[mesh->indices[i + 1] * 3],mesh->vertices[mesh->indices[i + 1] * 3 + 1], mesh->vertices[mesh->indices[i + 1] * 3 + 2] };
				triangle.c = { mesh->vertices[mesh->indices[i + 2] * 3], mesh->vertices[mesh->indices[i + 2] * 3 + 1], mesh->vertices[mesh->indices[i + 2] * 3 + 2] };

				float triangleDistance;
				float3 hitPoint;
				if (localTransformPikingLine.Intersects(triangle, &triangleDistance, &hitPoint))
				{
					if (triangleDistance < minDistance)
					{
						minDistance = triangleDistance;
						nearest_hit_GO = go;
					}
				}
			}
		}
	}

	return nearest_hit_GO;
}

void ModuleCamera::DrawRaycast() const
{
	dd::line(raycast.a, raycast.b, App->editor->raycast_color);
}