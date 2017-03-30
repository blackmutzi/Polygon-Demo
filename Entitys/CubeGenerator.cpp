#include "cubegenerator.h"

using namespace Engine;

CubeGenerator::CubeGenerator(int draw_cubes)
    : m_draw_cubes ( draw_cubes ) ,
      m_mesh( nullptr )           ,
      m_position( nullptr )       ,
      m_shader( nullptr )         ,
      m_buffer( nullptr )         ,
      m_cube_mapping( nullptr )   ,
      RenderEntity("CubeGenerator")
{}

CubeGenerator::~CubeGenerator()
{

    delete m_buffer;

    delete m_cube_mapping;
}

void CubeGenerator::create( OpenPolygonDisplay * display )
{
    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();

    ShaderManager   * shader_manager   = (ShaderManager   *) database_manager->getDatabase("ShaderManager");
    MeshManager     * mesh_manager     = (MeshManager     *) database_manager->getDatabase("MeshManager");
    PositionManager * position_manager = (PositionManager *) database_manager->getDatabase("PositionManager");

    m_position = position_manager->createPosition();
    m_mesh     = mesh_manager->createMesh("./resource/cube.obj");

    //Cube Generator Shader
    m_shader    = shader_manager->createShader("BlockTerrain");
    shader_manager->addSource( m_shader , "blockterrain" , FRAGMENT | VERTEX );
    m_shader->LinkProgram();

    //Attach
    getEntity()->attach( m_mesh );
    getEntity()->attach( m_position );
    getEntity()->attach( m_shader );
    getEntity()->attach( CameraManager::getSingletonPtr()->getCamera( MAIN_CAMERA ));

    //MatrixBuffer
    m_buffer = new MatrixBuffer( getEntity() );

    Vector3fv tbo_data_empty;
    tbo_data_empty.resize( m_draw_cubes );
    m_buffer->create( m_draw_cubes , tbo_data_empty , true );

    //CubeMapping
    m_cube_mapping = new CubeMapping( getEntity() );
}

void CubeGenerator::draw(const DrawEvent &event)
{
    m_shader->UseProgram();

    m_shader->BindMatrix("Projection" , event.getProjection() );
    m_shader->BindMatrix("WorldView"  , event.getWorldView() );
    m_shader->BindMatrix("TexMatrix"  , glm::mat4(1.0f) );

    //Bind Cube Texture
    m_cube_mapping->enable( 0 );

    //Bind Cube Positions
    m_buffer->enable( 1 );

    //Draw Cubes
    m_mesh->DrawElementsIndirect( m_draw_cubes );

    //finish
    m_shader->Unused();
}

void CubeGenerator::setCubeTextures(Textures textures)
{
    m_cube_mapping->create( textures , "CubeGeneratorTextures");
}

void CubeGenerator::setCubePositions(Vector3fv positions)
{
    m_cube_positions = positions;
    m_buffer->setTBOData( m_cube_positions );
}


