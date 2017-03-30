 /*
    Copyright (C) 2014  Mutzii

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "index.h"
#include "input.h"
#include "rendermanager.h"
#include "factory.h" /* inc Entitys | Elements */
#include "openpolygon.h"
#include "systemmanager.h"
#include "sphere.h"
#include "terrain.h"
#include "block.h"
#include "human.h"
#include "blockterrain.h"
#include "multirendering.h"
#include "deformationmanager.h"

#include "Technique/spotshadowtech.h"
#include "shadowmappingtech.h"
#include "spotlighttech.h"
#include "screenspacereflection.h"
#include "cascadedshadowtech.h"
#include "fresneltech.h"

#include "displaceterrain.h"
#include "waterterrain.h"

#include "frustumentity.h"

#define  X_POSITIVE 1
#define  X_NEGATIVE 2
#define  Z_POSITIVE 3
#define  Z_NEGATIVE 4

using namespace Engine;

/*!
 * TestScene
 * \brief The TestScene class
 */
class TestScene : public FrameListener , public InputListener
{
    public:
        TestScene();
        ~TestScene(){}

    void initialize( OpenPolygonDisplay * display );
    void RenderLogic(float time );

    /*!
     * Catch Keyboard Event
     * \brief onKeyEvent
     * \param event
     */
    void onKeyEvent(const KeyEvent &event);

    /*!
     * Catch Mouse Event
     * \brief onMouseEvent
     * \param event
     */
    void onMouseEvent(const MouseEvent &event);

    void CameraMovement( const std::string & camara_name , const KeyEvent & event );
    void LightMovement ( const std::string & light_name  , const KeyEvent & event );
    void LightRotation ( const std::string & light_name  , const KeyEvent & event );

    /* Initialize functions */

    void createOverlay(void);
    void ActiveRenderSystem( OpenPolygonDisplay * display );
    void CameraSetup(void);

    /* Render Logic Functions */

    void ShowFPS( float time );

    private:

        void createBlocks( RenderEntityManager * manager );

        float mRotations; /*!< TODO */
        float mBestLowTime;
        float mBestHighTime;
        float mAnimTime;

        int mReset;

        Vector2i  m_rgb_pos;

        Human       * human; /*!< TODO */
        TextElement * mText; /*!< TODO */
        TextElement * mLightData;
        TextElement * mCamData;

        TextElement * mTimeData0;
        TextElement * mTimeData1;

        Light * mLight0; /*!< TODO */
        Light * mLight1; /*!< TODO */

        Block    * m_block_ai;
        Position * m_block_ai_pos;
        float      m_block_movement;
        int        m_block_direction;
        int        m_block_last_direction;
        int        m_block_next_direction;

        float      m_rot_count;
        float      m_yaw_count;
        float      m_water_position;

        int r,g,b;
        Texture * rgb;

        Position * spotlightposition;
        Camera * m_camera;
        Camera * m_custom_camera;
        OpenPolygonDisplay * m_display;
        Sphere  * m_light_test;

        FresnelTech * m_fresnel_tech;
        CascadedShadowTech * m_csm_tech;
        WaterTerrain * m_water_terrain;

        bool mCameraMode;
        bool mRotationMode;
        int  m_test;

};

#endif // TESTSCENE_H
