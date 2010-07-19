#include "ml-Blender/MaterialInstantiator.h"
#include "core.h"
#include "core-Renderer.h"
#include "tinyxml/tinyxml.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

MaterialInstantiator::MaterialInstantiator( TiXmlNode* materialNode, ResourcesManager& rm )
{
   if ( !materialNode )
   {
      throw std::invalid_argument( "NULL pointer instead a valid TiXmlNode material node specified" );
   }

   TiXmlElement* materialElem = materialNode->ToElement();
   ASSERT( materialElem != NULL );

   // get the mesh name
   m_name = materialElem->Attribute( "name" );

   // create a material
   double power = 0;
   materialElem->FirstChildElement( "Hardness" )->Attribute( "val", &power );

   Color ambient;
   std::string colorStr = materialElem->FirstChildElement( "Ambient" )->Attribute( "val" );
   sscanf_s( colorStr.c_str(), "%f %f %f", &ambient.r, &ambient.g, &ambient.b );

   Color diffuse;
   colorStr = materialElem->FirstChildElement( "Diffuse" )->Attribute( "val" );
   sscanf_s( colorStr.c_str(), "%f %f %f", &diffuse.r, &diffuse.g, &diffuse.b );

   Color specular;
   colorStr = materialElem->FirstChildElement( "Specular" )->Attribute( "val" );
   sscanf_s( colorStr.c_str(), "%f %f %f", &specular.r, &specular.g, &specular.b );

   static Color emissive( 0, 0, 0 );

   m_material = new Material( ambient, diffuse, specular, emissive, (float)power );

   // add a texture
   TiXmlElement* imageElem = materialElem->FirstChildElement( "Image" );
   if ( imageElem )
   {
      std::string absolutePath = imageElem->Attribute( "path" );
      std::string relativePath = rm.getFilesystem().toRelativePath( absolutePath );

      m_texture = new Texture( relativePath );
      rm.addResource( m_texture );
   }
   else
   {
      // no texture defined
      m_texture = NULL;
   }

   // load a shader
   std::string shaderResourceName = "/Renderer/Shaders/SingleTextureEffect.tfx";
   std::string shaderFilename = "/Renderer/Shaders/SingleTextureEffect.fx";
   if ( ( m_shader = dynamic_cast< Shader* >( rm.findResource( shaderResourceName ) ) ) == NULL )
   {
      m_shader = new Shader( shaderFilename );
      rm.addResource( m_shader );
   }
}

///////////////////////////////////////////////////////////////////////////////

MaterialInstantiator::~MaterialInstantiator()
{
   delete m_material;
   m_material = NULL;

   m_texture = NULL;
}

///////////////////////////////////////////////////////////////////////////////

Entity* MaterialInstantiator::instantiate() const
{
   SingleTextureEffect* effect = new SingleTextureEffect();
   effect->setMaterial( *m_material );

   if ( m_texture )
   {
      effect->setTexture( *m_texture );
   }

   effect->initialize( *m_shader );
   return effect;
}

///////////////////////////////////////////////////////////////////////////////
