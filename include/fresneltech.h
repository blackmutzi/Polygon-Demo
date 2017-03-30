#ifndef FRESNELTECH_H
#define FRESNELTECH_H

#include "index.h"
#include "rendermanager.h"
#include "framebuffermanager.h"
#include "positionmanager.h"
#include "shadermanager.h"
#include "meshmanager.h"
#include "GLTechnique.h"

namespace Engine
{

    class FresnelTech : public GLTechnique
    {
        public:
            explicit FresnelTech( const std::string & tech_name );
            ~FresnelTech()=default;

            void Create(void);
            void Prepare(void);
            void Update(void);
            void Render( Texture * basic );


            void CreateReflectionScene( const Vector4f & clip_distance );
            void CreateRefractionScene( const Vector4f & clip_distance );

            void setFresnelEntity( Entity * entity );

            void setTerrainEntity( RenderEntity * entity );

        public:

            float m_reflect_pos;

        private:

          glm::mat4 m_projection;

          Entity * m_fresnel_entity;
          Position * m_fresnel_entity_pos;
          Mesh     * m_fresnel_entity_mesh;

          RenderEntity * m_fresnel_terrain;

          OpenPolygonDisplay * m_display;
          IShader     * m_basic_shadow;
          IShader     * m_fresnel_shader;

          FrameBuffer * m_depth_fbo;
          FrameBuffer * m_reflection_fbo;
          FrameBuffer * m_refraction_fbo;

          Texture     * m_water_dudv;
          Texture     * m_water_normal;

          float m_fresnel_power;
          float m_fresnel_eta;

    };
}


#endif // FRESNELTECH_H
