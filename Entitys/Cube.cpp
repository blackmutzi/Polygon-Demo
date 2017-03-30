#include "cube.h"

using namespace Engine;

Cube::Cube()
    : m_mesh( nullptr )     ,
      m_position( nullptr ) ,
      m_shader( nullptr )   ,
      m_surface( nullptr )  ,
      RenderEntity("Cube")
{}

Cube::~Cube(){}

void Cube::create( OpenPolygonDisplay * display )
{
    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();

    ShaderManager   * shader_manager   = (ShaderManager   *) database_manager->getDatabase("ShaderManager");
    MeshManager     * mesh_manager     = (MeshManager     *) database_manager->getDatabase("MeshManager");
    PositionManager * position_manager = (PositionManager *) database_manager->getDatabase("PositionManager");

    m_position = position_manager->createPosition();
    m_mesh     = mesh_manager->createMesh("./resource/cube.obj");

    m_shader    = shader_manager->createShader("Cube");
    shader_manager->addSource( m_shader , "cube" , FRAGMENT | VERTEX  );
    m_shader->LinkProgram();

    //Attach
    getEntity()->attach( m_mesh );
    getEntity()->attach( m_position );
    getEntity()->attach( m_shader );
    getEntity()->attach( CameraManager::getSingletonPtr()->getCamera( MAIN_CAMERA ));
}

void Cube::draw(const DrawEvent &event)
{
    m_position->transformation();

    if( !event.hasShadowEvent() )
    {
        /* Use Cube Shader */
        m_shader->UseProgram();

        m_shader->BindMatrix("Projection"  , event.getProjection() );
        m_shader->BindMatrix("ModelView"   , event.getWorldView() * m_position->getMatrix() );
        m_shader->BindMatrix("ModelMatrix" , m_position->getMatrix() );
        m_shader->BindMatrix("WorldView"   , event.getWorldView() );
        m_shader->BindMatrix("TexMatrix"   , glm::mat4(1.0f) );
        m_shader->BindMatrix("NormalMatrix", m_position->getNormalMatrix( event.getWorldView() * m_position->getMatrix() ) );

        //Bind Cube Texture
        m_shader->BindTexture( m_surface , "surface" , 0 );

        m_mesh->DrawElements();
        m_shader->Unused();

    }else /* Flashlight Shadow Event */
    {
        glm::mat4 modelview =  event.getShadowEvent().getViewMatrix() * m_position->getMatrix();

        event.getShadowEvent().getShadowShader()->BindMatrix("ModelView", modelview );

        m_mesh->DrawElements();
    }
}

void Cube::setSingleCubeTexture( Texture * texture )
{
    if( texture->hasState( Texture::LOADED ) )
         LogManager::log( LogManager::LOG_ERROR , "Cube - texture is loaded  ");

    Textures textures;

    textures.push_back( texture );
    textures.push_back( texture );
    textures.push_back( texture );
    textures.push_back( texture );
    textures.push_back( texture );
    textures.push_back( texture );

    setCubeTextures( textures );
}

void Cube::setCubeTextures(Textures textures)
{
    if( textures.size() != 6 )
           LogManager::log(LogManager::LOG_ERROR , "Cube - texture array must be size of 6 ");

    m_surface = TextureManager::getSingletonPtr()->createCubemapping( textures , "cube_new_surface" );
}
