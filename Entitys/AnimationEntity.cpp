#include "animationentity.h"

using namespace Engine;

AnimationEntity::AnimationEntity( IShader * shader , Mesh * mesh )
    : m_shader( shader ) ,
      m_mesh  ( mesh   ) ,
      m_position( nullptr ) ,
      m_animation( nullptr ) ,
      m_animation_data_size( 0 ) ,
      RenderEntity("AnimationEntity")
{}

AnimationEntity::~AnimationEntity(){}

void AnimationEntity::create( OpenPolygonDisplay * display )
{
    //---------------------------------
    // Component Databases ..
    //---------------------------------
    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();

    ShaderManager   * shader_manager   = (ShaderManager   *) database_manager->getDatabase("ShaderManager");
    PositionManager * position_manager = (PositionManager *) database_manager->getDatabase("PositionManager");

    m_position = position_manager->createPosition();
    m_animation_data_size = m_mesh->getOriginalVertices().size() * 3;

    getEntity()->attach( m_mesh );
    getEntity()->attach( m_position );
    getEntity()->attach( m_shader );
    getEntity()->attach( CameraManager::getSingletonPtr()->getCamera( MAIN_CAMERA ) );
}

void AnimationEntity::draw(const DrawEvent &event)
{
    m_position->transformation();

    if( !event.hasShadowEvent() )
    {
        m_shader->UseProgram();

        m_shader->BindMatrix("Projection" ,  event.getProjection()  );
        m_shader->BindMatrix("ModelView"  ,  event.getWorldView() * m_position->getMatrix() );
        m_shader->BindMatrix("ModelMatrix",  m_position->getMatrix() );
        m_shader->BindMatrix("WorldView"  ,  event.getWorldView()        );
        m_shader->BindMatrix("TexMatrix"  ,  glm::mat4(1.0f)        );
        m_shader->BindMatrix("NormalMatrix", m_position->getNormalMatrix( event.getWorldView() * m_position->getMatrix() ) );

        m_mesh->getVertexArrayObject()->getAttachBuffer(ATTR_BUFFER_VERTICES)->update( 0 , m_animation_data_size * sizeof(float) , m_animation->getData());

        //m_mesh->getDoubleBuffer()->update( 0 , m_animation_data_size * sizeof(float) , m_animation->getData() );
        //m_mesh->getDoubleBuffer()->SwapBuffers();

        m_mesh->DrawElements();

        //m_mesh->DrawShadowElements();

         m_shader->Unused();

    }else /* FlashLight Shadow Event */
    {
        glm::mat4 modelview = event.getShadowEvent().getViewMatrix() * m_position->getMatrix();

        event.getShadowEvent().getShadowShader()->BindMatrix("ModelView", modelview );

        m_mesh->DrawElements();

        //m_mesh->DrawShadowElements();
    }
}

Animation * AnimationEntity::createAnimation( const std::string & animation_name , const std::string & animation_file )
{
    //mAnimation = AnimationManager::getSingletonPtr()->createAnimation("laufen","./resource/human3.bvh", mMesh ); //human3.bvh

    //---------------------------------
    // Component Databases ..
    //---------------------------------
    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();
    AnimationManager * animation_manager = (AnimationManager *) database_manager->getDatabase("AnimationManager");

    return animation_manager->createAnimation( animation_name , animation_file , m_mesh );
}

void AnimationEntity::setAnimation(Animation *animation)
{
    m_animation = animation;
}

void AnimationEntity::AnimationLoop(void)
{
    m_animation->Loop();

    //---------------------------------
    // Component Databases ..
    //---------------------------------
    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();
    AnimationManager * animation_manager = (AnimationManager *) database_manager->getDatabase("AnimationManager");

    animation_manager->prepareVBO( m_animation , m_mesh );
}
