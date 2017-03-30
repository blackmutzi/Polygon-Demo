#include "deformationmanager.h"

using namespace Engine;


DeformationManager::DeformationManager()
    : ITempDatabase< Deformation >("DeformationManager")
{}

Deformation * DeformationManager::createDeformation(void)
{
    Arch::ComponentBuilder builder;
    Deformation * deformation = builder.createComponent<Deformation>("DeformationManager");
    return deformation;
}

Deformation * DeformationManager::getDeformation(uint container_id)
{
    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();
    Deformation * deformation = (Deformation*) database_manager->getDatabase("DeformationManager")->getContainer( container_id );
    return deformation;
}

void DeformationManager::destroy(uint container_id)
{
    Arch::ComponentBuilder builder;
    builder.deleteComponent( getDeformation( container_id ) );
}

void DeformationManager::createEvent(Entity *deformation_entity, Entity *deformation_tool)
{
    //-----------------------------
    // Deformation Pipeline
    //-----------------------------

    //Phase 1
    Deformation * storage = _deformation_collect_info( deformation_entity , deformation_tool );

    if( storage->isValid() )
    {
        //Phase 2
        _deformation_search( storage );

        if( storage->isValid() )
        {
            //Phase 3
            Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();
            MeshManager        * mesh_manager  = (MeshManager*) database_manager->getDatabase("MeshManager");
            Mesh * mesh_def_entity = mesh_manager->getMesh( deformation_entity->getLocation("mesh").component_id );

            _deformation_new_vertices( storage  , mesh_def_entity );
        }
    }
}

Deformation * DeformationManager::_deformation_collect_info(Entity *deformation_entity, Entity *deformation_tool)
{
    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();
    MeshManager        * mesh_manager  = (MeshManager*) database_manager->getDatabase("MeshManager");

    Deformation * storage  = getDeformation( deformation_entity->getLocation("deformation").component_id );
    Mesh * mesh_def_entity = mesh_manager->getMesh( deformation_entity->getLocation("mesh").component_id );
    Mesh * mesh_def_tool   = mesh_manager->getMesh( deformation_tool->getLocation("mesh").component_id   );

    //--------------------------
    // Check valid Deformation
    //--------------------------
    for( Entity * tool : storage->getDeformationToolList() )
    {
        if( tool->getResourceName() == deformation_tool->getResourceName() )
        {
             storage->setValid( true );
             break;
        }
    }

    if( storage->isValid() )
    {
        //---------------------------
        // Collect Entity Vertices and Indices ...
        //---------------------------

        storage->setDeformationEntityIndices( mesh_def_entity->getIndexVertex() );
        storage->setDeformationEntityVertices( mesh_def_entity->getOriginalVertices()   );

        storage->setDeformationToolIndices( mesh_def_tool->getIndexVertex() );
        storage->setDeformationToolVertices( mesh_def_tool->getOriginalVertices()   );
    }

    return storage;
}


void DeformationManager::_deformation_search( Deformation * storage )
{
    //--------------------------------
    // Get Tool Vertices
    //--------------------------------
    Vector3fv tool_vertices = storage->getDeformationToolVertices();
    Vector3fv tool_vertices_inside;

    //--------------------------------
    // Get Entity Vertices
    //--------------------------------
    Vector3fv entity_vertices = storage->getDeformationEntityVertices();
    Vector3fv entity_vertices_inside;

    //--------------------------------
    // Create Boxen
    //--------------------------------
    FrustumBox entity_box = FrustumUtils::getFrustumBox( entity_vertices );
    FrustumBox tool_box   = FrustumUtils::getFrustumBox( tool_vertices   );

    //--------------------------------
    // Check tool vertices inside from Entity Box
    //--------------------------------
    for( Vector3f tool_vertex : tool_vertices )
    {
        if( FrustumUtils::isVectorInside( entity_box , tool_vertex ) )
        {
            tool_vertices_inside.push_back( tool_vertex );
        }
    }

    //---------------------------------
    // Check Entity Vertices inside from Tool Box
    //---------------------------------
    for( Vector3f entity_vertex : entity_vertices )
    {
        if( FrustumUtils::isVectorInside( tool_box , entity_vertex ) )
        {
            entity_vertices_inside.push_back( entity_vertex );
        }
    }

    //-----------------------------------
    // Phase 3
    // Delete entity_vertices_inside vectors
    // Add    tool_vertices_inside vectors
    //-----------------------------------

    storage->setDeformationEntityVertices( entity_vertices_inside );
    storage->setDeformationToolVertices  ( tool_vertices_inside   );
}

void DeformationManager::_deformation_new_vertices(Deformation *storage , Mesh * mesh_def_entity )
{
    Vector3fv old_vertices = mesh_def_entity->getOriginalVertices();

    //----------------------------------
    // Delete Vertices
    //----------------------------------

    int count = 0;
    for( Vector3f delete_vector :  storage->getDeformationEntityVertices())
    {
        count = 0;

        for( Vector3f original_vector : old_vertices )
        {
            if( delete_vector == original_vector )
            {
                old_vertices.erase( old_vertices.begin() + count );
                break;
            }

            count++;
        }
    }

    //------------------------------
    // Add Vertices
    //------------------------------
    for( Vector3f add_vector : storage->getDeformationToolVertices() )
    {
        old_vertices.push_back( add_vector );
    }

    //------------------------------
    // Save new vertices
    //------------------------------
    storage->setUpdateVertices( old_vertices );
}






