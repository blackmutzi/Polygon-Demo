#include "deformation.h"

using namespace Engine;

Deformation::Deformation()
    : IComponent("deformation") ,
      m_valid(false)
{}

void Deformation::addDeformationTool(Entity *entity)
{
    m_tool_list.push_back( entity );
}

DeformationToolList Deformation::getDeformationToolList()
{
    return m_tool_list;
}

void Deformation::setValid( bool valid )
{
    m_valid = valid;
}

bool Deformation::isValid(void)
{
    return m_valid;
}

Vector3fv Deformation::getDeformationToolVertices()
{
    return m_def_tool_vertices;
}

Vector3fv Deformation::getDeformationEntityVertices()
{
    return m_def_entity_vertices;
}

void Deformation::setDeformationEntityIndices(Vector3fv indices)
{
    m_def_entity_indices = indices;
}

void Deformation::setDeformationEntityVertices(Vector3fv vertices)
{
    m_def_entity_vertices = vertices;
}

void Deformation::setDeformationToolIndices(Vector3fv indices)
{
    m_def_tool_indices = indices;
}

void Deformation::setDeformationToolVertices(Vector3fv vertices)
{
    m_def_tool_vertices = vertices;
}

void Deformation::setUpdateVertices( Vector3fv update )
{
    m_update_vertices = update;
}

Vector3fv Deformation::getUpdateVertices(void)
{
    return m_update_vertices;
}




