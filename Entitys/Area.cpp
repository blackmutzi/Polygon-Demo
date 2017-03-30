#include "area.h"

using namespace Engine;

Area::Area()
    : m_mesh( nullptr )     ,
      m_position( nullptr ) ,
      m_shader( nullptr )   ,
      m_surface( nullptr )  ,
      RenderEntity("Area")
{}

Area::~Area(){}

void Area::create( OpenPolygonDisplay * display )
{
    //---------------------------------
    // Component Databases ..
    //---------------------------------
    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();

    ShaderManager   * shader_manager   = (ShaderManager   *) database_manager->getDatabase("ShaderManager");
    MeshManager     * mesh_manager     = (MeshManager     *) database_manager->getDatabase("MeshManager");
    PositionManager * position_manager = (PositionManager *) database_manager->getDatabase("PositionManager");


    m_mesh        = mesh_manager->createMesh("./resource/terrain.obj");
    m_position    = position_manager->createPosition();

    /* Texture Shader */
    m_shader      = shader_manager->createShader("Texture");
    shader_manager->addSource( m_shader , "texture" , FRAGMENT | VERTEX  );
    m_shader->LinkProgram();

    /* Attach Component to Entity */
    getEntity()->attach( m_mesh );
    getEntity()->attach( m_position );
    getEntity()->attach( m_shader );
    getEntity()->attach( CameraManager::getSingletonPtr()->getCamera( MAIN_CAMERA ));
}

void Area::draw(const DrawEvent &event)
{
    m_position->transformation();

    m_shader->UseProgram();

    m_shader->BindMatrix("Projection"  , event.getProjection() );
    m_shader->BindMatrix("ModelView"   , event.getWorldView() * m_position->getMatrix() );
    m_shader->BindMatrix("ModelMatrix" , m_position->getMatrix() );
    m_shader->BindMatrix("WorldView"   , event.getWorldView() );
    m_shader->BindMatrix("NormalMatrix", m_position->getNormalMatrix( event.getWorldView() * m_position->getMatrix() ) );

    m_shader->BindUniform1f("TexCoordModus" , 8.0f );
    m_shader->BindTexture( m_surface , "image" , 0 );

    m_mesh->DrawElements();

    m_shader->Unused();
}

void Area::setTexture( Texture * surface )
{
    if( !surface->hasState( Texture::LOADED ))
                LogManager::log( LogManager::LOG_ERROR , "Area surface texture is not loaded ");

    m_surface = surface;
}

void Area::setPosition(const Vector3f &position)
{
    m_position->setPosition( position );
}

void Area::setScale(const Vector3f &scale)
{
    m_position->setScale( scale );
}

void Area::setRotation(const Vector3f & rotation )
{
    m_position->setRotation( rotation );
}
