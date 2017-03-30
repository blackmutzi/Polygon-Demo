#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "index.h"
#include "texturemanager.h"
#include "shadermanager.h"
#include "resource.h"

namespace Engine
{
    class Resource;

    using FileList = std::list< std::string >;

    struct ResourceData
    {
        char * data_compress;
        long data_compress_length;
        long data_length;
        std::string file_name;
    };

    struct HeaderData
    {
        const char * type;
        long header_size;
        long header_size_compress;
        long resource_size;
        long resource_size_compress;
    };


    class ResourceManager
    {
         private:
            ResourceManager();
            ResourceManager( const ResourceManager &){}

            static ResourceManager * ptrInstance;

         public:

            static ResourceManager * getSingeltonPtr(void);

            /*!
             * Create a new Resource File
             * \brief createResource
             * \param resoure_name
             * \param directory_input
             */
            void createResource( const std::string & resoure_name , const std::string & directory_input );

            /*!
             * Open Resource
             * \brief getResource
             * \param resource_name
             * \return
             */
            Resource * openResourceFile( const std::string & resource_name );
            Texture  * getTexture( const std::string & texture_name , Resource * resource );
            IShader  * getShader ( const std::string & shader_name  , Resource * resource );

         private:

            //compress functions
            void getFileList        ( const std::string & start_directory , FileList & list );
            void WriteByteDataEnd   ( const std::string & file_name , char bytes[] , int byte_size );
            void LZ4CompressResource( ResourceData & resource , char bytes[] , int byte_size );
            void createResourceData ( ResourceData & resource , const std::string & file_name );
            void createBinaryFile   ( ResourceData & header , ResourceData & content , char header_bytes[] );

            //decompress functions
            void   getHeaderInfo( HeaderData & data , const std::string & resource_name );
            char * LZ4DecompressResource( Resource * resource );
            char * getOriginalResource( const std::string & file_name , Resource * resource );
            void   createFile( const std::string & file_output , const std::string & original_file_path , Resource * resource );

    };

}

#endif // RESOURCEMANAGER_H
