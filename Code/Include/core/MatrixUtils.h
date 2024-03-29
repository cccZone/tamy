/// @file   core/MatrixUtils.h
#pragma once

#include "core\Assert.h"
#include "core\types.h"


///////////////////////////////////////////////////////////////////////////////

struct Matrix;
struct Vector;
struct FastFloat;

///////////////////////////////////////////////////////////////////////////////

class MatrixUtils
{
public:
   /**
    * Performs a linear interpolation between two matrices.
    *
    * @param start
    * @param end
    * @param lerpDist
    * @param outInterpolatedMtx
    */
   static void lerp( const Matrix& start, const Matrix& end, const FastFloat& lerpDist, Matrix& outInterpolatedMtx );

   /**
    * Generates an perspective projection matrix.
    *
    * @param fov              specified in RADIANS
    * @param aspectRatio
    * @param nearZPlane
    * @param farZPlane
    * @param outProjMtx
    */
   static void generatePrespectiveProjection( float fov, float aspectRatio, float nearZPlane, float farZPlane, Matrix& outProjMtx );

   /**
    * Generates an orthogonal projection matrix.
    *
    * @param viewportWidth    if you want a viewport stretching from < -1, 1 > for instance, put 2 here ( it's the size, | -1 - 1 | == 2 )
    * @param viewportHeight   if you want a viewport stretching from < -1, 1 > for instance, put 2 here ( it's the size, | -1 - 1 | == 2 )
    * @param nearZPlane
    * @param farZPlane
    * @param outProjMtx
    */
   static void generateOrthogonalProjection( float viewportWidth, float viewportHeight, float nearZPlane, float farZPlane, Matrix& outProjMtx );

   /**
    * Generates a camera look-at matrix for a left-handed coordinate system.
    *
    * @param cameraOriginPos
    * @param lookAtPos
    * @param upAxis
    * @param outLookAtMtx
    */
   static void generateLookAtLH( const Vector& cameraOriginPos, const Vector& lookAtPos, const Vector& upAxis, Matrix& outLookAtMtx );

   /**
    * Generates a viewport matrix used to transform a projected position to a viewport position.
    *
    * @param offsetX       viewport left corner offset ( in pixels )
    * @param offsetY       viewport top corner offset ( in pixels )
    * @param width         viewport width
    * @param height         viewport height
    * @param outViewportMtx
    */
   static void generateViewportMatrix( uint offsetX, uint offsetY, uint width, uint height, Matrix& outViewportMtx );

   /**
    * Calculates a matrix that transforms all objects to the view space of the specified input matrix.
    *
    * @param inMtx
    * @param outViewMtx
    */
   static void calculateViewMtx( const Matrix& inMtx, Matrix& outViewMtx );
};

///////////////////////////////////////////////////////////////////////////////
