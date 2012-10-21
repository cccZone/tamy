#include "core-Renderer\MaterialsDB.h"
#include "core-Renderer\MaterialInstance.h"
#include "core\Assert.h"
#include "core\StringUtils.h"

// NOTE:
// I was supposed to undo MaterialsDB as a singleton, turning it into a RenderingView component
// ( same as an octree), that would manage only the materials used by a specific renderer.
//
// But that would require an introduction of:
//  - An observation mechanism to MaterialEntity, which contain the managed MaterialInstances.
//    If an entity is assigned another material instance, it should inform databases it's observed by.
//    This is required, because when we're shifting from a singleton in favor of multiple db instances,
//    we can no longer assume that when a material instance is loaded, it should immediately
//    inform the one existing DB instance about it.
//  - a MaterialInstance would have to store its index in a TRuntimeVar. That's not such a bad thing,
//    as there are no contraindications for this ( no dependencies on it being accessible from 
//    a context other than a runtime context ), but it's another change we would need to introduce.
// 
// All remaining things that need to be implemented would still have to be implemented nonetheless
// ( such as multiple light response textures atlases generation etc ).
//
// Another thing is - what do we actually gain from having multiple DB instances around.
// The only thing is a smaller amount of materials that have to be rendered for a view.
// But the material descriptors texture and other artifacts a DB creates are rendered only
// on-demand ( not every frame ), so it's not a big deal at this point.
// Moreover, I don't expect a running game can be running with more than 512 materials at any 
// given time, and that's what I'm optimizing here for.
// So what I actually need is a way to unregister material only and make it effectively
// at runtime - get rid of materials that are no longer used.
// This will be handled by the streaming system, so if the DB works well, it should solve all 
// materials count related issues.

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
      outDataBuffer.back()[3] = surfaceProps.getPower() / 255.0f;
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
