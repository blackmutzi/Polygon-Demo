#include "displaceterrain.h"

using namespace Engine;

DisplaceTerrain::DisplaceTerrain()
    :RenderEntity("DisplaceTerrain")
{}

void DisplaceTerrain::create(OpenPolygonDisplay *display)
{
    TerrainGen * terrain = new TerrainGen( Vector3i(257,257,20) );
    terrain->GenerateTerrain( TextureManager::getSingletonPtr()->createTexture("./resource/heightfield.png") );

    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();

    ShaderManager   * shader_manager   = (ShaderManager   *) database_manager->getDatabase("ShaderManager");
    PositionManager * position_manager = (PositionManager *) database_manager->getDatabase("PositionManager");


    m_shader      = shader_manager->createShader("TerrainColourShader");
    shader_manager->addSource( m_shader , "displace" , FRAGMENT | VERTEX );
    m_shader->BindAttributeLocation("points" , 0x9 );
    m_shader->LinkProgram();

    m_entity     = getEntity();
    m_mesh       = terrain->getTerrain();

    m_position = position_manager->createPosition();
    m_position->setPosition( Vector3f(0 , 0 , 0) );

    getEntity()->attach( display->getCamera() );
    getEntity()->attach( m_position );
    getEntity()->attach( m_mesh );
    getEntity()->attach( m_shader );


    TextureUnit * unit = new TextureUnit("test");
    Texture * height_field = unit->loadTexture("./resource/heightfield.png");

    Texture * texture0 = TextureManager::getSingletonPtr()->createTexture("./resource/test_red.png");
    Texture * texture1 = TextureManager::getSingletonPtr()->createTexture("./resource/sand.jpg");
    Texture * texture2 = TextureManager::getSingletonPtr()->createTexture("./resource/grass2.jpg");
    Texture * texture3 = TextureManager::getSingletonPtr()->createTexture("./resource/stein.jpg");

    Texture * texture_array = TextureManager::getSingletonPtr()->createTextureArray("TextureArrayUnitTest",512,512,4);
    TextureManager::getSingletonPtr()->createStorage( texture_array , GL_RGBA32F );

    TextureManager::getSingletonPtr()->addTextureToArray( texture_array , texture1 , 0 );
    TextureManager::getSingletonPtr()->addTextureToArray( texture_array , texture1 , 1 );
    TextureManager::getSingletonPtr()->addTextureToArray( texture_array , texture2 , 2 );
    TextureManager::getSingletonPtr()->addTextureToArray( texture_array , texture3 , 3 );

    m_surface = unit->colour_height_op( height_field , texture_array , 4.0f , 0.1f );
}

void DisplaceTerrain::draw(const DrawEvent &event)
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
        m_shader->BindVec4f ("clipDist"    , event.getClipDistance() );


        m_shader->BindUniform1f("width" ,257.0f);
        m_shader->BindUniform1f("height",257.0f);

        m_shader->BindTexture( m_surface  , "image" , 0 );

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
