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
#include "testscene.h"

TestScene::TestScene()
{
    InputManager::getSingletonPtr()->addInputListener( this );

    mRotations        = 0.0f;
    mBestLowTime      = 1.0f;
    mBestHighTime     = 0.0f;
    mReset            = 0;
    mCameraMode       = true;
    mRotationMode     = false;
    mAnimTime         = 0.0f;
    m_block_movement  = 0.35f;
    m_block_direction = Z_POSITIVE;
    m_block_next_direction = X_NEGATIVE;

    m_rot_count = 0.0f;
    m_yaw_count = 0.0f;
    m_test      = 0;
    m_water_position = 0.0f;

    r = 0;
    g = 0;
    b = 0;
}

void TestScene::initialize( OpenPolygonDisplay * display )
{
    m_display = display;
    m_custom_camera = new Camera("test");

    createOverlay();

    CameraSetup();

    ActiveRenderSystem( display );
}

void TestScene::createOverlay(void)
{
    OverlayManager * manager        = OverlayManager::getSingletonPtr();
    Overlay        * overlay_hud    = manager->createOverlay("HUD"); // HUD - Overlay

    /* Create  HUD Elements */
    mText                = new TextElement();
    mText->create( m_display );
    mLightData           = new TextElement();
    mLightData->create( m_display );
    mCamData             = new TextElement();
    mCamData->create( m_display );

    mTimeData0           = new TextElement();
    mTimeData0->create( m_display );
    mTimeData0->getElement()->setVisible(false);

    mTimeData1           = new TextElement();
    mTimeData1->create( m_display );
    mTimeData1->getElement()->setVisible( false );

    /* Modifier HUD Elements */
    mText->setColour  (Vector4f(0,1,0,1));
    mText->setPosition(Vector3f(1,10,0));
    mText->setPanelOptions( TextElement::PANEL_ACTIVE | TextElement::PANEL_SIZE_AUTO );
    mText->getElement()->setVisible(false);

    mLightData->setColour  (Vector4f(1,1,0,1));
    mLightData->setPosition(Vector3f(4,728,0));
    mLightData->setSize    (Vector2f( 0.5f , 0.5f));

    mCamData->setColour  (Vector4f(0,1,1,1));
    mCamData->setPosition(Vector3f(4,708,0));
    mCamData->setSize    (Vector2f( 0.5f , 0.5f));


    mTimeData0->setSize(Vector2f(0.5f,0.5f));
    mTimeData0->setPosition( Vector3f(1,50,0) );

    mTimeData1->setSize(Vector2f(0.5f,0.5f));
    mTimeData1->setPosition( Vector3f(1,70,0) );

    /* Add Element to Overlay */
    //overlay_hud->addElement( mText );
    //overlay_hud->addElement( mTimeData0 );
    //overlay_hud->addElement( mTimeData1 );
}



