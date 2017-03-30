 /*
    Copyright (C) 2014  Mutzii

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
#ifndef TERRAIN_H
#define TERRAIN_H

#include "index.h"
#include "rendermanager.h"
#include "entitymanager.h"
#include "meshmanager.h"
#include "animationmanager.h"
#include "texturemanager.h"
#include "lightmanager.h"
#include "positionmanager.h"
#include "cameramanager.h"

#include "framebuffer.h"
#include "textureunit.h"
#include "sphere.h"

namespace Engine
{

    /*!
     * Terrain - Renderobject
     * \brief The Terrain class
     */
    class Terrain : public RenderEntity
    {
        public:
            Terrain();
            ~Terrain(){}

            void create( OpenPolygonDisplay * display );
            void draw( const DrawEvent & event );

            void setPosition( const Vector3f & position );

     private:

        Entity    * mEntity; /*!< TODO */
        Mesh      * mMesh; /*!< TODO */
        Position  * mPosition; /*!< TODO */
        IShader   * mShader; /*!< TODO */

        Texture   * mSurface; /*!< TODO */
        Texture   * mNormalSurface; /*!< TODO */
        Texture   * mHeightMap;

    };

}


#endif // TERRAIN_H
