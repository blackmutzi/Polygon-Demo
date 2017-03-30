#include "resourcemanager.h"

using namespace Engine;

ResourceManager * ResourceManager::ptrInstance = nullptr;

ResourceManager::ResourceManager(){}

ResourceManager * ResourceManager::getSingeltonPtr(void)
{
    if( ptrInstance == nullptr )
    {
        ptrInstance = new ResourceManager();
    }

    return ptrInstance;
}

void ResourceManager::WriteByteDataEnd( const std::string & file_name , char bytes[] , int byte_size )
{
    ofstream byte_file( file_name.c_str() , ios::out | ios::app | ios::binary );

    if( byte_file.is_open() )                    // open binary output file
    {
        //jump to end of file
        byte_file.seekp( 0 , ios::end );         // jump to end of file

        //add Bytes
        byte_file.write( bytes , byte_size );    // add bytes to binary output file

        //close file
        byte_file.close();                       // close binary output file
    }

}

void ResourceManager::getFileList( const std::string & start_directory  , FileList & list)
{

#ifdef __linux__

    struct dirent *files;
    DIR * directory = opendir( start_directory.c_str() );

    while( files = readdir( directory ) )
    {
        if( files->d_type & DT_DIR ) //is a Directory
        {
            string directory_name;
            directory_name.append( files->d_name );

            if( directory_name == "." || directory_name == "..")
                    continue;

            string directory_string;
            directory_string.append( start_directory );
            directory_string.append( files->d_name   );
            directory_string.append("/");

            getFileList( directory_string , list );

        }else // is a File
        {
            string file_string;
            file_string.append( start_directory );
            file_string.append( files->d_name );
            list.push_back( file_string );
        }
    }

    closedir( directory );    
#endif



}


void ResourceManager::createResource(const string &resource_name, const string & directory_start )
{
    FileList file_list;
    getFileList( directory_start , file_list );

    char * file_data   = 0;
    long   file_length = 0;
    int    start_bytes = 0;

    //--------------------------------------------------------
    // Iterator by Orignal files
    for( std::string file : file_list )
    {
        ifstream is;
        is.open( file.c_str() , ios::binary );

        if( is.is_open() )
        {
            //get length of file
            is.seekg( 0 , std::ios::end );
            file_length = is.tellg();
            is.seekg( 0 , std::ios::beg );

            file_data = new char[ file_length ];

            //read data as a block
            is.read( file_data , file_length );
            is.close();
        }

        // Write Filesystem Header
        //---------------------------------------
        ofstream header("_header" , ios::out | ios::app | ios::binary );
        if( header.is_open() )
        {
            header << file << " ";
            header << file_length << " ";
            header << start_bytes << "\n";
        }
        header.close();
        //---------------------------------------

        //Write Resource Content
        //---------------------------------------
        WriteByteDataEnd( "_resource" ,  file_data , file_length );
        delete[] file_data;
        file_data = 0;
        //----------------------------------------

        start_bytes += file_length;
    }

    //------------------------------------------------
    // Resource Filesystem Header
    // create compress data
    //------------------------------------------------
    ResourceData resource_header;
    createResourceData( resource_header , "_header");
    //------------------------------------------------

    //-----------------------------------------------
    // Resource - Content
    // create compress data
    //-----------------------------------------------
    ResourceData resource_data_content;
    createResourceData( resource_data_content , "_resource" );
    resource_data_content.file_name = resource_name;
    //------------------------------------------------

    //------------------------------------------------
    // Resource Header - First 512 bytes
    //------------------------------------------------
    char * header_info = new char[ 512 ];
    memset( header_info , '\0' , 512 );

    //------------------------------------------------
    // Create Header Info
    stringstream info;
    info << "Lib: LZ4 \n";
    info << "Header-Size: " << resource_header.data_length << "\n";
    info << "Header-Size-Compress: "  << resource_header.data_compress_length << "\n";
    info << "Content-Size: " << resource_data_content.data_length << "\n";
    info << "Content-Size-Compress: " << resource_data_content.data_compress_length << "\n";

    //------------------------------------------------
    // Get Header Size
    info.seekp( 0 , ios::end );
    long size = info.tellp();

    //------------------------------------------------
    // Copy info into header_info array
    strncpy( header_info , info.str().c_str() , size );

    //------------------------------------------------
    //Create Binary Resource File
    createBinaryFile( resource_header , resource_data_content , header_info );

    //clean up
    delete[] header_info;
    delete[] resource_header.data_compress;
    delete[] resource_data_content.data_compress;

    //remove temp files
    remove("_header");
    remove("_resource");
}

void ResourceManager::createResourceData( ResourceData & resource_data_content , const std::string & file_name )
{
    char * file_data   = 0;
    long   file_length = 0;

    ifstream resource_file;
    resource_file.open( file_name , ios::binary );

    //get length of file
    resource_file.seekg( 0 , std::ios::end );
    file_length = resource_file.tellg();
    resource_file.seekg( 0 , std::ios::beg );

    file_data = new char[ file_length ];

    //read data as a block
    resource_file.read( file_data , file_length );

    //close header file
    resource_file.close();

    //Resource Compress
    LZ4CompressResource( resource_data_content , file_data , file_length );

    delete[] file_data;
    file_data = 0;
}

