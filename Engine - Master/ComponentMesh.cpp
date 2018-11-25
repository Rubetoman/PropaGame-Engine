#include "ComponentMesh.h"

#include "GameObject.h"
#include "ComponentTransform.h"

ComponentMesh::ComponentMesh(GameObject* go) : Component(go, component_type::Mesh)
{
	mesh = new ModuleModelLoader::mesh();
}


ComponentMesh::~ComponentMesh()
{
}

void ComponentMesh::RenderMesh(unsigned program, const math::float4x4& view, const math::float4x4& proj)
{
	assert(mesh != nullptr);
	//Use shaders loaded in program
	glUseProgram(program);

	math::float4x4 Model(math::float4x4::identity); // Not moving anything
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);

	if (mesh->texture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->texture->id);
		glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	}

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * mesh->num_vertices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

}