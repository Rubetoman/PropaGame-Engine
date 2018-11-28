#include "ComponentMesh.h"

#include "ComponentTransform.h"

#include "GL/glew.h"

ComponentMesh::ComponentMesh(GameObject* go) : Component(go, component_type::Mesh)
{
}


ComponentMesh::~ComponentMesh()
{
}

void ComponentMesh::CleanUp()
{
	DeleteMesh();
}

void ComponentMesh::DrawOnInspector()
{
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Mesh Component"))
	{
		Component::DrawOnInspector();
		ImGui::Text("Triangles Count: %d", num_indices / 3);
		ImGui::Text("Vertices Count: %d", num_vertices);
	}
}

void ComponentMesh::RenderMesh(unsigned program, const Texture* texture, const math::float4x4& view, const math::float4x4& proj)
{
	if (texture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->id);
		glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	}

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