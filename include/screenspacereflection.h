#ifndef SCREENSPACEREFLECTION_H
#define SCREENSPACEREFLECTION_H

#include "index.h"
#include "rendermanager.h"
#include "framebuffermanager.h"
#include "meshmanager.h"
#include "positionmanager.h"
#include "shadermanager.h"
#include "GLTechnique.h"

namespace Engine
{

    class ScreenSpaceReflection : public GLTechnique
    {
        public:
            explicit ScreenSpaceReflection( const std::string & tech_name );
            ~ScreenSpaceReflection()=default;

            void Create(void);
            void Prepare(void);
            void Update(void);

            void Render( Texture * basic );

            void setSSREntity( Entity  * entity );

        private:

            glm::mat4 m_projection;

            OpenPolygonDisplay * m_display;
            IShader     * m_basic_shadow;
            IShader     * m_ssr_shader;
            IShader     * m_ssr_final_shader;
            FrameBuffer * m_depth_fbo;
            FrameBuffer * m_ssr_fbo;
            Entity      * m_ssr_entity;
    };

}

#endif // SCREENSPACEREFLECTION_H
