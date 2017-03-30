#include "contextcontainertest.h"

using namespace Engine;

ContextContainerTest::ContextContainerTest( WaveFrontLoader & importer )
  : m_loader( importer )
{}

void ContextContainerTest::loadContainer()
{
    //---------------------------------
    // Component Databases ..
    //---------------------------------
    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();
    MeshManager     * mesh_manager     = (MeshManager     *) database_manager->getDatabase("MeshManager");

    m_result = mesh_manager->createVertexArrayObject( m_loader );
}

GLVertexArrayObject * ContextContainerTest::getObject(void)
{
    return m_result;
}



