#include "heightmaparea.h"

using namespace Engine;

HeightMapArea::HeightMapArea()
    : m_height_map( nullptr ) ,
      Area()
{}
void HeightMapArea::create( OpenPolygonDisplay * display )
{
    Area::create( display );

    /* Shader: Enable Displacement */
    m_shader->UseProgram();
    m_shader->BindUniform1f("Displacement"  , 1.0f );
    m_shader->Unused();
}

void HeightMapArea::draw(const DrawEvent &event)
{
    /* Shader: Bind HeightMap */
    m_shader->UseProgram();
    m_shader->BindTexture( m_height_map , "tHeightMap", 1 ); // Set HeightMap Texture
    m_shader->Unused();

    Area::draw( event );
}

void HeightMapArea::setHeightMap(Texture * texture)
{
    if( !texture->hasState( Texture::LOADED ))
                LogManager::log( LogManager::LOG_ERROR , "Area - HeightMap texture is not loaded ");

    m_height_map = texture;
}

