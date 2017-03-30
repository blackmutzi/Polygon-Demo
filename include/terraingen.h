#ifndef TERRAINGEN_H
#define TERRAINGEN_H

#include "meshmanager.h"
#include "scenemanager.h"

namespace Engine
{

    class TerrainGen
    {
        public:
            TerrainGen( const Vector3i & terrain_size );
            ~TerrainGen() = default;

            void GenerateTerrain(void);
            void GenerateTerrain( Texture * height_map );

            Mesh * getTerrain(void);

        private:

            Vector3i m_terrain_size;
            Mesh   * m_terrain;
            Mesh   * m_terrain_temp;
    };

}

#endif // TERRAINGEN_H
