#include "core-Renderer\MNMaterialIndex.h"
#include "core-Renderer\MaterialSockets.h"
#include "core-Renderer\MaterialInstance.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MNMaterialIndex );
   PARENT( MaterialNode );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MNMaterialIndex::MNMaterialIndex()
   : m_index( NULL )
{
   bool isBeingDeserialized = SerializationFlag::getInstance().isSerializationInProgress();
   if ( !isBeingDeserialized )
   {
      m_index = new MSIntOutput( "index" );
      defineOutput( m_index );
   }
}

///////////////////////////////////////////////////////////////////////////////

MNMaterialIndex::~MNMaterialIndex()
{
   m_index = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MNMaterialIndex::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_index = static_cast< MSIntOutput* >( findOutput( "index" ) );
}

///////////////////////////////////////////////////////////////////////////////

void MNMaterialIndex::preRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const
{
   int materialInstanceIdx = instance.getIndex();
   m_index->setValue( instance.data(), materialInstanceIdx );
}

///////////////////////////////////////////////////////////////////////////////