void TestScene::ActiveRenderSystem( OpenPolygonDisplay * display )
{
    glShadeModel( GL_SMOOTH );


    //---------------------------------------------------
    // Create Lights
    //---------------------------------------------------

//    Color + B = color * shadow;
//    Color + ( color * shadow - Color ) = Color * Shadow;
// 1,0,0,1  + ( -0,5,0,0,-0.5 )          =
// 1,0,1,1  + ( (color * shadow ) - ((color * shadow) * 2) = Color * Shadow


    LightManager * manager = LightManager::getSingletonPtr();
    manager->setBias_Test( 0.0005f );

    Light     * sun_light = manager->createLight("sunlight");

    sun_light->setPosition( Vector3f(0       , 300     ,  300    ) );
    sun_light->setAmbient ( Vector3f( -0.3f  , -0.3f  , -0.3f ) );
    sun_light->setDiffuse ( Vector3f( 0.5f   , 0.5f   , 0.5f  ) );
    sun_light->setSpecular( Vector3f( 0.4f   , 0.4f   , 0.2f  ) );
    sun_light->setRotation( Vector3f( 50.0f  , 0.0f   , 0     ) );

    SpotLight * spotlight0 = manager->createSpotLight("spotlight0");
    SpotLight * spotlight1 = manager->createSpotLight("spotlight1");


    //Shadow Scene wird mit dem SpotLight gezeichnet
    spotlight0->setPosition( Vector3f(  0 , 0 , 0 ) );                  //Light Position for Light  = (positive) -12 -2 -9
    spotlight0->setAmbient ( Vector3f( -0.3f , -0.3f  , -0.3f ) );          //Light Position for Shadow = (negative)
    spotlight0->setDiffuse ( Vector3f( 0.3f , 0.2f  , 0.1f ) );
    spotlight0->setSpecular( Vector3f( 0.6f , 0.6f  , 0.6f ) );
    spotlight0->setRotation( Vector3f( 90.0f , 0.0f , 0 ) );

    spotlight0->setSpotDirection( Vector3f( 0.0f , 1.0f , 0.0f ) );
    spotlight0->setSpotCosCutoff( 60 );

    //Spotlight1
    spotlight1->setPosition( Vector3f( 0.0f , 5.0f , 5.0f ) );
    spotlight1->setAmbient ( Vector3f( 2.0f , 0.0f  , 0.0f ) );
    spotlight1->setDiffuse ( Vector3f( 0.0f , 0.0f  , 0.0f ) );
    spotlight1->setSpecular( Vector3f( 0.1f , 0.1f  , 0.1f ) );
    spotlight1->setRotation( Vector3f( 0 , 0 , 0 ) );

    spotlight1->setSpotDirection( Vector3f( 0.0 , -1.0f , 0.0f ) );
    spotlight1->setSpotCosCutoff( 60 );

    //---------------------------------------------------
    // create & init. System
    //--------------------------------------------------

    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();
    database_manager->addDatabase( new DeformationManager() );


   //Create System & Entity Manager
   MultiRendering       * system          = new MultiRendering("RenderSystem");
   RenderEntityManager  * entity_manager  = new RenderEntityManager();
   RenderProcessManager * process_manager = new RenderProcessManager( entity_manager );

   system->setRenderProcessManager( process_manager );

   //Create Techniques
   SpotShadowTech * spot_shadow_tech    = new SpotShadowTech("SpotShadowTech");
   SpotLightTech  * spot_light_tech     = new SpotLightTech("SpotLightTech");
   ShadowMappingTech * smtech           = new ShadowMappingTech("ShadowMappingTech");
   m_csm_tech                           = new CascadedShadowTech("CascadedShadowTech");
   ScreenSpaceReflection * ssr_tech     = new ScreenSpaceReflection("SSR");
   m_fresnel_tech                       = new FresnelTech("Fresnel_Tech");

   /* Initialized RenderSystem */
   RenderManager::getSingletonPtr()->getSystemManager()->initRenderSystem( system , display );
   RenderManager::getSingletonPtr()->getSystemManager()->addRenderSystem ( system );

   //Add Cascaded Shadow Mapping Technique
   m_csm_tech->setScreen( system->getScreen() );
   m_csm_tech->setScreenPosition( system->getScreenPosition() );
   m_csm_tech->setLight( sun_light );
   system->addTechnique( m_csm_tech );

   //Add Spot Light Technique
   //spot_light_tech->setScreen( system->getScreen() );
   //spot_light_tech->setScreenPosition( system->getScreenPosition() );
   //system->addTechnique( spot_light_tech  ); //diffuse + light

   //Add Shadow Mapping Technique
   smtech->setScreen( system->getScreen() );
   smtech->setScreenPosition( system->getScreenPosition() );
   //system->addTechnique( smtech );
   //smtech->getRenderModulManager()->addModul( new BasicShadowModul() );


   DisplaceTerrain * displace = new DisplaceTerrain();
   displace->create( m_display );

   m_water_terrain = new WaterTerrain();
   m_water_terrain->create( display );
   m_water_terrain->setPosition( Vector3f( 0, 15 ,0 ) );

   entity_manager->addEntity( m_water_terrain );


   //Add Screen Space Reflection
   //ssr_tech->setScreen( system->getScreen() );
   //ssr_tech->setScreenPosition( system->getScreenPosition() );
   //ssr_tech->setSSREntity( water->getEntity() );
   //system->addTechnique( ssr_tech );

   //Add Fresnel Technique
   m_fresnel_tech->setScreen( system->getScreen() );
   m_fresnel_tech->setScreenPosition( system->getScreenPosition() );
   m_fresnel_tech->setFresnelEntity( m_water_terrain->getEntity() );
   m_fresnel_tech->setTerrainEntity( displace );
   m_fresnel_tech->m_reflect_pos = 0.0f;
   system->addTechnique( m_fresnel_tech );


   //---------------------------------------------------
   // Create Coordinaten Spheres
   //---------------------------------------------------

   Sphere * red   = new Sphere();
   Sphere * blue  = new Sphere();
   Sphere * green = new Sphere();

   red->create( m_display );
   red->setPosition( Vector3f(1,0,0) );
   red->setColour  ( Vector4f(1,0,0,1)  );  // color red

   green->create( m_display );
   green->setPosition( Vector3f(0,1,0) );
   green->setColour( Vector4f(0,1,0,1) );   // color green

   blue->create( m_display );
   blue->setPosition( Vector3f(0,0,1));
   blue->setColour ( Vector4f(0,0,1,1) );   // color blue

   //---------------------------------------------------
   // Fill RenderEntityManager
   //---------------------------------------------------

   entity_manager->addEntity( red   );
   entity_manager->addEntity( green );
   entity_manager->addEntity( blue  );

   m_light_test = new Sphere();
   m_light_test->create( m_display );
   m_light_test->setScale( Vector3f( 3 , 3 , 3 ) );



   Terrain * first_terrain = new Terrain();
   first_terrain->create( m_display );
   first_terrain->setPosition( Vector3f( 50 , -1 , 50 ) );

   FrustumEntity * frustum = new FrustumEntity();
   frustum->create( m_display );

   //entity_manager->addEntity( frustum );

   //entity_manager->addEntity( first_terrain );
   entity_manager->addEntity( displace );
   //entity_manager->addEntity( m_light_test );


   //---------------------------------------------------
   // Create Cubes
   //---------------------------------------------------
   createBlocks( entity_manager );

   //---------------------------------------------------
   // RGB - Driver Test Unit ( Collision )
   //---------------------------------------------------
    rgb = TextureManager::getSingletonPtr()->createTexture("./resource/testroute2.png");

   //---------------------------------------------------
   // Frustum Entity
   //---------------------------------------------------
   //FrustumEntity * frustum_test = new FrustumEntity();
   //frustum_test->create( m_display );

   //entity_manager->addEntity( frustum_test );
}

