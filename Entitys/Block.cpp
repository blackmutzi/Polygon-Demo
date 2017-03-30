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
#include "block.h"

using namespace Engine;

Block::Block()
    : RenderEntity("cube")
{
     //Nothing to do ...
}

Mesh * Block::createMeshTest( const std::string & mesh_resource )
{
    WaveFrontLoader importer;
    importer.load( mesh_resource );

    //make new mesh
    Mesh * new_mesh = new Mesh();

    //for Generate Original Vertices
    new_mesh->mIndices = importer.mIndices;

    //for Generate Original TexCoords
    new_mesh->mIndicesTexcoord = importer.mIndicesTexcoord;
    new_mesh->mTexcoord        = importer.mTexcoord;

    //for Generate Original Normals
    new_mesh->mIndicesNormals  = importer.mIndicesNormals;
    new_mesh->mNormals         = importer.mNormals;

    //for Animation
    new_mesh->mVertices        = importer.mVertices;
    new_mesh->mGroups          = importer.mGroups;

    ContextContainerTest * test_container = new ContextContainerTest( importer );
    ContextManager::getSingeltonPtr()->AddSingleContainer( test_container );

    ContextManager::getSingeltonPtr()->LoadSingleContainer();

    while( !ContextManager::getSingeltonPtr()->isSingleContainerFinish() ){}

    if( ContextManager::getSingeltonPtr()->isSingleContainerFinish() )
    {
        new_mesh->setGLVertexArrayObject( test_container->getObject() );
        ContextManager::getSingeltonPtr()->ResetSingleContainer();
    }


    return new_mesh;
}


void Block::create( OpenPolygonDisplay * display )
{
    //---------------------------------------------------
    // Create Render Basic Shader
    //---------------------------------------------------

    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();

    ShaderManager   * shader_manager   = (ShaderManager   *) database_manager->getDatabase("ShaderManager");
    MeshManager     * mesh_manager     = (MeshManager     *) database_manager->getDatabase("MeshManager");
    PositionManager * position_manager = (PositionManager *) database_manager->getDatabase("PositionManager");


    mEntity    = getEntity();
    mPosition  = position_manager->createPosition();

    /* Einmalige Resource Erstellung  */
    mMesh      = mesh_manager->createMesh("./resource/cube.obj");
    //mMesh      = createMeshTest("./resource/cube.obj");


    mShader    = shader_manager->createShader("Cube");
    shader_manager->addSource( mShader , "cube" , FRAGMENT | VERTEX  );
    mShader->LinkProgram();

    Texture * dirt  = TextureManager::getSingletonPtr()->createTexture("./resource/dirt.jpg");
    Texture * grass = TextureManager::getSingletonPtr()->createTexture("./resource/grass.jpg");

    // Attach
    getEntity()->attach( mMesh     );
    getEntity()->attach( mPosition );
    getEntity()->attach( mShader   );
    getEntity()->attach( CameraManager::getSingletonPtr()->getCamera(MAIN_CAMERA) );

    Textures six_textures;
    six_textures.push_back( dirt );
    six_textures.push_back( dirt );
    six_textures.push_back( grass  );
    six_textures.push_back( dirt  );
    six_textures.push_back( dirt );
    six_textures.push_back( dirt );

    mSurface = TextureManager::getSingletonPtr()->createCubemapping( six_textures , "block");
}


void Block::setTexture( Texture * texture )
{
    Texture * grass = TextureManager::getSingletonPtr()->createTexture("./resource/grass.jpg");

    Textures six_textures;
    six_textures.push_back( texture );
    six_textures.push_back( texture );
    six_textures.push_back( grass  );
    six_textures.push_back( texture  );
    six_textures.push_back( texture );
    six_textures.push_back( texture );

    mSurface = TextureManager::getSingletonPtr()->createCubemapping( six_textures , "block_newsurface");
}

void Block::draw( const DrawEvent & event )
{
    mPosition->transformation();

    if( !event.hasShadowEvent() )
    {

        glDisable( GL_CULL_FACE );

        /* Use Cube Shader */
        mShader->UseProgram();

        mShader->BindMatrix("Projection"  , event.getProjection() );
        mShader->BindMatrix("ModelView"   , event.getWorldView() * mPosition->getMatrix() );
        mShader->BindMatrix("ModelMatrix" , mPosition->getMatrix() );
        mShader->BindMatrix("WorldView"   , event.getWorldView() );
        mShader->BindMatrix("TexMatrix"   , glm::mat4(1.0f) );
        mShader->BindMatrix("NormalMatrix", mPosition->getNormalMatrix( event.getWorldView() * mPosition->getMatrix() ) );

        //Texture Setup
        mShader->BindTexture( mSurface , "surface" , 0 );     // cubeImage

        //glCullFace( GL_FRONT_AND_BACK );

        mMesh->DrawElements();

        //glCullFace( GL_BACK );

        mShader->Unused();

        glEnable( GL_CULL_FACE );

    }else
    {
        event.getShadowEvent().getShadowShader()->BindMatrix("ModelView" , event.getShadowEvent().getViewMatrix() * mPosition->getMatrix() );
        event.getShadowEvent().getShadowShader()->BindUniform1f("Mode",0.0f);

        mMesh->DrawElements();

    }
}
