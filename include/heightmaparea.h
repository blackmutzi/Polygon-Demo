#ifndef HEIGHTMAPAREA_H
#define HEIGHTMAPAREA_H

#include "index.h"
#include "rendercomponent.h"
#include "area.h"

namespace Engine
{

    class HeightMapArea : public Area
    {
        public:
            HeightMapArea();
            ~HeightMapArea()=default;

            void create( OpenPolygonDisplay * display ) override final ;

            void draw(const DrawEvent &event) override final ;

            void setHeightMap( Texture * texture );

        private:

            Texture * m_height_map;


    };

}

#endif // HEIGHTMAPAREA_H
