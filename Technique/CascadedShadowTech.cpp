/*
   Copyright (C) 2014 - 2016 Mutzii

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "cascadedshadowtech.h"

using namespace Engine;

CascadedShadowTech::CascadedShadowTech( const std::string & tech_name )
    : GLTechnique( tech_name )
{
    test_float = 0.0f;
}

void CascadedShadowTech::Create( void )
{
     m_display    = m_system->getDisplay();
     m_projection = m_display->getPerspective();

     Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();
     ShaderManager   * shader_manager   = (ShaderManager   *) database_manager->getDatabase("ShaderManager");

     //---------------------------------------------------
     // Basic Shadow Shader - for Entity Rendering
     //---------------------------------------------------
     m_basic_shadow  = shader_manager->createShader("BasicShadowRendering");
     shader_manager->addSource( m_basic_shadow  ,  "shadow" , FRAGMENT | VERTEX );
     m_basic_shadow->LinkProgram();

     //---------------------------------------------------
     // Cascaded Mapping Shader
     //---------------------------------------------------
     m_cascaded_mapping = shader_manager->createShader("CascadedMapping");
     shader_manager->addSource( m_cascaded_mapping , "cascaded_mapping" , FRAGMENT | VERTEX );
     m_cascaded_mapping->LinkProgram();

     //---------------------------------------------------
     // Create Shadow Map - Framebuffers
     //---------------------------------------------------

     m_csm[ SHADOW_MAP0 ].csm_fbo = FrameBufferManager::getSingletonPtr()->createDepthBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );
     m_csm[ SHADOW_MAP1 ].csm_fbo = FrameBufferManager::getSingletonPtr()->createDepthBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );
     m_csm[ SHADOW_MAP2 ].csm_fbo = FrameBufferManager::getSingletonPtr()->createDepthBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );
     m_csm[ SHADOW_MAP3 ].csm_fbo = FrameBufferManager::getSingletonPtr()->createDepthBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );

     m_test_fbo = FrameBufferManager::getSingletonPtr()->createDepthBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );

     //---------------------------------------------------
     // Status this Technique
     //---------------------------------------------------
     m_active = false; // default Disabled

     //---------------------------------------------------
     // Setup Moduls
     //------------------------
     m_render_modul_manager->addModul( new BasicShadowModul() , "ShadowModul");
}

void CascadedShadowTech::setLight( Light * sun_light )
{
    m_light = sun_light;
}

void CascadedShadowTech::Update(void)
{
     FrameBufferManager::getSingletonPtr()->destroyDepthBuffer( m_csm[ SHADOW_MAP0 ].csm_fbo );
     FrameBufferManager::getSingletonPtr()->destroyDepthBuffer( m_csm[ SHADOW_MAP1 ].csm_fbo );
     FrameBufferManager::getSingletonPtr()->destroyDepthBuffer( m_csm[ SHADOW_MAP2 ].csm_fbo );
     FrameBufferManager::getSingletonPtr()->destroyDepthBuffer( m_csm[ SHADOW_MAP3 ].csm_fbo );

     m_csm[ SHADOW_MAP0 ].csm_fbo = FrameBufferManager::getSingletonPtr()->createDepthBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );
     m_csm[ SHADOW_MAP1 ].csm_fbo = FrameBufferManager::getSingletonPtr()->createDepthBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );
     m_csm[ SHADOW_MAP2 ].csm_fbo = FrameBufferManager::getSingletonPtr()->createDepthBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );
     m_csm[ SHADOW_MAP3 ].csm_fbo = FrameBufferManager::getSingletonPtr()->createDepthBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );
}

void CascadedShadowTech::CreateShadowMap( struct CSM csm )
{
    RenderToDepthTextureStart( csm.csm_fbo , m_display );

    DrawEvent draw_event;
    draw_event.setWorldView ( csm.csm_light_matrix     );
    draw_event.setProjection( csm.csm_light_projection , DrawEvent::PRO_ORTHO );

    m_system->getRenderProcessManager()->setRenderModulManager( m_render_modul_manager );
    m_system->getRenderProcessManager()->renderProcess( draw_event );

    RenderToDepthTextureStop( csm.csm_fbo );
}

float CascadedShadowTech::getRadian(float angle)
{
    return ( angle * PI / 180 );
}

void CascadedShadowTech::Prepare(void)
{
    //---------------------------------------------------
    // Make 4x Shadow Maps with new Frustums             ( Shadow Map Position )
    //---------------------------------------------------
    Vector3f light_offset0       = Vector3f(0,0,0);
    Vector3f camera_offset       = Vector3f(0,0,0);
    Vector3f offset              = Vector3f(0,0,0);


    //---------------------------------------------------
    // get Original Camera Position ( Inverse World Coordinate System )
    //---------------------------------------------------
    Vector3f camera_position     = m_display->getCamera()->getPosition();

    //---------------------------------------------------
    // convert to World (aka. Object ) Coordinate System
    //---------------------------------------------------
    camera_position.inverse();

    //---------------------------------------------------
    // Save X,Z Coordinates
    //---------------------------------------------------
    camera_offset.setX( camera_position.getX() );
    camera_offset.setZ( camera_position.getZ() );

    //---------------------------------------------------
    // World Camera Rotation * World Clip Offset = new Position ( World Coordinate System )
    //---------------------------------------------------
    glm::mat4 rotation_matrix = m_light->getRotationMatrix();
    glm::vec4 winkel          = glm::vec4(0,0, 150.0f + 320 ,1.0f) * rotation_matrix;
    offset                    = Vector3f( winkel.x , winkel.y , winkel.z );

    //---------------------------------------------------
    // Add Camera World X,Z Coordinate and World Clip Offset
    //---------------------------------------------------
    light_offset0 = light_offset0 + camera_offset;
    light_offset0 = light_offset0 + offset;

    m_csm [ SHADOW_MAP0 ].csm_light_position = light_offset0;
    m_csm [ SHADOW_MAP1 ].csm_light_position = light_offset0;
    m_csm [ SHADOW_MAP2 ].csm_light_position = light_offset0;
    m_csm [ SHADOW_MAP3 ].csm_light_position = light_offset0;

    //---------------------------------------------------
    // convert Light Position to ( Inverse World Coordinate System ) (aka. Camera Coordinates )
    // Light Position: inverse World Position
    // Light Rotation: 90° Positive ( for top to=> bottom )
    //---------------------------------------------------
    light_offset0.inverse();

    m_light->setPosition( light_offset0 );
    m_light->camtransformation();

    //---------------------------------------------------
    // Setup LightView Matrix for Shadow Maps
    //---------------------------------------------------

    m_csm[ SHADOW_MAP0 ].csm_light_matrix = m_light->getMatrix();
    m_csm[ SHADOW_MAP1 ].csm_light_matrix = m_light->getMatrix();
    m_csm[ SHADOW_MAP2 ].csm_light_matrix = m_light->getMatrix();
    m_csm[ SHADOW_MAP3 ].csm_light_matrix = m_light->getMatrix();

    //---------------------------------------------------
    // Setup LightProjection (Ortho) Matrix for Shadow Maps ( >= Shadow Map Size )
    //---------------------------------------------------

    m_csm[SHADOW_MAP0].csm_light_projection = FrustumUtils::getOrthoFrustum( FrustumUtils::createFrustumVertices( -1.0 , 300.0   ) , 50.0f , 1000.0f );
    m_csm[SHADOW_MAP1].csm_light_projection = FrustumUtils::getOrthoFrustum( FrustumUtils::createFrustumVertices( -1.0 , 600.0   ) , 20.0f , 1000.0f );
    m_csm[SHADOW_MAP2].csm_light_projection = FrustumUtils::getOrthoFrustum( FrustumUtils::createFrustumVertices( -1.0 , 1000.0  ) , -1.0f  , 1000.0f );
    m_csm[SHADOW_MAP3].csm_light_projection = FrustumUtils::getOrthoFrustum( FrustumUtils::createFrustumVertices( -1.0 , 1500.0  ) , -1.0f  , 1000.0f );

    //---------------------------------------------------
    // Create Shadow Maps
    //---------------------------------------------------

    CreateShadowMap( m_csm[ SHADOW_MAP0 ] );
    CreateShadowMap( m_csm[ SHADOW_MAP1 ] );
    CreateShadowMap( m_csm[ SHADOW_MAP2 ] );
    CreateShadowMap( m_csm[ SHADOW_MAP3 ] );

    //---------------------------------------------------
    // Reset Light Offset
    //---------------------------------------------------
    //m_light->setPosition( light_offset );
}

void CascadedShadowTech::Render(Texture *basic_tech_array)
{
    //---------------------------------------------------
    // Create Final Cascaded Shadow Texture
    //--------------------------------------------------

    GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
    RenderToColourTextureStart( m_fbo , m_display , buffers );

    m_cascaded_mapping->UseProgram();

    m_cascaded_mapping->BindMatrix( "Projection" , glm::mat4(1.0f) );
    m_cascaded_mapping->BindMatrix( "ModelView"  , m_screen_position->getMatrix() );

    //Bind Basic Colour (Array)
    m_cascaded_mapping->BindTexture( basic_tech_array , "tech_basic" , 0 );

    //Bind 4x Shadow Maps
    m_cascaded_mapping->BindTexture( m_csm[SHADOW_MAP0].csm_fbo->getTexture( GL_DEPTH_ATTACHMENT ) , "shadow_map0", 1);
    m_cascaded_mapping->BindTexture( m_csm[SHADOW_MAP1].csm_fbo->getTexture( GL_DEPTH_ATTACHMENT ) , "shadow_map1", 2);
    m_cascaded_mapping->BindTexture( m_csm[SHADOW_MAP2].csm_fbo->getTexture( GL_DEPTH_ATTACHMENT ) , "shadow_map2", 3);
    m_cascaded_mapping->BindTexture( m_csm[SHADOW_MAP3].csm_fbo->getTexture( GL_DEPTH_ATTACHMENT ) , "shadow_map3", 4);

    //Bind Camera Depth Scene
    m_cascaded_mapping->BindTexture( m_fbo->getTexture(GL_DEPTH_ATTACHMENT) , "depth_camera" , 5 );

    //Bind 4x DepthMVPs ( Light Projection * LightView )
    m_cascaded_mapping->BindMatrix("DepthMVP0", m_csm[SHADOW_MAP0].csm_light_projection * m_csm[SHADOW_MAP0].csm_light_matrix );
    m_cascaded_mapping->BindMatrix("DepthMVP1", m_csm[SHADOW_MAP1].csm_light_projection * m_csm[SHADOW_MAP1].csm_light_matrix );
    m_cascaded_mapping->BindMatrix("DepthMVP2", m_csm[SHADOW_MAP2].csm_light_projection * m_csm[SHADOW_MAP2].csm_light_matrix );
    m_cascaded_mapping->BindMatrix("DepthMVP3", m_csm[SHADOW_MAP3].csm_light_projection * m_csm[SHADOW_MAP3].csm_light_matrix );

    double distance0 = 0.1f;
    double distance1 = 0.4f;
    double distance2 = 0.8f;
    double distance3 = 1.0f;

    m_cascaded_mapping->BindVec4f("dists",Vector4f( distance0 , distance1 , distance2 , distance3) );

    m_cascaded_mapping->BindVec3f("lightposition0", m_csm[SHADOW_MAP0].csm_light_position);
    m_cascaded_mapping->BindVec3f("lightposition1", m_csm[SHADOW_MAP1].csm_light_position);
    m_cascaded_mapping->BindVec3f("lightposition2", m_csm[SHADOW_MAP2].csm_light_position);
    m_cascaded_mapping->BindVec3f("lightposition3", m_csm[SHADOW_MAP3].csm_light_position);

    //Draw Screen
    m_screen->DrawElements();

    m_cascaded_mapping->Unused();

    RenderToColourTextureStop( m_fbo );
}


