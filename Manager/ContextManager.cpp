#include "contextmanager.h"

using namespace Engine;

unsigned int ContextManager::contextId = 0;

ContextManager * ContextManager::ptrInstance = nullptr;

ContextManager::ContextManager(){
    m_single_container = nullptr;
}

ContextManager * ContextManager::getSingeltonPtr(void)
{
    if( ptrInstance == nullptr )
    {
        ptrInstance = new ContextManager();
    }

    return ptrInstance;
}

void ContextManager::AddSingleContainer( ContextContainer * container )
{
    container->m_ready_flag = false;
    container->m_context_id = contextId;
    contextId++;

    if( m_single_container == nullptr )
            m_single_container = container;
}

bool ContextManager::isSingleContainerFinish(void)
{
    if( m_single_container != nullptr )
    {
       return m_single_container->m_ready_flag;
    }

    return false;
}

void ContextManager::LoadSingleContainer(void)
{
    if( m_single_container != nullptr )
    {
        m_single_container->loadContainer();
        m_single_container->m_ready_flag = true;
    }
}

void ContextManager::ResetSingleContainer(void)
{
    if( m_single_container != nullptr )
    {
        if( m_single_container->m_ready_flag )
                    ResetSingleContainerHard();
    }
}

void ContextManager::ResetSingleContainerHard(void)
{
    m_single_container = nullptr;
}

