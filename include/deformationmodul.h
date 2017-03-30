#ifndef DEFORMATIONMODUL_H
#define DEFORMATIONMODUL_H

#include "index.h"
#include "rendermodul.h"
#include "meshmanager.h"
#include "shadermanager.h"
#include "positionmanager.h"
#include "deformationmanager.h"

namespace Engine
{

    class DeformationModul : public RenderModul
    {
        public:
            DeformationModul();
            ~DeformationModul() = default;

            void renderProcess(const DrawEvent &event);

    };

}

#endif // DEFORMATIONMODUL_H
