#include "core-Renderer\MaterialsDB.h"
#include "core-Renderer\MaterialInstance.h"
#include "core\Assert.h"
#include "core\StringUtils.h"


///////////////////////////////////////////////////////////////////////////////

MaterialsDB MaterialsDB::s_theInstance;

///////////////////////////////////////////////////////////////////////////////

MaterialsDB::MaterialsDB()
{
   // preallocate a bit of space for materials - 512 entires should be more than enough
   m_materialHashes.allocate( 512 );
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

uint MaterialsDB::registerMaterial( const MaterialInstance& matInstance )
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
