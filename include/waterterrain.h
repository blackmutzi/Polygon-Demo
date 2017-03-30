#ifndef WATERTERRAIN_H
#define WATERTERRAIN_H

#include "index.h"
#include "scenemanager.h"
#include "rendercomponent.h"
#include "terraingen.h"
#include "textureunit.h"

namespace Engine
{
    class WaterTerrain : public RenderEntity
    {
        public:
            WaterTerrain();
            ~WaterTerrain()=default;

            void create(OpenPolygonDisplay *display);

            void draw(const DrawEvent &event);

            void setPosition( const Vector3f & position );

    private:

        Entity   * m_entity;
        Mesh     * m_mesh;
        Position * m_position;
        IShader  * m_shader;
        Texture  * m_water_surface;

    };
}

#endif // WATERTERRAIN_H
