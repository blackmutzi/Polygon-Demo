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
#include "sphere.h"

using namespace Engine;

Sphere::Sphere()
    : RenderEntity("sphere")
{}

void Sphere::create( OpenPolygonDisplay * display )
{
   mEntity    = getEntity();

   Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();

   ShaderManager    * shader_manager    = (ShaderManager   *) database_manager->getDatabase("ShaderManager");
   MeshManager      * mesh_manager      = (MeshManager     *) database_manager->getDatabase("MeshManager");
   PositionManager  * position_manager  = (PositionManager *) database_manager->getDatabase("PositionManager");

   mPosition  = position_manager->createPosition();
   mMesh      = mesh_manager->createMesh("./resource/sphere.obj");

   mShader    = shader_manager->createShader("ColorShader");
   shader_manager->addSource( mShader , "color" , FRAGMENT | VERTEX );
   mShader->LinkProgram();

   mEntity->attach( mPosition );
   mEntity->attach( mMesh );
   mEntity->attach( mShader );
   mEntity->attach( CameraManager::getSingletonPtr()->getCamera( MAIN_CAMERA ) );

   mPosition->setScale( Vector3f( 0.08f , 0.08f , 0.08f ) );
}


void Sphere::draw( const DrawEvent & event )
{
   mPosition->transformation();

   if( !event.hasShadowEvent() )
   {

    mShader->UseProgram();

    mShader->BindMatrix("Projection"  , event.getProjection() );
    mShader->BindMatrix("ModelView"   , event.getWorldView() * mPosition->getMatrix() );
    mShader->BindMatrix("ModelMatrix" , mPosition->getMatrix() );
    mShader->BindMatrix("WorldView"   , event.getWorldView() );
    mShader->BindMatrix("TexMatrix"   , glm::mat4(1.0f) );
    mShader->BindMatrix("NormalMatrix", glm::transpose( glm::inverse( event.getWorldView() * mPosition->getMatrix() )) ); // transpose( inverse( ModelView ))

    //Color Setup
    mShader->BindVec4f("colour", mColour );

    mMesh->DrawElements();

    mShader->Unused();

   }else
   {
       event.getShadowEvent().getShadowShader()->BindMatrix("ModelView" ,  event.getShadowEvent().getViewMatrix() * mPosition->getMatrix() );
       event.getShadowEvent().getShadowShader()->BindUniform1f("Mode",0.0f);

       mMesh->DrawElements();
   }
}

void Sphere::setScale( const Vector3f & scale )
{
    mPosition->setScale( scale );
}

void Sphere::setColour( const Vector4f & colour )
{
    mColour = colour;
}

void Sphere::setPosition( const Vector3f & position )
{
    mPosition->setPosition( position );
}
