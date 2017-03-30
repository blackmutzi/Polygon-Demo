#include "deformationmodul.h"

using namespace Engine;

DeformationModul::DeformationModul(){}

void DeformationModul::renderProcess(const DrawEvent &event)
{
    Entity      * current_entity = nullptr;
    Mesh        * current_mesh   = nullptr;
    Deformation * current_deformation = nullptr;

    Arch::DatabaseManager * database_manager  = Arch::DatabaseManager::getSingletonPtr();
    MeshManager        * mesh_manager         = (MeshManager     *) database_manager->getDatabase("MeshManager");
    DeformationManager * deformation_manager  = (DeformationManager *) database_manager->getDatabase("DeformationManager");


    for( RenderEntity * render_entity : m_render_entity_manager->getEntitys() )
    {
        current_entity = render_entity->getEntity();

        if( current_entity->isAttached("deformation") )
        {
            current_deformation = deformation_manager->getDeformation( current_entity->getLocation("deformation").component_id );
            current_mesh = mesh_manager->getMesh( current_entity->getLocation("mesh").component_id );

            if( current_deformation->isValid() )
            {
                Vector3fv update_vertices = current_deformation->getUpdateVertices();
                current_mesh->getVertexArrayObject()->getAttachBuffer( ATTR_BUFFER_VERTICES )->update( 0 , update_vertices.size() * 3 * sizeof(float) , update_vertices.data() );

                //-----------------------
                // Change Status
                //-----------------------
                current_deformation->setValid( false );

            }

        }
    }

}
