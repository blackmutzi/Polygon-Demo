#ifndef ANIMATIONENTITY_H
#define ANIMATIONENTITY_H

#include "index.h"
#include "display.h"
#include "rendercomponent.h"
#include "animation.h"
#include "mesh.h"
#include "IShader.h"
#include "drawevent.h"
#include "cameramanager.h"
#include "animationmanager.h"

namespace Engine
{
    class AnimationEntity : public RenderEntity
    {
         public:
              explicit AnimationEntity( IShader * shader , Mesh * mesh );
             ~AnimationEntity();

              /*!
               * Create Animation Entity
               * \brief create
               */
              void create( OpenPolygonDisplay * display );

              /*!
               * Draw Animation with Entity
               * \brief draw
               * \param event
               */
              void draw(const DrawEvent &event);

              /*!
               * set active Animation
               * \brief setAnimation
               * \param animation
               */
              void setAnimation( Animation * animation );

              /*!
               * Prepare animation data for next frame
               * Notice: call into Logic Process
               * \brief AnimationLoop
               */
              void AnimationLoop(void);

              /*!
               * Create Animations
               * \brief createAnimation
               * \param animation_name
               * \param animation_file
               * \return
               */
              Animation * createAnimation( const std::string & animation_name , const std::string & animation_file );

         protected:

              IShader   * m_shader;
              Mesh      * m_mesh;
              Position  * m_position;
              Animation * m_animation;

              int  m_animation_data_size;

    };
}

#endif // ANIMATIONENTITY_H
