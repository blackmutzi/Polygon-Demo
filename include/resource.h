#ifndef RESOURCE_H
#define RESOURCE_H

#include "index.h"

namespace Engine
{
    using ResourceHeader = std::map< std::string , Vector2f >;

    class Resource
    {
        public:
           explicit Resource( const std::string & name );
           ~Resource()=default;

           bool existsResource( const std::string & original_resource_path ) const;
           void addOriginalResource( const std::string & original_resource_path , const Vector2f & byte_start_end );
           void setName(const std::string & name );
           void setStartByte( long start_byte );
           void setResourceSizeCompress( long resource_size_compress );
           void setResourceSize( long resource_size_original );

           const std::string getName(void) const;
           long  getStartByte(void) const;
           long  getResourceSize(void) const;
           long  getResourceSizeCompress(void) const;
           const Vector2f getByteData( const std::string & original_resource_path ) const;

        private:
            std::string m_name;
            long m_start_byte;
            long m_resource_size_original;
            long m_resource_size_compress;
            ResourceHeader m_resource_header;
    };

}

#endif // RESOURCE_H
