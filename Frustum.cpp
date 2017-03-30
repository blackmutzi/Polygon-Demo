#include "frustum.h"

using namespace Engine;

void FrustumUtils::getSplitFrustum( SplitFrustum4 * frustums  )
{
    Frustum frustum_level_1;
    Frustum frustum_level_2;
    Frustum frustum_level_3;

    getFrustum( &frustum_level_1  , 45.0f , 16/9 , 1.0f , 100.0f );
    getFrustum( &frustum_level_2  , 45.0f , 16/9 , 1.0f , 100.0f + 300.0f );
    getFrustum( &frustum_level_3  , 45.0f , 16/9 , 1.0f , 100.0f + 600.0f );

    frustums->level_1 = getFrustumMatrix( &frustum_level_1 );
    frustums->level_2 = getFrustumMatrix( &frustum_level_1.far_plane , 1.0f , 300.0f );
    frustums->level_3 = getFrustumMatrix( &frustum_level_2.far_plane , 1.0f , 300.0f );
    frustums->level_4 = getFrustumMatrix( &frustum_level_3.far_plane , 1.0f , 300.0f );
}

Vector3f FrustumUtils::getCentroid3f(double nearDist, double farDist, glm::mat4 transformation)
{
    glm::vec3 centroid = getCentroid( nearDist , farDist , transformation );
    return Vector3f( centroid.x , centroid.y , centroid.z );
}

glm::vec3 FrustumUtils::getCentroid( double nearDist , double farDist , glm::mat4 transformation )
{
    double distance      = (farDist - nearDist) / 2;
    glm::vec4 direction  = glm::vec4( 0 , 0 , nearDist - distance , 1.0 );
              direction  = transformation * direction;
    return glm::vec3( direction.x , direction.y , direction.z );
}

glm::mat4 FrustumUtils::getLightViewMatrix( Vector3f centroid , Vector3f lightPosition , float distFromCentroid )
{
    Vector3f light_direction = lightPosition - centroid;
    light_direction.normalize();

    glm::vec3 _centroid = glm::vec3( centroid.getX() , centroid.getY() , centroid.getZ() );
    glm::vec3 _lightDir = glm::vec3( light_direction.getX() , light_direction.getY() , light_direction.getZ() );

    return getLightViewMatrix( _centroid , _lightDir , distFromCentroid );
}

glm::mat4 FrustumUtils::getLightViewMatrix( glm::vec3 centroid  , glm::vec3 lightDirection , float distFromCentroid )
{
    glm::vec3 position = glm::vec3(0,0,0);
    position  = centroid + ( lightDirection * distFromCentroid );
    return glm::lookAt( position , centroid , glm::vec3( 0 , 1 , 0 ) );
}


glm::mat4 FrustumUtils::getOrthoFrustum( Vector3fv frustum_corners , float _near , float _far )
{
    FrustumBox ortho_box = getFrustumBox( frustum_corners );
    return glm::ortho<float>( ortho_box.minX , ortho_box.maxX , ortho_box.minY , ortho_box.maxY , _near , _far );
}

glm::mat4 FrustumUtils::getOrthoFrustum( Vector3fv frustum_corners , glm::mat4 light_view_matrix , float nearClipOffset )
{
    Vector3fv view_vertices = getTransformVector3fv( frustum_corners , light_view_matrix , true );
    FrustumBox ortho_box    = getFrustumBox( view_vertices );
    return glm::ortho<float>( ortho_box.minX , ortho_box.maxX , ortho_box.minY , ortho_box.maxY , -ortho_box.maxZ - nearClipOffset , -ortho_box.minZ );
}


bool  FrustumUtils::isVectorInside( FrustumBox & box , Vector3f vector )
{
    if( vector.getX() >= box.minX && vector.getX() <= box.maxX )
        if( vector.getY() >= box.minY && vector.getY() <= box.maxY )
            if( vector.getZ() >= box.minZ && vector.getZ() <= box.maxZ )
                        return true;

    return false;
}

FrustumBox FrustumUtils::getFrustumBox( Vector3fv vertices )
{
    FrustumBox box;
    Vector3f first_vector = vertices[0];

    box.maxX = first_vector.getX();
    box.minX = first_vector.getX();
    box.maxY = first_vector.getY();
    box.minY = first_vector.getY();
    box.maxZ = first_vector.getZ();
    box.minZ = first_vector.getZ();

    for( Vector3f vector : vertices )
    {
        if( box.maxX < vector.getX() )
                box.maxX = vector.getX();

        if( box.minX > vector.getX() )
                box.minX = vector.getX();

        if( box.maxY < vector.getY() )
                box.maxY = vector.getY();

        if( box.minY > vector.getY() )
                box.minY = vector.getY();

        if( box.maxZ < vector.getZ() )
                box.maxZ = vector.getZ();

        if( box.minZ > vector.getZ() )
                box.minZ = vector.getZ();
    }

    return box;
}