void ResourceManager::LZ4CompressResource( ResourceData & resource , char bytes[] , int byte_size  )
{
    resource.data_compress        = new char[ byte_size ];
    resource.data_compress_length = LZ4_compress( (const char *) bytes , resource.data_compress , byte_size );
    resource.data_length          = byte_size;
}

void ResourceManager::createBinaryFile( ResourceData & header , ResourceData & content , char header_bytes[] )
{
    WriteByteDataEnd( content.file_name , header_bytes , 512 );  // first 512 bytes
    WriteByteDataEnd( content.file_name , header.data_compress  , header.data_compress_length  );
    WriteByteDataEnd( content.file_name , content.data_compress , content.data_compress_length );
}

Resource * ResourceManager::openResourceFile(const string & resource_name)
{
    Resource * new_resource = new Resource( resource_name );

    //------------------------------------------------
    // Read Header

    HeaderData header_data;
    getHeaderInfo( header_data , resource_name );

    //------------------------------------------------
    new_resource->setStartByte( 512 + header_data.header_size_compress );
    new_resource->setResourceSize( header_data.resource_size );
    new_resource->setResourceSizeCompress( header_data.resource_size_compress  );

    //------------------------------------------------
    char * header_file_compress = new char[ header_data.header_size_compress ];
    ifstream filesystem;
    filesystem.open( resource_name , ios::binary );

    if( filesystem.is_open() )
    {
        filesystem.seekg( 512 );
        filesystem.read( header_file_compress , header_data.header_size_compress );
        filesystem.close();
    }

    char * header_file_original = new char[ header_data.header_size ];
    LZ4_decompress_fast( header_file_compress , header_file_original , header_data.header_size );
    delete[] header_file_compress;

    //------------------------------------------------
    std::string line;
    ofstream temp("temp_files", ios::out | ios::binary );
    temp.write( header_file_original , header_data.header_size );
    temp.close();
    //------------------------------------------------

    delete[] header_file_original;

    ifstream tempi("temp_files",  ios::binary );
    if( tempi.is_open() )
    {
        while( std::getline(tempi , line ) )
        {
            Strings split = Engine::Utils::StringSplit( line , " ");

            stringstream file_length_stream;
            stringstream file_start_stream;
            float file_length_float = 0;
            float file_start_float  = 0;

            file_length_stream << split[1];
            file_length_stream >> file_length_float;

            file_start_stream  << split[2];
            file_start_stream  >> file_start_float;

            Vector2f float_data = Vector2f( file_length_float , file_start_float );
            new_resource->addOriginalResource( split[0] , float_data );

            /*!
             *  split[0] = ./test/resource.path
             *  split[1] = original file length
             *  split[2] = start byte
             */
        }

       temp.close();
       remove("temp_files");
    }

    return new_resource;
}


void ResourceManager::getHeaderInfo(HeaderData &data, const string &resource_name)
{

#ifdef __linux__

    std::string header_info_string;
    char * header_info = new char[ 512 ]; //first 512 bytes
    memset( header_info , '\0' , 512 );

    ifstream header;
    header.open( resource_name , ios::binary );

    if( header.is_open() )
    {
        header.read( header_info , 512 );
        header.close();
    }

    header_info_string.append( header_info );

    Strings header_info_split   = Engine::Utils::StringSplit( header_info_string , "\n" );
    Strings type_split          = Engine::Utils::StringSplit( header_info_split[0] , " ");
    Strings head_size           = Engine::Utils::StringSplit( header_info_split[1] , " ");
    Strings head_size_compress  = Engine::Utils::StringSplit( header_info_split[2] , " ");
    Strings res_size            = Engine::Utils::StringSplit( header_info_split[3] , " ");
    Strings res_size_compress   = Engine::Utils::StringSplit( header_info_split[4] , " ");

    stringstream head_size_stream;
    stringstream head_size_compress_stream;
    stringstream res_size_stream;
    stringstream res_size_compress_stream;

    long header_size_long = 0;
    long header_size_compress_long = 0;
    long res_size_long = 0;
    long res_size_compress_long = 0;

    head_size_stream << head_size[1];
    head_size_stream >> header_size_long;

    head_size_compress_stream << head_size_compress[1];
    head_size_compress_stream >> header_size_compress_long;

    res_size_stream << res_size[1];
    res_size_stream >> res_size_long;

    res_size_compress_stream << res_size_compress[1];
    res_size_compress_stream >> res_size_compress_long;

    data.type                    =  type_split[1].c_str();
    data.header_size             =  header_size_long;
    data.resource_size           =  res_size_long;
    data.header_size_compress    =  header_size_compress_long;
    data.resource_size_compress  =  res_size_compress_long;

#endif
}

