#include "basicshadowmodul.h"

using namespace Engine;

BasicShadowModul::BasicShadowModul()
    : RenderModul()
{
    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();

    ShaderManager   * shader_manager   = (ShaderManager   *) database_manager->getDatabase("ShaderManager");

    //---------------------------------------------------
    // Basic Shadow Shader - for Entity Rendering
    //---------------------------------------------------
    m_shadow_shader  = shader_manager->createShader("BasicShadowRendering");
    shader_manager->addSource( m_shadow_shader  , "shadow" , FRAGMENT | VERTEX  );
    m_shadow_shader->LinkProgram();
}

void BasicShadowModul::renderProcess( const DrawEvent & event )
{
    Entity   * current_entity    = nullptr;
    Position * current_position  = nullptr;
    Mesh     * current_mesh      = nullptr;

    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();
    MeshManager     * mesh_manager     = (MeshManager     *) database_manager->getDatabase("MeshManager");
    PositionManager * position_manager = (PositionManager *) database_manager->getDatabase("PositionManager");


    //Use Shader Program
    m_shadow_shader->UseProgram();

    //Classic: Ortho Matrix ( Spot: Perspective Matrix )
    m_shadow_shader->BindMatrix("Projection", event.getProjection() );

    //DrawElements Mode ( TODO: Check DrawElements Modul )
    m_shadow_shader->BindUniform1f("Mode",0.0f);

    for( RenderEntity * render_entity : m_render_entity_manager->getEntitys() )
    {
        current_entity    = render_entity->getEntity();
        current_position  = position_manager->getPosition( current_entity->getLocation("position").component_id );
        current_mesh      = mesh_manager->getMesh( current_entity->getLocation("mesh").component_id );

        current_position->transformation();

        //---------------------------------
        // Setup Model View Matrix
        // Light View Matrix * Model Matrix = ModelView
        //---------------------------------
        m_shadow_shader->BindMatrix("ModelView", event.getWorldView() * current_position->getMatrix() );

        //Draw Object

        glCullFace( GL_BACK );

        current_mesh->Draw();

        glCullFace( GL_FRONT );

        // if has DrawElementsIndirect Modul
        //
        //  getShadowShader()->BindMatrix   ("LightView" , getShadowEvent().getViewMatrix() );
        //  getShadowShader()->BindUniform1f("Mode",1.0f);
        //  current_mesh->DrawElementsIndirect( draw_count ); // draw_count come from Component Modul
    }

    m_shadow_shader->Unused();
}
