#include "ComponentMesh.h"

#include "ComponentTransform.h"

#include "GL/glew.h"

ComponentMesh::ComponentMesh(GameObject* go) : Component(go, component_type::Mesh)
{
}

ComponentMesh::ComponentMesh(const ComponentMesh& comp) : Component(comp)
{
	// TODO: Create new buffers or not delete when they are used by other comp
	vbo = comp.vbo;
	ibo = comp.ibo;
	vao = comp.vao;
	num_vertices = comp.num_vertices;
	num_indices = comp.num_indices;
}

ComponentMesh::~ComponentMesh()
{
}

Component* ComponentMesh::Duplicate()
{
	return new ComponentMesh(*this);
}

void ComponentMesh::CleanUp()
{
	Component::CleanUp();
	DeleteMesh();
}

void ComponentMesh::DrawOnInspector()
{
	ImGui::PushID(this);
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Mesh Component"))
	{
		Component::DrawOnInspector();
		ImGui::Text("Triangles Count: %d", num_indices / 3);
		ImGui::Text("Vertices Count: %d", num_vertices);
	}
	ImGui::PopID();
}

void ComponentMesh::RenderMesh(unsigned program, const Texture* texture, const math::float4x4& view, const math::float4x4& proj)
{
	glActiveTexture(GL_TEXTURE0);
	if (texture != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, texture->id);
	}
	glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);

	// Disable VAO
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ComponentMesh::DeleteMesh()
{
	if (vbo != 0)
	{
		glDeleteBuffers(1, &vbo);
	}

	if (ibo != 0)
	{
		glDeleteBuffers(1, &ibo);
	}

	if (vao != 0)
	{
		glDeleteBuffers(1, &vao);
	}
}

void ComponentMesh::Delete()
{
	my_go->mesh = nullptr;
	Component::Delete();
}