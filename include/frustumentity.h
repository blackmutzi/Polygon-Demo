#ifndef FRUSTUMENTITY_H
#define FRUSTUMENTITY_H

#include "index.h"
#include "rendermanager.h"
#include "entitymanager.h"
#include "meshmanager.h"
#include "animationmanager.h"
#include "scenemanager.h"
#include "frustum.h"

namespace Engine
{
    class FrustumEntity : public RenderEntity
    {
        public:
            FrustumEntity();
            ~FrustumEntity(){}

            void create(OpenPolygonDisplay *display);
            void draw(const DrawEvent &event);

        private:

            Entity   * m_entity;
            Mesh     * m_mesh;
            Position * m_position;
            IShader  * m_shader;
            OpenPolygonDisplay * m_display;

    };

}
#endif // FRUSTUMENTITY_H