void TestScene::createBlocks( RenderEntityManager * manager )
{
    //---------------------------------------------------
    // Create Cubes
    //---------------------------------------------------



    //---------------------------------------------------
    // Fill RenderEntityManager
    //---------------------------------------------------

//    manager->addEntity( start_block  );
//    manager->addEntity( block_x_512  );
//    manager->addEntity( block_z_512  );
//    manager->addEntity( block_xz_512 );
//    manager->addEntity( m_block_ai   );
//    manager->addEntity( test );
//    manager->addEntity( test2 );
//    manager->addEntity( test3 );
//    manager->addEntity( test4 );
}

void TestScene::CameraSetup(void)
{
    m_camera = CameraManager::getSingletonPtr()->getCamera(MAIN_CAMERA);
    m_camera->setPosition( Vector3f( -12 , -2 , -9 ) );
    m_camera->setPitch( 90.0f );
    m_camera->setYaw( 180.0f );

    /* Achsen Yaw Values */
    // Yaw: 180  ( Z+ )
}

void TestScene::ShowFPS( float time )
{
    stringstream show_best_low_time;
    stringstream show_best_high_time;
    stringstream show_fps;

    if( mReset == 100 )
    {
        mBestLowTime  = 1.0f;
        mBestHighTime = 0.0f;
        mReset = 0;
    }

    if( time < mBestLowTime && time > 0.00000f )
    {
       mBestLowTime = time;
    }

    if( time > mBestHighTime && time < 1.0000f )
    {
        mBestHighTime = time;
    }


    //Show High Time and Low Time
    show_best_low_time  << "L.T " << mBestLowTime;
    show_best_high_time << "H.T " << mBestHighTime;

    // Show FPS and Time per Frame
    //show_fps << " Driver Colour Collision: " << "( " << r << ":" << g << ":" << b << " )";
    show_fps << "Reflect Offset:" << m_csm_tech->test_float;

    std::string show_best = show_best_high_time.str();
    std::string show_low  = show_best_low_time.str();
    std::string fps       = show_fps.str();


    //Draw Best High Time
    mTimeData1->setCaption( show_best );

    //Draw Low Time
    mTimeData0->setCaption( show_low );

    //Draw FPS
    mText->setCaption( fps );

    mReset++;
}


