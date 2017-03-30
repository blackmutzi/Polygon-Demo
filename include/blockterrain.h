#ifndef BLOCKTERRAIN_H
#define BLOCKTERRAIN_H

#include "index.h"
#include "rendermanager.h"
#include "entitymanager.h"
#include "meshmanager.h"

#include "block.h"
#include "matrixbuffer.h"
#include "cubemapping.h"

namespace Engine
{

class BlockTerrain : public RenderEntity
{
    public:

        BlockTerrain();
        ~BlockTerrain(){}

        void create( OpenPolygonDisplay * display );
        void draw( const DrawEvent & event );
    private:

        Entity       * mEntity; /*!< TODO */
        Mesh         * mMesh; /*!< TODO */
        Position     * mPosition; /*!< TODO */
        IShader      * mShader; /*!< TODO */
        MatrixBuffer * mBuffer;
        CubeMapping  * mCubeMapping;
};


}

#endif // BLOCKTERRAIN_H
