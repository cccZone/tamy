/// @file   ml-Blender/TamyScene.h
/// @brief  Tamy scene export-related proxies

#include "ml-Blender/TamyMesh.h"
#include "core/Matrix.h"
#include "core/Vector.h"
#include "core/Color.h"
#include "core/Quaternion.h"


///////////////////////////////////////////////////////////////////////////////

class SpatialEntity;

///////////////////////////////////////////////////////////////////////////////

struct TamyMatrix
{
   Matrix               rotationMtx;
   Vector               position;

   void applyTo( Matrix& outTransform ) const;

   void applyToLight( Matrix& outTransform ) const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This structure represents a Geometry entity
 */
struct TamyGeometry
{
   const char*          name;

   TamyMesh*            meshesList;
   TamyMatrix           localMatrix;

   int                  meshesCount;
   int                  parentIdx;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This structure represents a Geometry entity
 */
struct TamyLight
{
   const char*          name;

   int                  type;                         // @see TamyLightType enum
   
   Color                lightColor;
   float                energy;
   float                distance;
   bool                 useSpecular;

   bool                 castShadows;
   Color                shadowColor;

   float                linearAttenuation;
   float                quadraticAttenuation;

   int                  falloffType;                  // @see LightFalloffType

   float                spotAttenuation;
   float                spotAngle;
   bool                 spotlightCastsOnlyShadows;
     
   TamyMatrix           localMatrix;
   int                  parentIdx;
};

///////////////////////////////////////////////////////////////////////////////

enum TamyLightType
{
   LT_Directional = 0,
   LT_Point = 1,
   LT_Spotlight = 2,
   LT_Hemisphere = 3,
   LT_Area = 4,
};

///////////////////////////////////////////////////////////////////////////////

enum LightFalloffType
{
   FOT_Constant = 0,
   FOT_InverseLinear = 1,
   FOT_InverseSquare = 2
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This structure holds global rendering settings.
 */
struct TamyWorld
{
   Color                ambientLightColor;
};

///////////////////////////////////////////////////////////////////////////////
