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
#include "human.h"

using namespace Engine;

Human::Human()
    : RenderEntity("human")
{
    mSize = 0;
}

void Human::create( OpenPolygonDisplay * display )
{
    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();

    ShaderManager    * shader_manager    = (ShaderManager   *) database_manager->getDatabase("ShaderManager");
    MeshManager      * mesh_manager      = (MeshManager     *) database_manager->getDatabase("MeshManager");
    PositionManager  * position_manager  = (PositionManager *) database_manager->getDatabase("PositionManager");
    AnimationManager * animation_manager = (AnimationManager*) database_manager->getDatabase("AnimationManager");


    mEntity    = getEntity();

    mMesh      = mesh_manager->createMesh("./resource/human2.obj"); //human2.obj
    mSize      = mMesh->getOriginalVertices().size() * 3 ;

    mPosition  = position_manager->createPosition();

    //// DEPRECATED
    mShader    = shader_manager->createShader("Animation");
    shader_manager->addSource( mShader , "animation"   , VERTEX  );
    mShader->LinkProgram();

    /* Transform Feedback TestCase */
    //const GLchar* feedbackVaryings[] = { "outValue" };
    //glTransformFeedbackVaryings( mShader->getProgram() , 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);

    /* Create Draw Colour Shader */
    mColorShader = shader_manager->createShader("AnimationColorShader");
    shader_manager->addSource( mColorShader ,  "color" , FRAGMENT | VERTEX );
    mColorShader->LinkProgram();

    //Make Animation and create BoneIndex & BoneWeight Buffer

    mAnimation = animation_manager->createAnimation("laufen","./resource/human3.bvh", mMesh ); //human3.bvh

    //Attach Objects to Entity
    mEntity->attach( mMesh );
    mEntity->attach( mPosition );
    mEntity->attach( mShader );
    mEntity->attach( mAnimation );
    mEntity->attach( CameraManager::getSingletonPtr()->getCamera( MAIN_CAMERA ) );

    //Prepare Model Matrix

    mPosition->setPosition( Vector3f( -6 , 14 , -15 ) );
    mPosition->setScale(    Vector3f( 4 , 4 , 4 ) );
    mPosition->setRotation( Vector3f( 0, 90.0f , 0 ) );

}

void Human::draw( const DrawEvent & event )
{
    mPosition->transformation();

    /* Draw Object */
    mColorShader->UseProgram();

    mColorShader->BindMatrix("Projection" ,  event.getProjection()  );
    mColorShader->BindMatrix("ModelView"  ,  event.getWorldView() * mPosition->getMatrix() );
    mColorShader->BindMatrix("ModelMatrix",  mPosition->getMatrix() );
    mColorShader->BindMatrix("WorldView"  ,  event.getWorldView()        );
    mColorShader->BindMatrix("TexMatrix"  ,  glm::mat4(1.0f)        );
    mColorShader->BindMatrix("NormalMatrix", glm::transpose( glm::inverse( event.getWorldView() * mPosition->getMatrix() )) ); // transpose( inverse( ModelView ))

    mColorShader->BindVec4f("colour",Vector4f(1,0,0,1)); //red

    //mMesh->getDoubleBuffer()->update( 0 , mSize * sizeof(float) , mAnimation->getData() );
    //mMesh->getDoubleBuffer()->SwapBuffers();

    //mMesh->DrawShadowElements(); // Draw with Second VAO (Shadow)

    mColorShader->Unused();

}

void Human::shadowDraw( const ShadowEvent & event )
{
    mPosition->transformation();

    glm::mat4 modelview =  event.getViewMatrix() * mPosition->getMatrix();

    event.getShadowShader()->BindMatrix("ModelView", modelview );

    //mMesh->DrawShadowElements();
}

Entity * Human::getObject(void)
{
    return mEntity;
}

