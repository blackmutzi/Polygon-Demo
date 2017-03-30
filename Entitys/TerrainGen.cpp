#include "terraingen.h"

using namespace Engine;

TerrainGen::TerrainGen( const Vector3i & terrain_size )
    : m_terrain_size( terrain_size ) ,
      m_terrain( nullptr )
{
    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();
    MeshManager      * mesh_manager      = (MeshManager     *) database_manager->getDatabase("MeshManager");

    m_terrain_temp = mesh_manager->createMesh("./resource/terrain.obj");
}

void TerrainGen::GenerateTerrain(void)
{
    Vector3fv basic_vertices = m_terrain_temp->getOriginalVertices();
    Vector3fv normals        = m_terrain_temp->GenerateTriangleNormals( basic_vertices );
    Vector3fv terrain_vertices;
    Vector2fv points;

    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();
    MeshManager      * mesh_manager      = (MeshManager     *) database_manager->getDatabase("MeshManager");

    int vertice_counter = 0;
    for( int width = 0 ; width < m_terrain_size.getX() ; width++ )
    {
        for( int height = 0; height < m_terrain_size.getY() ; height++ )
        {
            for( Vector3f vertice : basic_vertices )
            {
                // normal * terrain_height + old_vertice
                terrain_vertices.push_back( vertice +  Vector3f( width * 2 , 0 , height * 2 ));

                //points
                points.push_back( Vector2f( width , height ) );

                //next TexCoords
                vertice_counter++;
            }
            vertice_counter = 0;
        }
    }

    m_terrain = mesh_manager->createMesh("terrain_gen", terrain_vertices );

    AttributeCmd cmd;
    cmd.attribute_id = 0x9;
    cmd.type         = GL_FLOAT;
    cmd.vector_size  = VECTOR_SIZE_2;
    cmd.instance     = false;
    cmd.size_data    = sizeof(float) * points.size() * 2 ;

    float * data     = m_terrain->convert2fv( points );
    cmd.data         = data;

    m_terrain->getVertexArrayObject()->AttachBuffer( cmd );
    delete[] data;
}

void TerrainGen::GenerateTerrain( Texture * height_map )
{
    Vector3fv basic_vertices = m_terrain_temp->getOriginalVertices();
    Vector2fv texcoords      = m_terrain_temp->getOriginalTexCoords();
    Vector3fv normals        = m_terrain_temp->GenerateTriangleNormals( basic_vertices );
    Vector3fv terrain_vertices;
    Vector2fv points;

    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();
    MeshManager      * mesh_manager      = (MeshManager     *) database_manager->getDatabase("MeshManager");

    double scale        = m_terrain_size.getZ();
    double y            = 0;
    double pixi         = (double) 1 / 255;
    double tcs          = (double) 1 / m_terrain_size.getX();
    double tct          = (double) 1 / m_terrain_size.getY();
    Vector2f tc         = Vector2f(0,0);
    Vector2i pc         = Vector2i(0,0);
    Vector3i pixel      = Vector3i(0,0,0);
    Vector3f normal     = Vector3f(0,0,0);

    int vertice_counter = 0;
    for( int width = 0 ; width < m_terrain_size.getX() ; width++ )
    {
        for( int height = 0; height < m_terrain_size.getY() ; height++ )
        {
            for( Vector3f vertice : basic_vertices )
            {
                //old Texcoords
                tc      = texcoords[ vertice_counter ];
                normal  = normals[ vertice_counter ];

                //new Texcoords
                tc    = Vector2f( tcs * tc.getX() + tcs * width , tct * tc.getY() + tct * height );
                //new PixelCoords
                pc    = Vector2i( m_terrain_size.getX() * tc.getX() , m_terrain_size.getY() * tc.getY() );
                //new Pixel
                pixel = Utils::getPixelData( height_map , pc );
                //new Terrain Height
                y     = ( 0.30f * pixi * pixel.getX() + 0.59f * pixi * pixel.getY() + 0.11f * pixi *pixel.getZ() ) * scale;

                // normal * terrain_height
                normal = normal * y;
                // normal * terrain_height + old_vertice
                terrain_vertices.push_back( vertice + normal + Vector3f( width * 2  , 0 , height * 2 ));

                //points
                points.push_back( Vector2f( width , height ) );

                //next TexCoords
                vertice_counter++;
            }
            vertice_counter = 0;
        }
    }

    m_terrain = mesh_manager->createMesh("terrain_gen", terrain_vertices );

    AttributeCmd cmd;
    cmd.attribute_id = 0x9;
    cmd.type         = GL_FLOAT;
    cmd.vector_size  = VECTOR_SIZE_2;
    cmd.instance     = false;
    cmd.size_data    = sizeof(float) * points.size() * 2 ;

    float * data     = m_terrain->convert2fv( points );
    cmd.data         = data;

    m_terrain->getVertexArrayObject()->AttachBuffer( cmd );
    delete[] data;
}


Mesh * TerrainGen::getTerrain(void)
{
    return m_terrain;
}

