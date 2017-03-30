#ifndef AREA_H
#define AREA_H

#include "index.h"
#include "rendercomponent.h"
#include "shadermanager.h"
#include "meshmanager.h"
#include "positionmanager.h"
#include "cameramanager.h"

namespace Engine
{
    class Area : public RenderEntity
    {
        public:
           Area();
          ~Area();

           /*!
            * Create Area
            * \brief create
            */
           void create( OpenPolygonDisplay * display );

           /*!
            * Draw Area
            * \brief draw
            * \param event
            */
           void draw(const DrawEvent &event);


           void setPosition( const Vector3f & position);
           void setScale( const Vector3f & scale );
           void setRotation( const Vector3f & scale );
           void setTexture( Texture *  surface );

        protected:

           Mesh     * m_mesh;
           Position * m_position;
           IShader  * m_shader;
           Texture  * m_surface;
    };

}

#endif // AREA_H
