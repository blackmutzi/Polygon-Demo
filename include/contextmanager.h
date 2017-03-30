#ifndef CONTEXTMANAGER_H
#define CONTEXTMANAGER_H

#include "index.h"

namespace Engine
{

class ContextContainer
{
    public:
        ~ContextContainer() = default;
        virtual void loadContainer(void) =0;

    public:
         unsigned int m_context_id;
         bool m_ready_flag;
};

class ContextManager
{
    private:
        ContextManager();
        ContextManager( const ContextManager &){}

        static ContextManager * ptrInstance;
        static unsigned int contextId;

    public:

        static ContextManager * getSingeltonPtr(void);

        //# Single Container Support
        void AddSingleContainer( ContextContainer * container );
        void LoadSingleContainer(void);
        bool isSingleContainerFinish(void);
        void ResetSingleContainer(void);
        void ResetSingleContainerHard(void);

        //# Multi Container Support
        //void AddContainer( ContextContainer * container );
        //void LoadContainer(void);
        //bool isContainerFinish( unsigned int context_id );

    private:

        ContextContainer * m_single_container;
};

}

#endif // CONTEXTMANAGER_H