void TestScene::RenderLogic( float time )
{
     ShowFPS( time );

     Vector3f frustumCentroid = Vector3f(0,0,0);

     SpotLight * light = LightManager::getSingletonPtr()->getSpotLight("spotlight0");
     glm::vec3 centroid = FrustumUtils::getCentroid( -1.0 , -50.0 , glm::inverse( light->getMatrix() ) );
               //centroid = FrustumUtils::getCentroid( -1.0d , -50.0d , glm::inverse( m_display->getCamera()->getMatrix() ) );
     frustumCentroid.setX(  centroid.x );
     frustumCentroid.setY(  centroid.y );
     frustumCentroid.setZ(  centroid.z );

     //m_block_ai_pos->setPosition( frustumCentroid );

     m_water_terrain->setPosition( Vector3f(0,m_water_position,0) );

     //Vector3f light_position = light->getPosition();
     //light_position.inverse();
     //m_light_test->setPosition( light_position  );

     //m_camera->AutoRotation( m_block_ai_pos );
     //Driver Controll

//     EditorManager::getSingletonPtr()->createUpdateFileEvent("./scripts/objects.cr");
//     EditorManager::getSingletonPtr()->createUpdateFileEvent("./scripts/fence.cr");

//     /* CPU - Skinning - Animation */

//     Animation * animation = AnimationManager::getSingletonPtr()->getAnimation( mHuman->getObject()->getComponent("animation")->getID() );
//     Mesh      * mesh      = MeshManager::getSingletonPtr()->getMesh( mHuman->getObject()->getComponent("mesh")->getID() );

//     AnimationManager::getSingletonPtr()->prepareVBO( animation , mesh );

//     mAnimTime += time;

//     if( mAnimTime > 1.0f )    // ever second one frame more ...
//     {
//         mAnimTime = 0.0f;     // reset Timer
//         animation->Loop();    // next frame
//     }


//     /* Test Calc. */

//     PixelCoordData data;
//     data.terrain_size = Vector2i( 100  , 100  );
//     data.texture_size = Vector2i( 512  , 512  );
//     // U , V
//     data.player_pos   = Vector2f( m_block_ai_pos->getPosition().getZ() , m_block_ai_pos->getPosition().getX() );
//     m_rgb_pos = Utils::getPixelCoord( data );

//     /* Read RGB Texture Data */
//     uchar * pdata  = rgb->getPixelData();

//     //get RGBA
//     int value_red    = ( ( m_rgb_pos.getY() * 512 ) + m_rgb_pos.getX() ) * 4;
//     int value_green  = value_red + 1;
//     int value_blue   = value_red + 2;

//     //set BGRA
//     b = pdata[ value_red    ];
//     g = pdata[ value_green  ];
//     r = pdata[ value_blue   ];


//     if( m_block_direction == Z_POSITIVE && m_block_ai_pos->getPosition().getZ() > 90.0f )
//     {
//          m_block_direction = X_POSITIVE;
//     }

//     if( m_block_direction == X_POSITIVE && m_block_ai_pos->getPosition().getX() > 86.0f )
//     {
//          m_block_direction = Z_NEGATIVE;
//     }

//     if( m_block_direction == Z_NEGATIVE && m_block_ai_pos->getPosition().getZ() < 12.0f)
//     {
//          m_block_direction = X_NEGATIVE;
//     }

//     if( m_block_direction == X_NEGATIVE && m_block_ai_pos->getPosition().getX() < 15.0f)
//     {
//          m_block_direction = Z_POSITIVE;
//     }


//     if( m_block_direction == X_POSITIVE )
//              m_block_ai_pos->setPosition( Vector3f( m_block_ai_pos->getPosition().getX() + m_block_movement , 0 ,  m_block_ai_pos->getPosition().getZ() ));

//     if( m_block_direction == X_NEGATIVE )
//              m_block_ai_pos->setPosition( Vector3f( m_block_ai_pos->getPosition().getX() - m_block_movement , 0 ,  m_block_ai_pos->getPosition().getZ() ));

//     if( m_block_direction == Z_POSITIVE )
//              m_block_ai_pos->setPosition( Vector3f( m_block_ai_pos->getPosition().getX() , 0 ,  m_block_ai_pos->getPosition().getZ() + m_block_movement ));

//     if( m_block_direction == Z_NEGATIVE )
//              m_block_ai_pos->setPosition( Vector3f( m_block_ai_pos->getPosition().getX() , 0 ,  m_block_ai_pos->getPosition().getZ() - m_block_movement ));
}


