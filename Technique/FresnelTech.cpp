#include "fresneltech.h"

using namespace Engine;

FresnelTech::FresnelTech( const std::string & tech_name )
    : GLTechnique(tech_name)
{}

void FresnelTech::Create()
{
    m_display        = m_system->getDisplay();         // Get Display by RenderSystem
    m_projection     = m_display->getPerspective();

    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();
    ShaderManager   * shader_manager   = (ShaderManager   *) database_manager->getDatabase("ShaderManager");
    MeshManager     * mesh_manager     = (MeshManager     *) database_manager->getDatabase("MeshManager");
    PositionManager * position_manager = (PositionManager *) database_manager->getDatabase("PositionManager");

    //---------------------------------------------------
    // Basic Shadow Shader - for Entity (Depth Texture) Rendering
    //---------------------------------------------------

    m_basic_shadow  = shader_manager->createShader("Fresnel_Depth_Shader");
    shader_manager->addSource( m_basic_shadow  , "shadow" , FRAGMENT | VERTEX );
    m_basic_shadow->LinkProgram();

    //---------------------------------------------------
    // Fresnel Shader
    //---------------------------------------------------

    m_fresnel_shader  = shader_manager->createShader("Fresnel");
    shader_manager->addSource( m_fresnel_shader , "fresnel" , VERTEX | FRAGMENT );
    m_fresnel_shader->LinkProgram();


    //---------------------------------------------------
    // Create Depth Texture Framebuffer
    //---------------------------------------------------
    m_depth_fbo = FrameBufferManager::getSingletonPtr()->createDepthBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );

    m_reflection_fbo = FrameBufferManager::getSingletonPtr()->createDefferedBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );
    m_refraction_fbo = FrameBufferManager::getSingletonPtr()->createDefferedBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );

    //---------------------------------------------------
    // Water Disorientation - Wave Effect
    //---------------------------------------------------
    m_water_dudv    = TextureManager::getSingletonPtr()->createTexture("./resource/test_water_dudv.png");
    TextureManager::getSingletonPtr()->loadTexture( m_water_dudv , true );

    m_water_normal  = TextureManager::getSingletonPtr()->createTexture("./resource/test_water_normal.png");
    TextureManager::getSingletonPtr()->loadTexture( m_water_normal , true );



    m_fresnel_entity_pos  = position_manager->getPosition( m_fresnel_entity->getLocation("position").component_id );
    m_fresnel_entity_mesh = mesh_manager->getMesh( m_fresnel_entity->getLocation("mesh").component_id );

    //---------------------------------------------------
    // Status this Technique
    //---------------------------------------------------

    m_active        = false; // default Disabled

    //---------------------------------------------------
    // Fresnel Algorithums Settings
    //---------------------------------------------------
    m_fresnel_eta    = 0.15f;
    m_fresnel_power  = 5.0f;

}

void FresnelTech::setTerrainEntity( RenderEntity * entity )
{
    m_fresnel_terrain = entity;
}

void FresnelTech::setFresnelEntity(Entity *entity)
{
    m_fresnel_entity = entity;
}

void FresnelTech::Prepare()
{
    //---------------------------------------------------
    // Create Depth Texture from the Camera (one) Perspective
    //---------------------------------------------------

    RenderToDepthTextureStart( m_depth_fbo , m_display );

        m_fresnel_entity_pos->transformation();

        m_basic_shadow->UseProgram();
        m_basic_shadow->BindMatrix("Projection" , m_projection );
        m_basic_shadow->BindMatrix("ModelView"  , m_display->getCamera()->getMatrix() * m_fresnel_entity_pos->getMatrix() );
        m_basic_shadow->BindUniform1f("Mode",0.0f); //no DrawElementsIndirect

        glCullFace( GL_BACK );

                m_fresnel_entity_mesh->Draw();

        glCullFace(GL_FRONT);

        m_basic_shadow->Unused();


    RenderToDepthTextureStop( m_depth_fbo );

    //---------------------------------------------------
    // Create Reflection & Refraction Scene
    //---------------------------------------------------
    Position * water_position = m_fresnel_entity_pos;
    float water_height = water_position->getPosition().getY();

    //---------------------------------------------------
    // Setup Reflection Camera
    //---------------------------------------------------
    Vector3f position = m_display->getCamera()->getPosition();
    position.inverse();  //World Coordinates System
    float distance    = 2 * (position.getY() - water_height);
    position.setY( position.getY() - distance );
    position.inverse();  // Camera Coordinates System
    m_display->getCamera()->setPosition( position );
    m_display->getCamera()->setPitch( -m_display->getCamera()->getPitch() );
    m_display->getCamera()->setScale(Vector3f(1,1,1));
    m_display->getCamera()->transform();

    //---------------------------------------------------
    // Create Reflection Scene
    //---------------------------------------------------
    CreateReflectionScene( Vector4f( 0, 1 ,0 , -water_height ));

    //---------------------------------------------------
    // Reset Camera - ( Setup Refraction Camera )
    //---------------------------------------------------
    position = m_display->getCamera()->getPosition();
    position.inverse(); // World Coordinates
    position.setY( position.getY() + distance );
    position.inverse(); // Camera Coordinates
    m_display->getCamera()->setPosition( position );
    m_display->getCamera()->setScale(Vector3f(1,1,1));
    m_display->getCamera()->setPitch( -m_display->getCamera()->getPitch() );
    m_display->getCamera()->transform();

    CreateRefractionScene( Vector4f( 0, -1 , 0 , water_height ) );
}

