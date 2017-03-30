#ifndef FRUSTUM_H
#define FRUSTUM_H

#define PI_OVER_180 0.0174532925199432957692369076849f
#define DEG_TO_RAD(x) (x * PI_OVER_180)

#include "index.h"
#include "position.h"
#include "camera.h"

namespace Engine
{
    struct FrustumPlane
    {
        double left;
        double right;
        double bottom;
        double top;
    };

    struct Frustum
    {
        FrustumPlane near_plane;
        FrustumPlane far_plane;

        double fov;
        double aspect;
        double nearDist;
        double farDist;
    };

    struct SplitFrustum4
    {
        glm::mat4 level_1;
        glm::mat4 level_2;
        glm::mat4 level_3;
        glm::mat4 level_4;
    };

    struct FrustumBox
    {
        double minX;
        double maxX;

        double minY;
        double maxY;

        double minZ;
        double maxZ;
    };


    class FrustumUtils
    {
        public:
            FrustumUtils(){}
            ~FrustumUtils()=default;

            static Vector3fv  createFrustumVertices( double nearDist , double farDist );
            static Vector3fv  getTransformVector3fv( Vector3fv base_vertices , glm::mat4 rotation , bool inverse );
            static FrustumBox getFrustumBox( Vector3fv vertices );
            static bool       isVectorInside( FrustumBox & box , Vector3f vector );

            static Vector3f   getCentroid3f( double nearDist, double farDist, glm::mat4 transformation   );
            static glm::vec3  getCentroid( double nearDist , double farDist , glm::mat4 transformation );
            static glm::mat4  getLightViewMatrix( glm::vec3 centroid , glm::vec3 lightDirection , float distFromCentroid );
            static glm::mat4  getLightViewMatrix( Vector3f centroid , Vector3f lightPosition , float distFromCentroid );
            static glm::mat4  getOrthoFrustum( Vector3fv frustum_corners , glm::mat4 light_view_matrix , float nearClipOffset );
            static glm::mat4  getOrthoFrustum( Vector3fv frustum_corners , float near , float far );

            static void getFrustum( Frustum * frustum , double fov , double aspect , double nearDist , double farDist );
            static glm::mat4 getFrustumMatrix( Frustum * frustum );
            static glm::mat4 getFrustumMatrix( FrustumPlane * plane , double nearDist , double farDist );

            static void getSplitFrustum( SplitFrustum4 * frustums );
    };
}

#endif // FRUSTUM_H
