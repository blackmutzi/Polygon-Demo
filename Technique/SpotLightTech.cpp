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
#include "spotlighttech.h"

using namespace Engine;

SpotLightTech::SpotLightTech( const std::string & tech_name )
    : GLTechnique( tech_name )
{}

void SpotLightTech::Create( void )
{
     m_display    = m_system->getDisplay();
     m_projection = m_display->getPerspective();

     Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();
     ShaderManager   * shader_manager   = (ShaderManager   *) database_manager->getDatabase("ShaderManager");

     //---------------------------------------------------
     // Light Shader
     //---------------------------------------------------
     m_light_shader = shader_manager->createShader("SpotLightShader");
     shader_manager->addSource( m_light_shader , "spot_lighting" , FRAGMENT | VERTEX );
     m_light_shader->LinkProgram();

     //---------------------------------------------------
     // Status this Technique
     //---------------------------------------------------
     m_active        = false; // default Disabled
}

void SpotLightTech::Update(void){}
void SpotLightTech::Prepare(void)
{
    //---------------------------------------------------
    // Build Light TBOs ( Texture Buffer Objects )
    //---------------------------------------------------

    // TODO: Important for SpotLightTech
    //      spotlight0->ShaderUpdate( 0 , m_shadow_mapping );
    //      SpotLight * spotlight1 = LightManager::getSingletonPtr()->getSpotLight("spotlight1");
    //      spotlight1->camtransformation();
    //      spotlight1->ShaderUpdate( 1 , shader );
}

void SpotLightTech::Render(Texture * basic_tech_array )
{
    //---------------------------------------------------
    // Create Final Colour + Light Texture
    //---------------------------------------------------
    //---------------------------------------------------
    // Draw Screen with Colour + Light Source(s)
    //---------------------------------------------------

    GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
    RenderToColourTextureStart( m_fbo , m_display , buffers );

    //use light_shader
    m_light_shader->UseProgram();

    m_light_shader->BindMatrix( "Projection" , glm::mat4(1.0f) );
    m_light_shader->BindMatrix( "ModelView"  , m_screen_position->getMatrix() );

    //bind basic_tech_array texture
    m_light_shader->BindTexture( basic_tech_array , "tech_basic" , 0 );

    //glClear( GL_COLOR_BUFFER_BIT  );
    //glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

    //bind Light TBOs ( new Texture Buffer Objects )
    //ShaderUpdate is old
    SpotLight * light = LightManager::getSingletonPtr()->getSpotLight("spotlight0");
    light->camtransformation();
    light->ShaderUpdate( 0 , m_light_shader );

//    SpotLight * light1 = LightManager::getSingletonPtr()->getSpotLight("spotlight1");
//    light1->camtransformation();
//    light1->ShaderUpdate( 1 , m_light_shader );

    //draw screen
    m_screen->DrawElements();

    //unused light_shader
    m_light_shader->Unused();

    RenderToColourTextureStop( m_fbo );
}