void TestScene::CameraMovement(const string & object_name , const KeyEvent & event )
{
    Camera * camera = CameraManager::getSingletonPtr()->getCamera( object_name );

    for( int key : event.getKeys() )
    {
        switch( key  )
        {
             case GLFW_KEY_W:
                    camera->move( Camera::FORWARD  , 0.5f);
                    //camera->move_ex( Camera::FORWARD , 0.5f , m_block_ai_pos );
             break;

             case GLFW_KEY_S:
                    camera->move( Camera::BACKWARD , 0.5f);
                    //camera->move_ex( Camera::BACKWARD , 0.5f , m_block_ai_pos );
             break;

             case GLFW_KEY_A:
                    camera->move( Camera::LEFT , 0.5f);
                    //camera->move_ex( Camera::LEFT , 0.5f , m_block_ai_pos );
             break;

             case GLFW_KEY_D:
                    camera->move( Camera::RIGHT , 0.5f);
                    //camera->move_ex( Camera::RIGHT , 0.5f , m_block_ai_pos );
             break;

        }
    }
}

void TestScene::LightMovement( const std::string & light_name , const KeyEvent & event )
{
    SpotLight * light     = LightManager::getSingletonPtr()->getSpotLight( light_name );

    Position local_pos    = Position();
    local_pos.setPosition( light->getPosition() );

    for( int key : event.getKeys() )
    {
        switch( key  )
        {
             case GLFW_KEY_W:
                    m_camera->move_ex( Camera::FORWARD , 0.5f , &local_pos );
             break;

             case GLFW_KEY_S:
                    m_camera->move_ex( Camera::BACKWARD , 0.5f , &local_pos );
             break;

             case GLFW_KEY_A:
                    m_camera->move_ex( Camera::LEFT , 0.5f , &local_pos );
             break;

             case GLFW_KEY_D:
                    m_camera->move_ex( Camera::RIGHT , 0.5f , &local_pos );
             break;

        }
    }

    light->setPosition( local_pos.getPosition() );
}

void TestScene::LightRotation(const string &light_name, const KeyEvent &event)
{
    for( int key : event.getKeys() )
    {
        switch( key  )
        {
             case GLFW_KEY_W:
                    //m_fresnel_tech->m_reflect_pos =  m_fresnel_tech->m_reflect_pos  + 0.0005f;
                    m_water_position += 0.5f;
                    //m_csm_tech->test_float += 1.0f;
             break;

             case GLFW_KEY_S:
                    //m_fresnel_tech->m_reflect_pos =  m_fresnel_tech->m_reflect_pos  - 0.0005f;
                    m_water_position -= 0.5f;
                    //m_csm_tech->test_float -= 1.0f;
             break;

        }
    }
}