void FresnelTech::CreateReflectionScene( const Vector4f & clip_distance )
{
    RenderToBasicTextureStart(  m_reflection_fbo , m_display );

    //glDisable(GL_CULL_FACE);

    DrawEvent event;
    event.setProjection( m_projection  , DrawEvent::PRO_PROJECTION );    /* 3D Projections Matrix - come from DisplayConfig */
    event.setWorldView ( m_display->getCamera()->getMatrix()       );    /* ViewMatrix = Camera Matrix */
    event.setClipDistance( clip_distance );

    //m_system->getRenderEntityManager()->RenderAll( event );
    m_system->getRenderProcessManager()->renderClassicProcess( event );

    //m_fresnel_terrain->draw( event );

    //glEnable(GL_CULL_FACE);

    RenderToBasicTextureStop( m_reflection_fbo );
}

void FresnelTech::CreateRefractionScene(const Vector4f & clip_distance )
{
    RenderToBasicTextureStart(  m_refraction_fbo , m_display );

    DrawEvent event;
    event.setProjection( m_projection  , DrawEvent::PRO_PROJECTION );    /* 3D Projections Matrix - come from DisplayConfig */
    event.setWorldView ( m_display->getCamera()->getMatrix()       );    /* ViewMatrix = Camera Matrix */
    event.setClipDistance( clip_distance );

    //glCullFace( GL_FRONT );

    //m_system->getRenderEntityManager()->RenderAll( event );

    //glCullFace( GL_BACK );

    m_fresnel_terrain->draw( event );

    RenderToBasicTextureStop( m_refraction_fbo );
}

void FresnelTech::Update()
{
    //delete old framebuffers
    FrameBufferManager::getSingletonPtr()->destroyDepthBuffer( m_depth_fbo );
    FrameBufferManager::getSingletonPtr()->destroyDeferredBuffer( m_reflection_fbo );
    FrameBufferManager::getSingletonPtr()->destroyDeferredBuffer( m_refraction_fbo );

    //create new framebuffer
    m_depth_fbo      = FrameBufferManager::getSingletonPtr()->createDepthBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );
    m_reflection_fbo = FrameBufferManager::getSingletonPtr()->createDefferedBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );
    m_refraction_fbo = FrameBufferManager::getSingletonPtr()->createDefferedBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );
}

void FresnelTech::Render(Texture *basic)
{
    m_reflect_pos += 0.0003;

    GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
    RenderToColourTextureStart( m_fbo , m_display , buffers );

    m_fresnel_shader->UseProgram();
    m_fresnel_shader->BindMatrix( "Projection" , glm::mat4(1.0f) );
    m_fresnel_shader->BindMatrix( "ModelView"  , m_screen_position->getMatrix() );
    m_fresnel_shader->BindMatrix( "WorldProj"  , m_display->getPerspective() );

    // Bind Basic Texture Array
    m_fresnel_shader->BindTexture( basic , "tech_basic" , 0);

    //Bind Depth Texture from Camera Scene
    m_fresnel_shader->BindTexture( m_fbo->getTexture(GL_DEPTH_ATTACHMENT) , "depth_camera", 1 );

    //Bind Depth Texture with only x1 Entity
    m_fresnel_shader->BindTexture( m_depth_fbo->getTexture(GL_DEPTH_ATTACHMENT) , "fresnel_entity_depth" , 2 );

    //Bind Reflection Textures (Type:Array)
    m_fresnel_shader->BindTexture( m_reflection_fbo->getTexture(FrameBuffer::FB_TEXTURE_ARRAY0) , "reflection_textures" , 3 );

    //Bind Refraction Textures (Type::Array)
    m_fresnel_shader->BindTexture( m_refraction_fbo->getTexture(FrameBuffer::FB_TEXTURE_ARRAY0) , "refraction_textures" , 4 );

    //Bind Water DUDV Map
    m_fresnel_shader->BindTexture(  m_water_dudv   , "dudv_texture", 5 );

    //Bind Water DUDV Map
    m_fresnel_shader->BindTexture(  m_water_normal , "normal_texture", 6 );

    //float ETA
    m_fresnel_shader->BindUniform1f("Eta", m_fresnel_eta );

    //float FresnelPower
    m_fresnel_shader->BindUniform1f("FresnelPower" , m_fresnel_power );

    m_fresnel_shader->BindUniform1f("reflect_pos", m_reflect_pos );

    m_screen->DrawElements();

    m_fresnel_shader->Unused();

    RenderToColourTextureStop( m_fbo );
}
