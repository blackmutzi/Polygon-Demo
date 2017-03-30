#ifndef DISPLACETERRAIN_H
#define DISPLACETERRAIN_H

#include "index.h"
#include "scenemanager.h"
#include "rendercomponent.h"
#include "terraingen.h"
#include "textureunit.h"

namespace Engine
{

    class DisplaceTerrain : public RenderEntity
    {
        public:
            DisplaceTerrain();

            void create(OpenPolygonDisplay *display);

            void draw(const DrawEvent &event);

        private:

            Entity   * m_entity;
            Mesh     * m_mesh;
            Mesh     * m_water_mesh;
            Position * m_position;
            Position * m_water_position;
            IShader  * m_shader;
            Texture  * m_surface;
            Texture  * m_water_surface;
    };

}
#endif // DISPLACETERRAIN_H
