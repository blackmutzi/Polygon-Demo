#ifndef CASCADEDSHADOWTECH_H
#define CASCADEDSHADOWTECH_H

#include "index.h"
#include "GLTechnique.h"
#include "lightmanager.h"
#include "framebuffermanager.h"
#include "scenemanager.h"
#include "frustum.h"
#include "basicshadowmodul.h"

namespace Engine
{

    struct CSM
    {
        FrameBuffer * csm_fbo;
        glm::mat4     csm_light_matrix;
        glm::mat4     csm_light_projection;
        Vector3f      csm_light_position;
    };


    class CascadedShadowTech : public GLTechnique
    {
        enum
        {
            SHADOW_MAP0 ,
            SHADOW_MAP1 ,
            SHADOW_MAP2 ,
            SHADOW_MAP3
        };

        public:
            explicit CascadedShadowTech( const std::string & tech_name );
            ~CascadedShadowTech(void)=default;

            void Create(void);
            void Update(void);
            void Prepare(void);
            void Render( Texture * basic_tech_array );

            void setLight( Light * sun_light );
            void CreateShadowMap( struct CSM csm );

            float getRadian( float angle );

        public:

            float test_float;

        private:

            glm::mat4 m_projection;
            CSM m_csm[4];

            Light              * m_light;

            OpenPolygonDisplay * m_display;
            IShader            * m_basic_shadow;
            IShader            * m_cascaded_mapping;

            FrameBuffer        * m_test_fbo;
            glm::mat4            m_ortho;
            glm::mat4            m_light_matrix;

    };
}


#endif // CASCADEDSHADOWTECH_H
