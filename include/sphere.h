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
#ifndef SPHERE_H
#define SPHERE_H

#include "index.h"
#include "rendermanager.h"
#include "entitymanager.h"
#include "meshmanager.h"
#include "animationmanager.h"
#include "positionmanager.h"
#include "cameramanager.h"

namespace Engine
{
    /*!
     * Sphere Renderobject
     * \brief The Sphere class
     */
    class Sphere : public RenderEntity
    {
        public:
            Sphere();
            ~Sphere(){}


            void create( OpenPolygonDisplay * display );
            void draw( const DrawEvent & event );

            void setColour( const Vector4f & colour );
            void setScale( const Vector3f & scale );
            void setPosition( const Vector3f & position );

        private:

            Entity    * mEntity; /*!< TODO */
            Mesh      * mMesh; /*!< TODO */
            Position  * mPosition; /*!< TODO */
            IShader   * mShader; /*!< TODO */
            Animation * mAnimation; /*!< TODO */

            Vector4f mColour; /*!< TODO */
    };
}

#endif // SPHERE_H
