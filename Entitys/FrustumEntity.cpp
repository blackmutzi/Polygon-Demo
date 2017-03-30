#include "frustumentity.h"

using namespace Engine;

FrustumEntity::FrustumEntity()
    : m_mesh( nullptr )     ,
      m_position( nullptr ) ,
      m_shader( nullptr )   ,
      RenderEntity("Frustum")
{}

void FrustumEntity::create(OpenPolygonDisplay *display)
{
    m_display  = display;
    m_entity   = getEntity();

    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();

    ShaderManager   * shader_manager   = (ShaderManager   *) database_manager->getDatabase("ShaderManager");
    MeshManager     * mesh_manager     = (MeshManager     *) database_manager->getDatabase("MeshManager");
    PositionManager * position_manager = (PositionManager *) database_manager->getDatabase("PositionManager");


    m_position = position_manager->createPosition();

    Vector3fv vertices = FrustumUtils::createFrustumVertices( -1.0 , 300.0 );
    m_mesh             = mesh_manager->createMesh("Frustum", vertices);


    m_shader = shader_manager->createShader("ColourShader");
    shader_manager->addSource( m_shader , "color" , FRAGMENT | VERTEX );
    m_shader->LinkProgram();

    m_entity->attach( m_mesh     );
    m_entity->attach( m_position );
    m_entity->attach( m_shader   );
    m_entity->attach( display->getCamera() );
}

void FrustumEntity::draw(const DrawEvent &event)
{

    Vector3f centroid0  = FrustumUtils::getCentroid3f( 1.0 , 1000.0 , glm::inverse( event.getWorldView() ) );

    Vector3f camera_position = m_display->getCamera()->getPosition();
    camera_position.inverse();

    //centroid0.setY( camera_position.getY() );
    //centroid0 += Vector3f(0,20,0);

    double  nearDist = -1.0;
    double  farDist  = 100.0;
    double  distance = (farDist - nearDist) / 2;
    double  z        = nearDist - distance;

    centroid0.setY( camera_position.getY() );
    centroid0 += Vector3f( 0, 30 , 0 );


    m_position->setPosition( centroid0 );
    m_position->setRotation( Vector3f( -90.0f , 0 , 0 ) );
    m_position->transformation();

    if( !event.hasShadowEvent() )
    {

    glDisable( GL_CULL_FACE );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    m_shader->UseProgram();

    m_shader->BindMatrix("Projection"  , event.getProjection() );
    m_shader->BindMatrix("ModelView"   , event.getWorldView() * m_position->getMatrix() );
    m_shader->BindMatrix("ModelMatrix" , m_position->getMatrix() );
    m_shader->BindMatrix("WorldView"   , event.getWorldView() );
    m_shader->BindMatrix("TexMatrix"   , glm::mat4(1.0f) );
    m_shader->BindMatrix("NormalMatrix", m_position->getNormalMatrix( event.getWorldView() * m_position->getMatrix() ) );

    m_shader->BindVec4f("colour", Vector4f( 1 , 1 , 0 , 1) );

    m_mesh->Draw();

    m_shader->Unused();

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glEnable( GL_CULL_FACE );

    }else
    {

    }
}
