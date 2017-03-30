#include "waterterrain.h"

using namespace Engine;

WaterTerrain::WaterTerrain()
    :RenderEntity("WaterTerrain")
{}

void WaterTerrain::create(OpenPolygonDisplay *display)
{
    TerrainGen * terrain = new TerrainGen( Vector3i(257,257,20) );
    terrain->GenerateTerrain();

    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();

    ShaderManager   * shader_manager   = (ShaderManager   *) database_manager->getDatabase("ShaderManager");
    MeshManager     * mesh_manager     = (MeshManager     *) database_manager->getDatabase("MeshManager");
    PositionManager * position_manager = (PositionManager *) database_manager->getDatabase("PositionManager");


    m_shader      = shader_manager->createShader("TerrainColourShader");
    shader_manager->addSource( m_shader , "displace" , FRAGMENT | VERTEX  );
    m_shader->BindAttributeLocation("points" , 0x9 );
    m_shader->LinkProgram();

    m_mesh = terrain->getTerrain();

    m_position = position_manager->createPosition();

    getEntity()->attach( display->getCamera() );
    getEntity()->attach( m_position );
    getEntity()->attach( m_mesh );
    getEntity()->attach( m_shader );

    m_water_surface = TextureManager::getSingletonPtr()->createTexture("./resource/test_blue2.png"); //./resource/glz_14.jpg
    TextureManager::getSingletonPtr()->loadTexture( m_water_surface , true );
}

void WaterTerrain::setPosition( const Vector3f & position )
{
    m_position->setPosition( position );
}

void WaterTerrain::draw(const DrawEvent &event)
{
    m_position->transformation();

    if( !event.hasShadowEvent() )
    {
        m_shader->UseProgram();

        m_shader->BindMatrix("Projection"  , event.getProjection() );
        m_shader->BindMatrix("ModelView"   , event.getWorldView() * m_position->getMatrix() );
        m_shader->BindMatrix("ModelMatrix" , m_position->getMatrix() );
        m_shader->BindMatrix("WorldView"   , event.getWorldView() );
        m_shader->BindMatrix("NormalMatrix", m_position->getNormalMatrix( event.getWorldView() * m_position->getMatrix() ));

        m_shader->BindUniform1f("width" ,257.0f);
        m_shader->BindUniform1f("height",257.0f);

        m_shader->BindTexture( m_water_surface  , "image" , 0 );

        m_mesh->Draw();

        m_shader->Unused();

     }else{

        glCullFace( GL_BACK );

        event.getShadowEvent().getShadowShader()->BindMatrix("ModelView" ,  event.getShadowEvent().getViewMatrix() * m_position->getMatrix() );
        event.getShadowEvent().getShadowShader()->BindUniform1f("Mode",0.0f);

        m_mesh->Draw();

        glCullFace(GL_FRONT);

     }
}
