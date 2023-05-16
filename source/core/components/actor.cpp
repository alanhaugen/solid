#include "actor.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/matrix_decompose.hpp>

Actor::Actor()
 //   : uniforms(DEFAULT_HASH_SIZE, Hash<String, UniformData*>::StringHash)
 //   : componentsMap(DEFAULT_HASH_SIZE, Hash<String, IComponent*>::StringHash)
{
    matrix = glm::mat4(1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    forward = glm::vec3(0.0f, 0.0f, -1.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);

    hitbox = physics->CreateHitBox(glm::vec3(1.0f), &matrix);
}

Actor::~Actor()
{
}

void Actor::Add(Component *component)
{
    //components.Append(component);
    //componentsMap.Insert(component->tag, component);

    delete *component->uniforms;
    component->uniforms = uniforms;
    components.Add(component);
}

Component *Actor::FindComponent(const char *tag)
{
    for (unsigned int i = 0; i < components.Size(); i++)
    {
        if ((*components[i])->tag == tag)
        {
            return *components[i];
        }
    }

    return NULL;
    //return componentsMap.Find(tag)->data_;
}

void Actor::UpdateAfterPhysics()
{
    // Update game components after physics update
    for (unsigned int i = 0; i < components.Size(); i++)
    {
        (*components[i])->UpdateAfterPhysics();
    }
}

void Actor::Update()
{
    matrix.matrix[3].x = hitbox->position->x;
    matrix.matrix[3].y = hitbox->position->y;
    matrix.matrix[3].z = hitbox->position->z;

    // Update game components
    for (unsigned int i = 0; i < components.Size(); i++)
    {
        (*components[i])->matrix.subMatrix = matrix.matrix;
        (*components[i])->Update();
        //components[i]->matrix = components[i]->matrix.matrix * glm::affineInverse(matrix.matrix); // pop
    }

    glm::quat rotation = glm::quat_cast(matrix.matrix);

    up = rotation * glm::vec3(0.0f, 1.0f, 0.0f);
    forward = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
    right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);

    /*LinkedList<IComponent*>::Iterator iterator(components.Begin());

    for(; iterator != components.End(); ++iterator) {
        (*iterator)->Update();
    }*/
}
