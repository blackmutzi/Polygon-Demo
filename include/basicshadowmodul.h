#ifndef BASICSHADOWMODUL_H
#define BASICSHADOWMODUL_H

#include "index.h"
#include "Interface/IShader.h"
#include "renderprocessmanager.h"
#include "shadowevent.h"
#include "meshmanager.h"
#include "positionmanager.h"
#include "shadermanager.h"

namespace Engine
{
    class BasicShadowModul : public RenderModul
    {
        public:
            BasicShadowModul();
            ~BasicShadowModul() = default;

            void renderProcess(const DrawEvent &event);

        private:

            IShader * m_shadow_shader;
    };
}



#endif // BASICSHADOWMODUL_H