void TestScene::onKeyEvent(const KeyEvent &event)
{
    if( m_test == 1 )
            CameraMovement( MAIN_CAMERA , event );

    if( m_test == 2 )
            LightMovement( "spotlight0" , event );

    if( m_test == 3  )
            LightRotation( "spotlight0" , event );

    for( int key : event.getKeys() )
    {
        if( key == GLFW_KEY_ESCAPE)
        {
            GLFWDisplay * display = (GLFWDisplay *) m_display;

            display->catchMouse( false );
            display->Close();
        }

        if( key  == GLFW_KEY_1)
        {
            GLFWDisplay * display = (GLFWDisplay *) m_display;
            display->catchMouse( true );
        }

        if( key == GLFW_KEY_2)
        {
            GLFWDisplay * display = (GLFWDisplay *) m_display;
            display->catchMouse( false );
        }

        if( key == GLFW_KEY_Q)
        {
           m_test = 1;
        }

        if( key == GLFW_KEY_E)
        {
           m_test = 2;
        }

        if( key == GLFW_KEY_R)
        {
           m_test = 3;
        }

        if( key == GLFW_KEY_KP_8 )
        {
            //Rotation Block
            m_rot_count += 0.9f;
            m_block_ai_pos->setRotation( Vector3f( 0 , m_rot_count , 0 ) );

            //m_camera->AutoFollow( m_block_ai_pos );

//            Vector3f camera_position = m_camera->getPosition();

//            //Rotation Camera
//            m_camera->setYaw( 180.0f - m_block_ai_pos->getRotation().getY() );
//            m_camera->transform();


//            //Position Camera
//            Vector3f  rot_vector  = Vector3f( 0.0f , m_block_ai_pos->getRotation().getY() - m_yaw_count , 0.0f );
//            glm::mat4 rot_matrix  = m_camera->getRotationMatrix( rot_vector );
//            glm::mat4 translation = m_camera->getTranslationMatrix( glm::vec3( m_block_ai_pos->getPosition().getX() , m_block_ai_pos->getPosition().getY() , m_block_ai_pos->getPosition().getZ() ));
//            glm::mat4 transform   = translation * rot_matrix;
//                      transform   = transform   * glm::inverse( translation );

//            //Fix old vector
//            glm::vec4 vector     = glm::vec4( -camera_position.getX() , -(camera_position.getY()) , -(camera_position.getZ()) , 1.0f );  //vector
//            glm::vec4 output     = transform * vector;

//            m_camera->setPosition( Vector3f( -output.x , -output.y , -output.z ) );
//            m_yaw_count = m_block_ai_pos->getRotation().getY();
        }

        if( key == GLFW_KEY_KP_2 )
        {
            m_rot_count -= 0.9f;
            m_block_ai_pos->setRotation( Vector3f( 0 , m_rot_count , 0 ) );
            //m_camera->AutoFollow( m_block_ai_pos );
        }

    }

}




void TestScene::onMouseEvent( const MouseEvent & event )
{
    //MouseEvent is calling by Mouse Movement ( and ) Mouse Clicked

    Vector2f currentMouse = InputManager::getSingletonPtr()->getMousePosition();


    //m_block_ai_pos->setRotation( Vector3f( 0 , -currentMouse.getX() , 0 ) );

    m_camera->setPitch( currentMouse.getY() );
    m_camera->setYaw(   currentMouse.getX() );


//    if( event.isMousePressed() && event.getButton() == GLFW_MOUSE_BUTTON_1 )
//    {
//        Vector2f currentMouse = InputManager::getSingletonPtr()->getMousePosition();
//        Camera * camera       = CameraManager::getSingletonPtr()->getCamera(MAIN_CAMERA);

//        camera->setPitch( currentMouse.getY() );
//        camera->setYaw(   currentMouse.getX() );
//    }


}
