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
#ifndef SHADOWMAPPINGTECH_H
#define SHADOWMAPPINGTECH_H

#include "index.h"
#include "rendermanager.h"
#include "framebuffermanager.h"
#include "shadermanager.h"
#include "positionmanager.h"
#include "lightmanager.h"
#include "GLTechnique.h"
#include "frustum.h"
#include "basicshadowmodul.h"

namespace Engine
{

class ShadowMappingTech : public GLTechnique
{
    public:
        explicit ShadowMappingTech( const std::string & tech_name );
        ~ShadowMappingTech()=default;

        void Create(void);
        void Prepare(void);
        void Update(void);

        void Render( Texture * basic );

    private:

        glm::mat4 m_ortho;

        OpenPolygonDisplay * m_display;
        IShader     * m_basic_shadow;
        IShader     * m_shadow_mapping;
        FrameBuffer * m_depth_fbo;

        RenderModulManager * m_render_modul_manager;
        BasicShadowModul   * m_basic_shadow_modul;
};


}

#endif // SHADOWMAPPINGTECH_H
