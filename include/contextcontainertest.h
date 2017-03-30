#ifndef CONTEXTCONTAINERTEST_H
#define CONTEXTCONTAINERTEST_H

#include "index.h"
#include "contextmanager.h"
#include "glvertexarrayobject.h"
#include "wavefrontloader.h"
#include "meshmanager.h"

namespace Engine
{

class ContextContainerTest : public ContextContainer
{
    public:
        ContextContainerTest( WaveFrontLoader & loader );

        void loadContainer();

        GLVertexArrayObject * getObject(void);

    private:

        GLVertexArrayObject * m_result;
        WaveFrontLoader m_loader;
};

}

#endif // CONTEXTCONTAINERTEST_H
