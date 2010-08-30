#include "ml-Blender/MaterialCS.h"
#include "core.h"
#include "core-Renderer.h"
#include "tinyxml/tinyxml.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

MaterialCS::MaterialCS( TiXmlNode* materialNode, ResourcesManager& rm )
{
   if ( !materialNode )
   {
      throw std::invalid_argument( "NULL pointer instead a valid TiXmlNode material node specified" );
   }

   TiXmlElement* materialElem = materialNode->ToElement();
   ASSERT( materialElem != NULL );

   TiXmlElement* profile = materialElem->FirstChildElement( "profile_COMMON" );
   ASSERT( profile != NULL );

   TiXmlElement* technique = profile->FirstChildElement( "technique" );
   ASSERT( technique != NULL );

   TiXmlElement* lambert = technique->FirstChildElement( "lambert" );
   ASSERT( lambert != NULL );

   // create a material
   float power = 0;
   if ( lambert->FirstChildElement( "reflectivity" ) )
   {
      const char* valStr = lambert->FirstChildElement( "reflectivity" )->FirstChildElement( "float" )->GetText();
      sscanf_s( valStr, "%f", &power );
   }

   Color ambient;
   if ( lambert->FirstChildElement( "ambient" ) )
   {
      const char* valStr = lambert->FirstChildElement( "ambient" )->FirstChildElement( "color" )->GetText();
      sscanf_s( valStr, "%f %f %f %f", &ambient.r, &ambient.g, &ambient.b, &ambient.a );
   }

   Color diffuse;
   if ( lambert->FirstChildElement( "diffuse" ) )
   {
      const char* valStr = lambert->FirstChildElement( "diffuse" )->FirstChildElement( "color" )->GetText();
      sscanf_s( valStr, "%f %f %f %f", &diffuse.r, &diffuse.g, &diffuse.b, &diffuse.a );
   }

   Color specular;
   if ( lambert->FirstChildElement( "reflective" ) )
   {
      const char* valStr = lambert->FirstChildElement( "reflective" )->FirstChildElement( "color" )->GetText();
      sscanf_s( valStr, "%f %f %f %f", &specular.r, &specular.g, &specular.b, &specular.a );
   }

   Color emissive( 0, 0, 0 );
   if ( lambert->FirstChildElement( "emission" ) )
   {
      const char* valStr = lambert->FirstChildElement( "emission" )->FirstChildElement( "color" )->GetText();
      sscanf_s( valStr, "%f %f %f %f", &emissive.r, &emissive.g, &emissive.b, &emissive.a );
   }

   m_material = new Material( ambient, diffuse, specular, emissive, power );
}

///////////////////////////////////////////////////////////////////////////////

MaterialCS::~MaterialCS()
{
   delete m_material;
   m_material = NULL;
}

///////////////////////////////////////////////////////////////////////////////
