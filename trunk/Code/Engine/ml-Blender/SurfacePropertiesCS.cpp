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

   // parse the material technique
   const char* techniqueName[] = { "lambert", "phong", NULL };

   typedef void ( SurfacePropertiesCS::*TechniqueParser )( TiXmlElement* );
   TechniqueParser techniqueParser[] = { &SurfacePropertiesCS::parseLambert, &SurfacePropertiesCS::parsePhong, NULL };

   for ( int i = 0; techniqueName[i] != NULL; ++i )
   {
      TiXmlElement* elem = technique->FirstChildElement( techniqueName[i] );
      if ( elem )
      {
         // success - parse it and get out
         (this->*techniqueParser[i])( elem );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void SurfacePropertiesCS::parseLambert( TiXmlElement* techniqueRoot )
{
   // create a material
   float power = 2;
   if ( techniqueRoot->FirstChildElement( "reflectivity" ) )
   {
      const char* valStr = techniqueRoot->FirstChildElement( "reflectivity" )->FirstChildElement( "float" )->GetText();
      sscanf_s( valStr, "%f", &power );
   }

   Color ambient( 0.7f, 0.7f, 0.7f, 1 );
   if ( techniqueRoot->FirstChildElement( "ambient" ) )
   {
      const char* valStr = techniqueRoot->FirstChildElement( "ambient" )->FirstChildElement( "color" )->GetText();
      sscanf_s( valStr, "%f %f %f %f", &ambient.r, &ambient.g, &ambient.b, &ambient.a );
   }

   Color diffuse( 0.7f, 0.7f, 0.7f, 1 );
   if ( techniqueRoot->FirstChildElement( "diffuse" ) )
   {
      const char* valStr = techniqueRoot->FirstChildElement( "diffuse" )->FirstChildElement( "color" )->GetText();
      sscanf_s( valStr, "%f %f %f %f", &diffuse.r, &diffuse.g, &diffuse.b, &diffuse.a );
   }

   Color specular( 0.9f, 0.9f, 0.9f, 1 );
   if ( techniqueRoot->FirstChildElement( "reflective" ) )
   {
      const char* valStr = techniqueRoot->FirstChildElement( "reflective" )->FirstChildElement( "color" )->GetText();
      sscanf_s( valStr, "%f %f %f %f", &specular.r, &specular.g, &specular.b, &specular.a );
   }

   Color emissive( 0, 0, 0 );
   if ( techniqueRoot->FirstChildElement( "emission" ) )
   {
      const char* valStr = techniqueRoot->FirstChildElement( "emission" )->FirstChildElement( "color" )->GetText();
      sscanf_s( valStr, "%f %f %f %f", &emissive.r, &emissive.g, &emissive.b, &emissive.a );
   }

   m_surfaceProperties = new SurfaceProperties( ambient, diffuse, specular, emissive, power );
}

///////////////////////////////////////////////////////////////////////////////

void SurfacePropertiesCS::parsePhong( TiXmlElement* techniqueRoot )
{
   // create a material
   float power = 2;
   if ( techniqueRoot->FirstChildElement( "shininess" ) )
   {
      const char* valStr = techniqueRoot->FirstChildElement( "shininess" )->FirstChildElement( "float" )->GetText();
      sscanf_s( valStr, "%f", &power );
   }

   Color ambient( 0.7f, 0.7f, 0.7f, 1 );
   if ( techniqueRoot->FirstChildElement( "ambient" ) )
   {
      const char* valStr = techniqueRoot->FirstChildElement( "ambient" )->FirstChildElement( "color" )->GetText();
      sscanf_s( valStr, "%f %f %f %f", &ambient.r, &ambient.g, &ambient.b, &ambient.a );
   }

   Color diffuse( 0.7f, 0.7f, 0.7f, 1 );
   if ( techniqueRoot->FirstChildElement( "diffuse" ) )
   {
      const char* valStr = techniqueRoot->FirstChildElement( "diffuse" )->FirstChildElement( "color" )->GetText();
      sscanf_s( valStr, "%f %f %f %f", &diffuse.r, &diffuse.g, &diffuse.b, &diffuse.a );
   }

   Color specular( 0.9f, 0.9f, 0.9f, 1 );
   if ( techniqueRoot->FirstChildElement( "specular" ) )
   {
      const char* valStr = techniqueRoot->FirstChildElement( "specular" )->FirstChildElement( "color" )->GetText();
      sscanf_s( valStr, "%f %f %f %f", &specular.r, &specular.g, &specular.b, &specular.a );
   }

   Color emissive( 0, 0, 0 );
   if ( techniqueRoot->FirstChildElement( "emission" ) )
   {
      const char* valStr = techniqueRoot->FirstChildElement( "emission" )->FirstChildElement( "color" )->GetText();
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