Texture * ResourceManager::getTexture(const string &texture_name, Resource *resource)
{
    createFile("output_image" , texture_name  , resource );

    Texture * texture = TextureManager::getSingletonPtr()->createTexture("output_image");
    texture->setName( texture_name );

    remove("output_image");

    return texture;
}

IShader * ResourceManager::getShader(const string &shader_name, Resource * resource )
{
    // Create Shader file names .frag | .vert
    //---------------------------------------------------
    stringstream fragment_stream;
    stringstream vertex_stream;
    fragment_stream << shader_name << ".frag";
    vertex_stream   << shader_name << ".vert";

    // Create basic Shader without source
    //---------------------------------------------------
    Arch::DatabaseManager * database_manager = Arch::DatabaseManager::getSingletonPtr();
    ShaderManager   * shader_manager   = (ShaderManager   *) database_manager->getDatabase("ShaderManager");

    IShader * shader = shader_manager->createShader( shader_name );

    // has Shader already source then return shader
    //---------------------------------------------------
    if( shader->hasSource() )
            return shader;

    // Get Original Fragment Resource | Create Fragment Shader and attach it ...
    //---------------------------------------------------
    if( resource->existsResource( fragment_stream.str() ) )
    {
        char * current_resource = getOriginalResource( fragment_stream.str() , resource );

        stringstream shader_resource_stream;
        shader_resource_stream << current_resource;

        uint fragment_id = shader_manager->CreateShader( fragment_stream.str() , shader_resource_stream.str() , GL_FRAGMENT_SHADER );
        delete[] current_resource;

        shader->AttachShader( fragment_id );
    }

    // Get Original Vertex Resource | Create Vertex Shader and attach it ...
    //---------------------------------------------------
    if( resource->existsResource( vertex_stream.str() ) )
    {
        char * current_resource = getOriginalResource( fragment_stream.str() , resource );
        stringstream shader_resource_stream;
        shader_resource_stream << current_resource;

        uint vertex_id = shader_manager->CreateShader( fragment_stream.str() , shader_resource_stream.str() , GL_VERTEX_SHADER );
        delete[] current_resource;

        shader->AttachShader( vertex_id );
    }

    // finish
    //---------------------------------------------------
    shader->setSource( true );

    // Return Shader with source
    //---------------------------------------------------
    return shader;
}

char * ResourceManager::LZ4DecompressResource( Resource *resource  )
{
    char * compress_buffer_data = new char[ resource->getResourceSizeCompress() ];
    char * original_data        = new char[ resource->getResourceSize() ];

    // Get Compress Resource Data
    //---------------------------------------------------
    ifstream content;
    content.open( resource->getName() , ios::binary );
    if( content.is_open() )
    {
        content.seekg( resource->getStartByte() );
        content.read ( compress_buffer_data , resource->getResourceSizeCompress() );
        content.close();
    }

    // Decompress Resource Data - and create original resource data
    //--------------------------------------------------
    LZ4_decompress_fast( original_data ,compress_buffer_data , resource->getResourceSize() );
    delete[] compress_buffer_data;

    // Return Original Resource
    //--------------------------------------------------
    return original_data;
}

char * ResourceManager::getOriginalResource( const std::string & file_name , Resource * resource )
{
    char * original_data    = LZ4DecompressResource( resource );
    long file_length        = resource->getByteData( file_name ).getX();
    long start_byte         = resource->getByteData( file_name ).getY();
    char * current_resource = new char[ file_length ];

    // All Original Resources
    //----------------------------------------------------
    ofstream temp("temp_file", ios::out | ios::binary );
    temp.write( original_data , resource->getResourceSize() );
    temp.close();
    delete[] original_data;

    // Pick a Resource from all Resources
    //----------------------------------------------------
    ifstream tempi("temp_file",  ios::binary );
    if( tempi.is_open() )
    {
        tempi.seekg( start_byte );
        tempi.read( current_resource , file_length );
        tempi.close();
    }
    remove("temp_file");

    // Return Resource Data
    //----------------------------------------------------
    return current_resource;
}

void ResourceManager::createFile(const string &file_output , const std::string & original_file_path , Resource *resource)
{
    char * original_data    = LZ4DecompressResource( resource );
    long file_length        = resource->getByteData( original_file_path ).getX();
    long start_byte         = resource->getByteData( original_file_path ).getY();
    char * current_resource = new char[ file_length ];

    // All Original Resources
    //----------------------------------------------------
    ofstream temp("temp_file", ios::out | ios::binary );
    temp.write( original_data , resource->getResourceSize() );
    temp.close();
    delete[] original_data;

    // Pick a Resource from all Resources
    //----------------------------------------------------
    ifstream tempi("temp_file",  ios::binary );
    if( tempi.is_open() )
    {
        tempi.seekg( start_byte );
        tempi.read( current_resource , file_length );
        tempi.close();
    }
    remove("temp_file");

    // Create Original Resource File
    //------------------------------------------------------
    ofstream image( file_output , ios::out | ios::binary );
    if( image.is_open() )
    {
        image.write( current_resource , file_length );
        image.close();
    }
    delete[] current_resource;
}
