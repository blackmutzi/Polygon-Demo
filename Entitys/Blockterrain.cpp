#include "blockterrain.h"

using namespace Engine;

BlockTerrain::BlockTerrain()
    : RenderEntity("BlockTerrain")
{
}

void BlockTerrain::create( OpenPolygonDisplay * display )
{
    mEntity   = getEntity();

    //---------------------------------------------------
    // Component Databases ...
    //---------------------------------------------------

    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();

    ShaderManager   * shader_manager   = (ShaderManager   *) database_manager->getDatabase("ShaderManager");
    MeshManager     * mesh_manager     = (MeshManager     *) database_manager->getDatabase("MeshManager");
    PositionManager * position_manager = (PositionManager *) database_manager->getDatabase("PositionManager");


    mMesh     = mesh_manager->createMesh("./resource/cube.obj");
    mPosition = position_manager->createPosition();

    //Create Custom Shader
    mShader    = shader_manager->createShader("BlockTerrain"); //add Vertex Attribute
    shader_manager->addSource( mShader , "blockterrain" , FRAGMENT | VERTEX );
    mShader->LinkProgram();

    //Attach
    getEntity()->attach( mMesh );
    getEntity()->attach( mPosition );
    getEntity()->attach( mShader );
    getEntity()->attach( CameraManager::getSingletonPtr()->getCamera( MAIN_CAMERA ) );

    mBuffer = new MatrixBuffer( mEntity );
    Vector3fv tbo_data;

    int x , z ;

       for( x = 0; x < 1000 ; x++)
       {
           for( z = 0; z < 1000 ; z++)
           {
               tbo_data.push_back(  Vector3f( x * 2.0f , 4.0f  , z * 2.0f ) );
           }
       }
     mBuffer->create( 1000 * 1000 , tbo_data , false );


     //Create Texture
     Texture * dirt  = TextureManager::getSingletonPtr()->createTexture("./resource/dirt.jpg");
     Texture * grass = TextureManager::getSingletonPtr()->createTexture("./resource/grass.jpg");

     //Create Cube Texture

     Textures six_textures;
     six_textures.push_back( dirt );
     six_textures.push_back( dirt );
     six_textures.push_back( grass  );
     six_textures.push_back( dirt );
     six_textures.push_back( dirt );
     six_textures.push_back( dirt );

     mCubeMapping = new CubeMapping( getEntity() );
     mCubeMapping->create( six_textures , "terrainblock_cube");
}

void BlockTerrain::draw(const DrawEvent &event)
{
    //mMesh->clearMatrixBuffer( mModelMatrixBuffer , 1000 );

   if( !event.hasShadowEvent() )
   {

    mShader->UseProgram();

    mShader->BindMatrix("Projection" , event.getProjection() );
    mShader->BindMatrix("WorldView"  , event.getWorldView() );
    mShader->BindMatrix("TexMatrix"  , glm::mat4(1.0f) );

    //Bind Cube Texture
    mCubeMapping->enable( 0 );

    //Bind TBO
    mBuffer->enable( 1 );

    mMesh->DrawElementsIndirect( 1000 );

    mShader->Unused();

   }else
   {
       event.getShadowEvent().getShadowShader()->BindMatrix("LightView" ,  event.getShadowEvent().getViewMatrix() );
       event.getShadowEvent().getShadowShader()->BindUniform1f("Mode",1.0f);

       mBuffer->enable( 1 );
       mMesh->DrawElementsIndirect( 1000 );
   }
}
