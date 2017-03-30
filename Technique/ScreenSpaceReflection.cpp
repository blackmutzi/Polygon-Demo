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
#include "screenspacereflection.h"

using namespace Engine;

ScreenSpaceReflection::ScreenSpaceReflection( const std::string & tech_name )
    : GLTechnique( tech_name )
{}

void ScreenSpaceReflection::Create(void)
{
    m_display        = m_system->getDisplay();         // Get Display by RenderSystem
    m_projection     = m_display->getPerspective();

    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();
    ShaderManager   * shader_manager   = (ShaderManager   *) database_manager->getDatabase("ShaderManager");


    //---------------------------------------------------
    // Basic Shadow Shader - for Entity (Depth Texture) Rendering
    //---------------------------------------------------

    m_basic_shadow  = shader_manager->createShader("BasicShadowRendering_TEST");
    shader_manager->addSource( m_basic_shadow  , "shadow" , FRAGMENT | VERTEX );
    m_basic_shadow->LinkProgram();

    //---------------------------------------------------
    // SSR Shader
    //---------------------------------------------------

    m_ssr_shader  = shader_manager->createShader("SSR");
    shader_manager->addSource( m_ssr_shader , "ssr" , VERTEX | FRAGMENT );
    m_ssr_shader->LinkProgram();

    m_ssr_final_shader  = shader_manager->createShader("SSR_FINAL");
    shader_manager->addSource( m_ssr_final_shader , "ssr_final" , VERTEX | FRAGMENT );
    m_ssr_final_shader->LinkProgram();


    //---------------------------------------------------
    // Create Depth Texture Framebuffer
    //---------------------------------------------------
    m_depth_fbo = FrameBufferManager::getSingletonPtr()->createDepthBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );
    m_ssr_fbo   = FrameBufferManager::getSingletonPtr()->createDefaultBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );

    //---------------------------------------------------
    // Status this Technique
    //---------------------------------------------------

    m_active        = false; // default Disabled
}

void ScreenSpaceReflection::setSSREntity( Entity * entity )
{
    m_ssr_entity = entity;
}


void ScreenSpaceReflection::Prepare(void)
{
    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();
    MeshManager     * mesh_manager     = (MeshManager     *) database_manager->getDatabase("MeshManager");
    PositionManager * position_manager = (PositionManager *) database_manager->getDatabase("PositionManager");


    //---------------------------------------------------
    // Create Depth Texture from the Camera (one) Perspective
    //---------------------------------------------------
    Position * ssr_entity_pos  = position_manager->getPosition( m_ssr_entity->getLocation("position").component_id );
    Mesh     * ssr_entity_mesh = mesh_manager->getMesh( m_ssr_entity->getLocation("mesh").component_id );

    RenderToDepthTextureStart( m_depth_fbo , m_display );

        ssr_entity_pos->transformation();

        m_basic_shadow->UseProgram();
        m_basic_shadow->BindMatrix("Projection" , m_projection );
        m_basic_shadow->BindMatrix("ModelView"  , m_display->getCamera()->getMatrix() * ssr_entity_pos->getMatrix() );
        m_basic_shadow->BindUniform1f("Mode",0.0f); //no DrawElementsIndirect

        glCullFace( GL_BACK );

                ssr_entity_mesh->Draw();

        glCullFace(GL_FRONT);

        m_basic_shadow->Unused();


    RenderToDepthTextureStop( m_depth_fbo );
}

void ScreenSpaceReflection::Update(void)
{
    //delete old framebuffers
    FrameBufferManager::getSingletonPtr()->destroyDepthBuffer( m_depth_fbo );

    //create new depth framebuffer
    m_depth_fbo = FrameBufferManager::getSingletonPtr()->createDepthBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );

    //delete old framebuffers
    FrameBufferManager::getSingletonPtr()->destroyDefaultBuffer( m_ssr_fbo );

    //create new depth framebuffer
    m_ssr_fbo = FrameBufferManager::getSingletonPtr()->createDefaultBuffer( m_display->getRenderWidth() , m_display->getRenderHeight() );
}

void ScreenSpaceReflection::Render(Texture *basic)
{
    //---------------------------------------------------
    // Create Final SSR Texture
    //---------------------------------------------------
    //---------------------------------------------------
    // Draw Screen with SSR Shader
    //---------------------------------------------------

    GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
    RenderToColourTextureStart( m_ssr_fbo , m_display , buffers );

    m_ssr_shader->UseProgram();
    m_ssr_shader->BindMatrix( "Projection"            , glm::mat4(1.0f) );
    m_ssr_shader->BindMatrix( "ModelView"             , m_screen_position->getMatrix() );
    m_ssr_shader->BindMatrix( "InverseViewProjection" , glm::inverse( m_projection ) );
    m_ssr_shader->BindMatrix( "ViewProjection"        , m_projection );

    // Bind Basic Texture Array
    m_ssr_shader->BindTexture( basic , "tech_basic" , 0);

    //Bind Depth Texture from Camera Scene
    m_ssr_shader->BindTexture( m_fbo->getTexture(GL_DEPTH_ATTACHMENT) , "depth_camera", 1 );

    m_screen->DrawElements();

    m_ssr_shader->Unused();

    RenderToColourTextureStop( m_ssr_fbo );


    RenderToColourTextureStart( m_fbo , m_display , buffers );

    m_ssr_final_shader->UseProgram();
    m_ssr_final_shader->BindMatrix( "Projection" , glm::mat4(1.0f) );
    m_ssr_final_shader->BindMatrix( "ModelView"  , m_screen_position->getMatrix() );

    // Bind Basic Texture Array
    m_ssr_final_shader->BindTexture( basic , "tech_basic" , 0);

    //Bind Depth Texture from Camera Scene
    m_ssr_final_shader->BindTexture( m_fbo->getTexture(GL_DEPTH_ATTACHMENT) , "depth_camera", 1 );

    //Bind Depth Texture with only x1 Entity
    m_ssr_final_shader->BindTexture( m_depth_fbo->getTexture(GL_DEPTH_ATTACHMENT) , "ssr_entity_depth" , 2 );

    //Bind Reflection Texture
    m_ssr_final_shader->BindTexture( m_ssr_fbo->getTexture(GL_COLOR_ATTACHMENT0) , "ssr_reflection", 3);

    m_screen->DrawElements();

    m_ssr_final_shader->Unused();

    RenderToColourTextureStop( m_fbo );
}


