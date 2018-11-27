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
	/*if (ImGui::CollapsingHeader("Texture"))
	{
		if (texture != 0)
		{
			ImGui::Text("Texture name: %s", texture->name);
			ImGui::Text("Texture Size:\n Width: %d | Height: %d", texture->width, texture->height);
			float panelWidth = ImGui::GetWindowContentRegionWidth();
			float conversionFactor = panelWidth / texture->width;
			ImVec2 imageSize = { texture->height *conversionFactor, panelWidth };
			ImGui::Image((ImTextureID)texture->id, imageSize);
		}
		else
			ImGui::Text("No texture");
	}*/
}

void ComponentMesh::RenderMesh(unsigned program, const Texture* texture, const math::float4x4& view, const math::float4x4& proj)
{
	//Use shaders loaded in program
	glUseProgram(program);

	math::float4x4 Model(math::float4x4::identity); // Not moving anything
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);

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
	glUseProgram(0);
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