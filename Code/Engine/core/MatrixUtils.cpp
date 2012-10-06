#include "core\MatrixUtils.h"
#include "core\Vector.h"
#include "core\Matrix.h"
#include "core\Quaternion.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

void MatrixUtils::lerp( const Matrix& start, const Matrix& end, const FastFloat& lerpDist, Matrix& outInterpolatedMtx )
{
   ASSERT_MSG( lerpDist <= Float_1, "The value for advancement should be <= 1");
   ASSERT_MSG( lerpDist >= Float_0, "The value for advancement should be >= 0");

   // translation

   Vector startPos( start.m[3][0], start.m[3][1], start.m[3][2] );
   Vector endPos( end.m[3][0], end.m[3][1], end.m[3][2] );
   Vector resultPos;
   resultPos.setSub( endPos, startPos ).mul( lerpDist.getFloat() ).add( startPos );

   Matrix translationMtx;
   translationMtx.setTranslation( resultPos );

   // rotation
   Quaternion startQuat, endQuat;
   start.getRotation( startQuat );
   end.getRotation( endQuat );

   Quaternion resultQuat;
   resultQuat.setSlerp( startQuat, endQuat, lerpDist );

   Matrix rotationMtx;
   rotationMtx.setRotation( resultQuat );

   // calculate the final result
   outInterpolatedMtx.setMul( rotationMtx, translationMtx );
}

///////////////////////////////////////////////////////////////////////////////

void MatrixUtils::generatePrespectiveProjection( float fov, float aspectRatio, float nearZPlane, float farZPlane, Matrix& outProjMtx )
{
   float yScale = 1.0f / tan( fov / 2.0f );
   float xScale = yScale / aspectRatio;

   memset( (float*)(outProjMtx.m), 0, sizeof( float ) * 16 );
   outProjMtx.m11 = xScale;
   outProjMtx.m22 = yScale;
   outProjMtx.m33 = farZPlane / ( farZPlane - nearZPlane );
   outProjMtx.m34 = 1.0f;
   outProjMtx.m43 = -( nearZPlane * farZPlane ) / ( farZPlane - nearZPlane );
}

///////////////////////////////////////////////////////////////////////////////

void MatrixUtils::generateOrthogonalProjection( float viewportWidth, float viewportHeight, float nearZPlane, float farZPlane, Matrix& outProjMtx )
{
   memset( (float*)(outProjMtx.m), 0, sizeof( float ) * 16 );
   outProjMtx.m11 = 2.0f / viewportWidth;
   outProjMtx.m22 = 2.0f / viewportHeight;
   outProjMtx.m33 = 1.0f / ( farZPlane - nearZPlane );
   outProjMtx.m43 = -nearZPlane / ( farZPlane - nearZPlane );
   outProjMtx.m44 = 1.0f;
}

///////////////////////////////////////////////////////////////////////////////

void MatrixUtils::generateLookAtLH( const Vector& cameraOriginPos, const Vector& lookAtPos, const Vector& upAxis, Matrix& outLookAtMtx )
{
   Vector lookVec;
   lookVec.setSub( lookAtPos, cameraOriginPos ).normalize();

   Vector sideVec;
   sideVec.setCross( upAxis, lookVec );

   outLookAtMtx.m11 = sideVec.x; outLookAtMtx.m12 = sideVec.y; outLookAtMtx.m13 = sideVec.z; outLookAtMtx.m14 = 0.0;
   outLookAtMtx.m21 = upAxis.x;  outLookAtMtx.m22 = upAxis.y;  outLookAtMtx.m23 = upAxis.z;  outLookAtMtx.m24 = 0.0;
   outLookAtMtx.m31 = lookVec.x; outLookAtMtx.m32 = lookVec.y; outLookAtMtx.m33 = lookVec.z; outLookAtMtx.m34 = 0.0;

   outLookAtMtx.m41 = cameraOriginPos.x;
   outLookAtMtx.m42 = cameraOriginPos.y;
   outLookAtMtx.m43 = cameraOriginPos.z;
   outLookAtMtx.m44 = 1.0;
}

///////////////////////////////////////////////////////////////////////////////

void MatrixUtils::generateViewportMatrix( uint offsetX, uint offsetY, uint width, uint height, Matrix& outViewportMtx )
{
   outViewportMtx = Matrix::IDENTITY;
   outViewportMtx.m11 = (float)width;
   outViewportMtx.m22 = -(float)height;
   outViewportMtx.m41 = (float)offsetX;
   outViewportMtx.m42 = (float)( height + offsetY );
}

///////////////////////////////////////////////////////////////////////////////

void MatrixUtils::calculateViewMtx( const Matrix& inMtx, Matrix& outViewMtx )
{
   Vector rightVec, upVec, lookVec, position;
   inMtx.getVectors( rightVec, upVec, lookVec, position );

   // create a view matrix
   outViewMtx = Matrix::IDENTITY;
   outViewMtx.m[0][0] = rightVec.x; outViewMtx.m[0][1] = upVec.x; outViewMtx.m[0][2] = lookVec.x; 
   outViewMtx.m[1][0] = rightVec.y; outViewMtx.m[1][1] = upVec.y; outViewMtx.m[1][2] = lookVec.y;
   outViewMtx.m[2][0] = rightVec.z; outViewMtx.m[2][1] = upVec.z; outViewMtx.m[2][2] = lookVec.z;
   outViewMtx.m[3][0] = -position.dot( rightVec );
   outViewMtx.m[3][1] = -position.dot( upVec );
   outViewMtx.m[3][2] = -position.dot( lookVec );

}

///////////////////////////////////////////////////////////////////////////////
