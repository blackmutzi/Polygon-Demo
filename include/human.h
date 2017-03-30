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
#ifndef HUMAN_H
#define HUMAN_H

#include "index.h"
#include "rendermanager.h"
#include "entitymanager.h"
#include "meshmanager.h"
#include "animationmanager.h"
#include "positionmanager.h"
#include "cameramanager.h"

#include "sphere.h"

namespace Engine
{

    /*!
     * Human Renderobject
     * \brief The Human class
     */
    class Human : public RenderEntity
    {
        public:
           Human();
           ~Human(){}

           void create( OpenPolygonDisplay * display );
           void draw( const DrawEvent & event );
           void shadowDraw( const ShadowEvent & event );

           Entity * getObject(void);


        private:

           Entity    * mEntity; /*!< TODO */
           Mesh      * mMesh; /*!< TODO */
           Position  * mPosition; /*!< TODO */
           IShader   * mShader; /*!< TODO */
           Animation * mAnimation; /*!< TODO */

           IShader   * mColorShader;

           Sphere    * mSphere; /*!< TODO */

           uint mFeedbackBuffer;

           int mSize;
    };

}

#endif // HUMAN_H