Vector3fv FrustumUtils::getTransformVector3fv( Vector3fv base_vertices , glm::mat4 rotation , bool inverse )
{
    Vector3fv result_vertices;

    for( Vector3f base_vertex : base_vertices )
    {
        glm::vec4 vector = glm::vec4( base_vertex.getX() , base_vertex.getY() , base_vertex.getZ() , 1.0f );
        glm::vec4 result = rotation * vector;

        if(inverse)
        {
            result_vertices.push_back( Vector3f( -result.x , -result.y , -result.z ) );
        }else{
            result_vertices.push_back( Vector3f( result.x , result.y , result.z ) );
        }
    }

    return result_vertices;
}

Vector3fv FrustumUtils::createFrustumVertices( double nearDist , double farDist  )
{
    Vector3fv vertices;

    Frustum frustum;
    getFrustum( &frustum , 45.0f , 16/9 , nearDist , farDist );

    glm::vec3 position     = glm::vec3(0,0,0);  //P
    glm::vec3 view_vector  = glm::vec3(0,0,-1);  //v
    glm::vec3 up_vector    = glm::vec3(0,1,0);  //up
    glm::vec3 right_vector = glm::vec3(1,0,0);  //w

    float HNear            = frustum.near_plane.top;
    float WNear            = frustum.near_plane.right;
    float HFar             = frustum.far_plane.top;
    float WFar             = frustum.far_plane.right;

    glm::vec3 CNear       = position + view_vector * (float) frustum.nearDist;
    glm::vec3 Cfar        = position + view_vector * (float) frustum.farDist;

    //Near Vertices
    glm::vec3 n_bottom_left  = CNear - (up_vector * (HNear/2)) - ( right_vector * (WNear/2) );
    glm::vec3 n_bottom_right = CNear - (up_vector * (HNear/2)) + ( right_vector * (WNear/2) );
    glm::vec3 n_top_left     = CNear + (up_vector * (HNear/2)) - ( right_vector * (WNear/2) );
    glm::vec3 n_top_right    = CNear + (up_vector * (HNear/2)) + ( right_vector * (WNear/2) );

    //Far Vertices
    glm::vec3 f_bottom_left  = Cfar - (up_vector * (HFar/2)) - ( right_vector * (WFar/2) );
    glm::vec3 f_bottom_right = Cfar - (up_vector * (HFar/2)) + ( right_vector * (WFar/2) );
    glm::vec3 f_top_left     = Cfar + (up_vector * (HFar/2)) - ( right_vector * (WFar/2) );
    glm::vec3 f_top_right    = Cfar + (up_vector * (HFar/2)) + ( right_vector * (WFar/2) );


    vertices.push_back( Vector3f( n_bottom_left.x  , n_bottom_left.y  , n_bottom_left.z  ));
    vertices.push_back( Vector3f( n_bottom_right.x , n_bottom_right.y , n_bottom_right.z ));
    vertices.push_back( Vector3f( n_top_left.x     , n_top_left.y     , n_top_left.z     ));
    vertices.push_back( Vector3f( n_top_right.x    , n_top_right.y    , n_top_right.z    ));

    vertices.push_back( Vector3f( f_bottom_left.x  , f_bottom_left.y  , f_bottom_left.z  ));
    vertices.push_back( Vector3f( f_bottom_right.x , f_bottom_right.y , f_bottom_right.z ));
    vertices.push_back( Vector3f( f_top_left.x     , f_top_left.y     , f_top_left.z     ));
    vertices.push_back( Vector3f( f_top_right.x    , f_top_right.y    , f_top_right.z    ));

    return vertices;
}

void FrustumUtils::getFrustum( Frustum * frustum , double fov, double aspect, double nearDist, double farDist)
{
    //---------------------------------------------------
    // http://cgvr.informatik.uni-bremen.de/teaching/cg_literatur/lighthouse3d_view_frustum_culling/index.html
    //
    //---------------------------------------------------
    fov = DEG_TO_RAD( fov );

    double height_near  = 2 * tanf(fov / 2.0f) * nearDist;
    double width_near   = height_near * aspect;

    double height_far   = 2 * tanf( fov / 2.0f ) * farDist;
    double width_far    = height_far * aspect;

    frustum->fov      = fov;
    frustum->aspect   = aspect;
    frustum->nearDist = nearDist;
    frustum->farDist  = farDist;

    frustum->near_plane.left   = -width_near;
    frustum->near_plane.right  = width_near;
    frustum->near_plane.bottom = -height_near;
    frustum->near_plane.top    = height_near;

    frustum->far_plane.left    = -width_far;
    frustum->far_plane.right   = width_far;
    frustum->far_plane.bottom  = -height_far;
    frustum->far_plane.top     = height_far;
}

glm::mat4 FrustumUtils::getFrustumMatrix( Frustum * frustum )
{
    return glm::frustum( frustum->near_plane.left , frustum->near_plane.right , frustum->near_plane.bottom , frustum->near_plane.top , frustum->nearDist , frustum->farDist );
}

glm::mat4 FrustumUtils::getFrustumMatrix( FrustumPlane * plane , double nearDist , double farDist )
{
    return glm::frustum( plane->left , plane->right , plane->bottom , plane->top , nearDist , farDist );
}
