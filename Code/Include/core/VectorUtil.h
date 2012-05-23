/// @file	core\VectorUtil.h
/// @brief  standalone mathematical vector operations
#pragma once


///////////////////////////////////////////////////////////////////////////////

struct Vector;

///////////////////////////////////////////////////////////////////////////////

class VectorUtil
{
public:
   /**
    * Calculates a vector perpendicular to the specified one.
    *
    * @param inVec
    * @para outPerpVec
    */
   static void calculatePerpendicularVector( const Vector& inVec, Vector& outPerpVec );
};

///////////////////////////////////////////////////////////////////////////////
