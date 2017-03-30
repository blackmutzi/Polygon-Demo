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
#include "terrain.h"

using namespace Engine;

Terrain::Terrain()
    :RenderEntity("area")
{
    //nothing to do...
}

void Terrain::create( OpenPolygonDisplay * display )
{
    mEntity      = getEntity();

    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();

    ShaderManager    * shader_manager    = (ShaderManager   *) database_manager->getDatabase("ShaderManager");
    MeshManager      * mesh_manager      = (MeshManager     *) database_manager->getDatabase("MeshManager");
    PositionManager  * position_manager  = (PositionManager *) database_manager->getDatabase("PositionManager");
    AnimationManager * animation_manager = (AnimationManager*) database_manager->getDatabase("AnimationManager");

    mMesh        = mesh_manager->createMesh("./resource/terrain.obj");
    mPosition    = position_manager->createPosition();

    mShader      = shader_manager->createShader("Texture");
    shader_manager->addSource( mShader , "texture" , FRAGMENT | VERTEX );
    mShader->LinkProgram();

    mEntity->attach( mMesh     );
    mEntity->attach( mPosition );
    mEntity->attach( mShader   );
    mEntity->attach( CameraManager::getSingletonPtr()->getCamera( MAIN_CAMERA ) );

    //mPosition->setPosition( Vector3f( 50 , -1 , 50 ) );
    mPosition->setScale(    Vector3f( 13 ,  0 , 13 ) );

    mSurface = TextureManager::getSingletonPtr()->createTexture("./resource/glz_14.jpg"); //./resource/glz_14.jpg
    TextureManager::getSingletonPtr()->loadTexture( mSurface , true );

    mNormalSurface = TextureManager::getSingletonPtr()->createTexture("./resource/14normal.png");
    TextureManager::getSingletonPtr()->loadTexture( mNormalSurface , true );

    mHeightMap = TextureManager::getSingletonPtr()->createTexture("./resource/heightmap.png");
    TextureManager::getSingletonPtr()->loadTexture( mHeightMap , false );

    /* Texture Unit Test */

    TextureUnit * tu_test = new TextureUnit("TextureUnitTest");

    Texture * rgb      = tu_test->loadTexture("./resource/testroute2.png");
    Texture * grey     = tu_test->loadTexture("./resource/grey.png");
    Texture * blackred = tu_test->loadTexture("./resource/blackred.png");
    Texture * sand     = tu_test->loadTexture("./resource/sand.jpg");
    Texture * uni      = tu_test->loadTexture("./resource/uni.jpg");


    mSurface = tu_test->colour_rgb_op( rgb , grey , blackred , sand );
    mSurface = tu_test->colour_alpha_add_op( 0.4f , mSurface , uni );



//    glBindTexture( GL_TEXTURE_2D , mSurface->getID() );

//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

//    glBindTexture( GL_TEXTURE_2D , 0 );
}


void Terrain::draw( const DrawEvent & event )
{
    mPosition->transformation();

   if( !event.hasShadowEvent() )
   {

    mShader->UseProgram();

    mShader->BindMatrix("Projection"  , event.getProjection() );
    mShader->BindMatrix("ModelView"   , event.getWorldView() * mPosition->getMatrix() );
    mShader->BindMatrix("ModelMatrix" , mPosition->getMatrix() );
    mShader->BindMatrix("WorldView"   , event.getWorldView() );
    mShader->BindMatrix("NormalMatrix", glm::transpose( glm::inverse( event.getWorldView() * mPosition->getMatrix() )) ); // transpose( inverse( ModelView ))

    Position texmatrix;
    texmatrix.setScale( Vector3f( 5 ,5 , 5 ) );
    texmatrix.setRotation( Vector3f( 0 , -90.0f , 0) );
    texmatrix.transformation();

    mShader->BindMatrix("TexMatrix"  , texmatrix.getMatrix() );

    //Texture Setup
    mShader->BindUniform1f("Displacement"  , 0.0f );                   // Displacement Mapping on / off ( 1.0f , 0.0f )
    mShader->BindUniform1f("TexCoordModus" , 8.0f );                   // TextureCoord Modus 1
    mShader->BindTexture( mSurface         , "image"     , 0 ); // Set Color  Texture
    mShader->BindTexture( mNormalSurface   , "tNormals"  , 1 ); // Set Normal Texture
    mShader->BindTexture( mHeightMap       , "tHeightMap", 2 ); // Set HeightMap Texture

    //Draw Mode
    mMesh->DrawElements();

    mShader->Unused();

   }else
   {
       //glDisable( GL_CULL_FACE );

       event.getShadowEvent().getShadowShader()->BindMatrix("ModelView" ,  event.getShadowEvent().getViewMatrix() * mPosition->getMatrix() );
       event.getShadowEvent().getShadowShader()->BindUniform1f("Mode",0.0f);

       mMesh->DrawElements();

       //glEnable( GL_CULL_FACE );
   }
}

void Terrain::setPosition( const Vector3f & position )
{
    mPosition->setPosition( position );
}
