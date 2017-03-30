#ifndef CUBE_H
#define CUBE_H

#include "index.h"
#include "shadermanager.h"
#include "positionmanager.h"
#include "meshmanager.h"
#include "cameramanager.h"
#include "rendercomponent.h"

namespace Engine
{

    class Cube : public RenderEntity
    {
        public:
            Cube();
            ~Cube();

            void create( OpenPolygonDisplay * display );

            void draw(const DrawEvent &event);

            void setSingleCubeTexture( Texture * texture );

            void setCubeTextures( Textures textures );

        protected:

            Mesh     * m_mesh;
            Position * m_position;
            IShader  * m_shader;
            Texture  * m_surface;
    };

}
#endif // CUBE_H
