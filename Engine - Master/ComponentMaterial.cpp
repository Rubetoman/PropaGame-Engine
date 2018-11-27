#include "ComponentMaterial.h"

#include "GL/glew.h"

ComponentMaterial::ComponentMaterial(GameObject* go) : Component(go, component_type::Material)
{
}


ComponentMaterial::~ComponentMaterial()
{
}
