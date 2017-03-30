#include "resource.h"

using namespace Engine;

Resource::Resource( const std::string & name )
    : m_name( name ) ,
      m_start_byte( 0 ) ,
      m_resource_size_original( 0 ) ,
      m_resource_size_compress( 0 )
{}

void Resource::addOriginalResource(const string &original_resource_path, const Vector2f & byte_start_end)
{
    m_resource_header.insert( ResourceHeader::value_type( original_resource_path , byte_start_end ) );
}

void Resource::setResourceSizeCompress(long resource_size_compress)
{
    m_resource_size_compress = resource_size_compress;
}

void Resource::setResourceSize(long resource_size)
{
    m_resource_size_original = resource_size;
}

void Resource::setStartByte(long start_byte)
{
    m_start_byte = start_byte;
}

void Resource::setName(const string &name)
{
    m_name = name;
}

long Resource::getResourceSizeCompress(void) const
{
    return m_resource_size_compress;
}

long Resource::getResourceSize(void) const
{
    return m_resource_size_original;
}

long Resource::getStartByte(void) const
{
    return m_start_byte;
}

const std::string Resource::getName(void) const
{
    return m_name;
}

bool Resource::existsResource( const std::string & original_resource_path ) const
{
    for( ResourceHeader::value_type pair : m_resource_header )
    {
        if( pair.first == original_resource_path )
                return true;
    }

    return false;
}

const Vector2f Resource::getByteData(const string &original_resource_path) const
{
    for( ResourceHeader::value_type pair : m_resource_header )
    {
        if( pair.first == original_resource_path )
                    return pair.second;
    }

    return Vector2f(0,0);
}
