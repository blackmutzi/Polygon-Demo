#ifndef DEFORMATION_H
#define DEFORMATION_H

#include "index.h"
#include "entity.h"

namespace Engine
{
    using DeformationToolList = std::list< Entity * >;
    class Deformation : public IComponent
    {
        public:
            Deformation();
            ~Deformation()=default;


            void addDeformationTool( Entity * entity );
            DeformationToolList getDeformationToolList();

            void setValid( bool );
            bool isValid(void);

            Vector3fv getDeformationToolVertices(void);
            Vector3fv getDeformationEntityVertices(void);
            Vector3fv getUpdateVertices(void);

            void setUpdateVertices( Vector3fv update_vertices );

            void setDeformationToolVertices( Vector3fv vertices );
            void setDeformationToolIndices( Vector3fv indices );

            void setDeformationEntityVertices( Vector3fv vertices );
            void setDeformationEntityIndices( Vector3fv indices );

        private:

            bool m_valid;

            Vector3fv m_def_tool_indices;
            Vector3fv m_def_tool_vertices;

            Vector3fv m_def_entity_indices;
            Vector3fv m_def_entity_vertices;

            Vector3fv m_update_vertices;
            DeformationToolList m_tool_list;
    };

}

#endif // DEFORMATION_H
