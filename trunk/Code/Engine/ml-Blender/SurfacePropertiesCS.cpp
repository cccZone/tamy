#include "ml-Blender/SurfacePropertiesCS.h"
#include "core.h"
#include "core-Renderer.h"
#include "tinyxml/tinyxml.h"


///////////////////////////////////////////////////////////////////////////////

SurfacePropertiesCS::SurfacePropertiesCS( TiXmlNode* materialNode, const FilePath& deploymentDir, ResourcesManager& rm )
{
   if ( !materialNode )
   {
      ASSERT_MSG( false, "NULL pointer instead a valid TiXmlNode material node specified" );
      return;
   }

   TiXmlElement* materialElem = materialNode->ToElement();
   ASSERT( materialElem != NULL );

   TiXmlElement* profile = materialElem->FirstChildElement( "profile_COMMON" );
   ASSERT( profile != NULL );

   TiXmlElement* technique = profile->FirstChildElement( "technique" );
   ASSERT( technique != NULL );

   TiXmlElement* lambert = technique->FirstChildElement( "lambert" );

   // create a material
   float power = 2;
   if ( lambert && lambert->FirstChildElement( "reflectivity" ) )
   {
      const char* valStr = lambert->FirstChildElement( "reflectivity" )->FirstChildElement( "float" )->GetText();
      sscanf_s( valStr, "%f", &power );
   }

   Color ambient( 0.7f, 0.7f, 0.7f, 1 );
   if ( lambert&& lambert->FirstChildElement( "ambient" ) )
   {
      const char* valStr = lambert->FirstChildElement( "ambient" )->FirstChildElement( "color" )->GetText();
      sscanf_s( valStr, "%f %f %f %f", &ambient.r, &ambient.g, &ambient.b, &ambient.a );
   }

   Color diffuse( 0.7f, 0.7f, 0.7f, 1 );
   if ( lambert && lambert->FirstChildElement( "diffuse" ) )
   {
      const char* valStr = lambert->FirstChildElement( "diffuse" )->FirstChildElement( "color" )->GetText();
      sscanf_s( valStr, "%f %f %f %f", &diffuse.r, &diffuse.g, &diffuse.b, &diffuse.a );
   }

   Color specular( 0.9f, 0.9f, 0.9f, 1 );
   if ( lambert && lambert->FirstChildElement( "reflective" ) )
   {
      const char* valStr = lambert->FirstChildElement( "reflective" )->FirstChildElement( "color" )->GetText();
      sscanf_s( valStr, "%f %f %f %f", &specular.r, &specular.g, &specular.b, &specular.a );
   }

   Color emissive( 0, 0, 0 );
   if ( lambert && lambert->FirstChildElement( "emission" ) )
   {
      const char* valStr = lambert->FirstChildElement( "emission" )->FirstChildElement( "color" )->GetText();
      sscanf_s( valStr, "%f %f %f %f", &emissive.r, &emissive.g, &emissive.b, &emissive.a );
   }

   m_surfaceProperties = new SurfaceProperties( ambient, diffuse, specular, emissive, power );
}

///////////////////////////////////////////////////////////////////////////////

SurfacePropertiesCS::~SurfacePropertiesCS()
{
   delete m_surfaceProperties;
   m_surfaceProperties = NULL;
}

///////////////////////////////////////////////////////////////////////////////
