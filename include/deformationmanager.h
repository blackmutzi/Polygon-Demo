/*
   Copyright (C) 2014 - 2016 Mutzii

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
#ifndef DEFORMATIONMANAGER_H
#define DEFORMATIONMANAGER_H

#include "index.h"
#include "deformation.h"
#include "entitymanager.h"
#include "meshmanager.h"
#include "frustum.h"

namespace Engine
{

    class DeformationManager : public ITempDatabase< Deformation >
    {
        public:
            DeformationManager();
            ~DeformationManager() = default;

            Deformation * createDeformation(void);
            void createEvent( Entity * deformation_entity , Entity * deformation_tool );

            Deformation * getDeformation( uint container_id );
            void destroy( uint container_id );

        private:

            //Phase 1
            Deformation * _deformation_collect_info( Entity * deformation_entity , Entity * deformation_tool  );

            //Phase 2
            void _deformation_search( Deformation * storage );

            //Phase 3
            void _deformation_new_vertices( Deformation * storage , Mesh * mesh_def_entity  );
    };

}
#endif // DEFORMATIONMANAGER_H
