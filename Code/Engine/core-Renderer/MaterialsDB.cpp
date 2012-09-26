#include "core-Renderer\MaterialsDB.h"
#include "core-Renderer\MaterialInstance.h"
#include "core\Assert.h"
#include "core\StringUtils.h"

// TODO: as soon as the material atlas is rendered ok, remove the singletonness from the MaterialsDB
// and make it local to a RenderingView.
// In order to do that, you'll need to make the MaterialInstance::m_index into a TRuntimeVar, initialized as the entities
// are collected from the scene. That's because each pipeline will run its own rendering view and a different
// subset of materials ( that depends on the actual scene contents ).
//
// That way we'll avoid putting the materials a scene doesn't use into the materials atlas!!!


///////////////////////////////////////////////////////////////////////////////

MaterialsDB MaterialsDB::s_theInstance;
uint MaterialsDB::RENDER_DATA_BUFFER_STRIDE = 4;
uint MaterialsDB::TEXTURE_COORDS_STRIDE = 2;

///////////////////////////////////////////////////////////////////////////////

MaterialsDB::MaterialsDB()
   : m_timestamp( 0 )
{
   // preallocate a bit of space for materials - 512 entires should be more than enough
   m_materialHashes.allocate( 512 );
}

///////////////////////////////////////////////////////////////////////////////

MaterialsDB::~MaterialsDB()
{
}

///////////////////////////////////////////////////////////////////////////////

MaterialsDB::MaterialsDB( const MaterialsDB& rhs )
{
   ASSERT_MSG( false, "Invalid operation on a singleton instance of MaterialsDB" );
}

///////////////////////////////////////////////////////////////////////////////

void MaterialsDB::operator=( const MaterialsDB& rhs )
{
   ASSERT_MSG( false, "Invalid operation on a singleton instance of MaterialsDB" );
}

///////////////////////////////////////////////////////////////////////////////

uint MaterialsDB::registerMaterial( MaterialInstance& matInstance )
{
   // check if the material hasn't been registered before
   uint newMaterialHash = calcHash( matInstance );
   uint materialsCount = m_materialHashes.size();
   for ( uint i = 0; i < materialsCount; ++i )
   {
      int materialHash = m_materialHashes[i];

      if ( materialHash == newMaterialHash )
      {
         // this material was already registered here
         return i;
      }
   }

   m_materialHashes.push_back( newMaterialHash );
   m_materials.push_back( &matInstance );

   // attach self as a listener to the material
   // We don't want to detach the observer ever - as long as it's in the DB, we want to keep observing it.
   matInstance.attachObserver( *this );

   // increase the timestamp
   ++m_timestamp;

   // the material's been appended to the end of our materials collection, so its index
   // is equal to the previous number of materials in the collection
   return materialsCount;
}

///////////////////////////////////////////////////////////////////////////////

uint MaterialsDB::calcHash( const MaterialInstance& matInstance ) const
{
   return StringUtils::calculateHash( matInstance.getFilePath().c_str() );
}

///////////////////////////////////////////////////////////////////////////////

void MaterialsDB::clear()
{
   m_materialHashes.clear();
}

///////////////////////////////////////////////////////////////////////////////

void MaterialsDB::collectMaterialData( Array< Vector >& outDataBuffer ) const
{
   uint count = m_materialHashes.size();
   outDataBuffer.allocate( count * RENDER_DATA_BUFFER_STRIDE );

   for ( uint i = 0; i < count; ++i )
   {
      const MaterialInstance* material = m_materials[i];

      const SurfaceProperties& surfaceProps = material->getSurfaceProperties();
      outDataBuffer.push_back( (const Vector&)surfaceProps.getAmbientColor() );
      outDataBuffer.push_back( (const Vector&)surfaceProps.getDiffuseColor() );
      outDataBuffer.push_back( (const Vector&)surfaceProps.getSpecularColor() );
      outDataBuffer.back().w = surfaceProps.getPower() / 255.0f;
      outDataBuffer.push_back( (const Vector&)surfaceProps.getEmissiveColor() );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialsDB::generateTextureCoordinates( Array< Vector >& outTextureCoords ) const
{
   uint count = m_materialHashes.size();
   outTextureCoords.allocate( count * TEXTURE_COORDS_STRIDE );

   // TODO: !!!!!!!!!!!!! http://www.blackpawn.com/texts/lightmaps/
}

///////////////////////////////////////////////////////////////////////////////

void MaterialsDB::update( MaterialInstance& subject )
{
   // Initial notification - skip it. The code that adds a new material to the DB
   // has already increased the timestamp
}

///////////////////////////////////////////////////////////////////////////////

void MaterialsDB::update( MaterialInstance& subject, const MaterialInstanceOperation& msg )
{
   // increase the timestamp
   ++m_timestamp;
}

///////////////////////////////////////////////////////////////////////////////
