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
#ifndef BLOCK_H
#define BLOCK_H

#include "index.h"
#include "rendermanager.h"
#include "entitymanager.h"
#include "meshmanager.h"
#include "animationmanager.h"
#include "positionmanager.h"
#include "shadermanager.h"
#include "cameramanager.h"

#include "texturemanager.h"
#include "framebuffer.h"



#include "contextmanager.h"
#include "contextcontainertest.h"

namespace Engine
{
    /*!
     * Block Renderobject
     * \brief The Block class
     */
    class Block : public RenderEntity
    {

        public:
            Block();
            ~Block(){}

            void create( OpenPolygonDisplay * display );
            void draw( const DrawEvent & event );
            void setTexture( Texture * texture );

            Mesh * createMeshTest( const std::string & mesh_resource );

        private:

            GLuint mindex;

            Entity    * mEntity; /*!< TODO */
            Mesh      * mMesh; /*!< TODO */
            Position  * mPosition; /*!< TODO */
            IShader    * mShader; /*!< TODO */
            Texture   * mSurface; /*!< TODO */
            FrameBuffer * mFramebuffer; /*!< TODO */

            bool liststatus;

    };

}
#endif // BLOCK_H
