/// @file   ml-Blender/TamyScene.h
/// @brief  Tamy scene export-related proxies

#include "ml-Blender/TamyMesh.h"
#include "core/Matrix.h"



///////////////////////////////////////////////////////////////////////////////

/**
 * This structure represents a Geometry entity
 */
struct TamyGeometry
{
   const char*          name;

   TamyMesh*            meshesList;
   Matrix               localMatrix;

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
   bool                 useSpecular;

   bool                 castShadows;
   Color                shadowColor;

   float                linearAttenuation;
   float                quadraticAttenuation;

   int                  falloffType;                  // @see LightFalloffType

   float                spotAttenuation;
   float                spotAngle;
   bool                 spotlightCastsOnlyShadows;
     
   Matrix               localMatrix;
   int                  parentIdx;
};

///////////////////////////////////////////////////////////////////////////////

enum TamyLightType
{
   LT_Directional = 0,
   LT_Point = 1,
   LT_Spotlight = 2,
   LT_Hemisphere = 3,
   LT_Area = 4
};

///////////////////////////////////////////////////////////////////////////////

enum LightFalloffType
{
   FOT_Constant = 0,
   FOT_InverseLinear = 1,
   FOT_InverseSquare = 2
};

///////////////////////////////////////////////////////////////////////////////
