#ifndef CUBEGENERATOR_H
#define CUBEGENERATOR_H

#include "index.h"
#include "rendercomponent.h"
#include "shadermanager.h"
#include "meshmanager.h"
#include "positionmanager.h"
#include "cameramanager.h"
#include "matrixbuffer.h"
#include "cubemapping.h"

namespace Engine
{

class CubeGenerator : public RenderEntity
{
    public:
        explicit CubeGenerator( int draw_cubes );
        ~CubeGenerator();

        void create( OpenPolygonDisplay * display );
        void draw(const DrawEvent &event);

        void setCubeTextures( Textures textures );
        void setCubePositions( Vector3fv positions );

    protected:

        int m_draw_cubes;

        Mesh         * m_mesh;
        Position     * m_position;
        IShader      * m_shader;
        MatrixBuffer * m_buffer;
        CubeMapping  * m_cube_mapping;

        Vector3fv m_cube_positions;

};

}

#endif // CUBEGENERATOR_H
