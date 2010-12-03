#include "GraphLayout.h"
#include "core.h"
#include "GraphBlock.h"
#include <algorithm>



///////////////////////////////////////////////////////////////////////////////

GraphLayout::GraphLayout( const std::string& path )
{
}

///////////////////////////////////////////////////////////////////////////////

GraphLayout::~GraphLayout()
{
   // the blocks are managed by an outside resource
   m_blocks.clear();
}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::add( GraphBlock* block )
{
   ASSERT_MSG( block != NULL, "NULL blocks can't be added to the layout" );

   // check if the block isn't already present in the layout
   ASSERT_MSG( std::find( m_blocks.begin(), m_blocks.end(), block ) == m_blocks.end(), "The block already exists in the layout" );
   
   m_blocks.push_back( block );

   // add the block to the scene
   addItem( block );
}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::remove( GraphBlock* block )
{
   ASSERT_MSG( block != NULL, "NULL blocks can't be removed from the layout" );

   std::vector< GraphBlock* >::iterator it = std::find( m_blocks.begin(), m_blocks.end(), block );
   if ( it != m_blocks.end() )
   {
      m_blocks.erase( it );
      removeItem( block );
   }
   else
   {
      ASSERT_MSG( false, "The specified block doesn't belong to this layout" );
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::saveState() 
{
   for ( std::vector< GraphBlock* >::const_iterator it = m_blocks.begin(); it != m_blocks.end(); ++it )
   {
      ASSERT_MSG( *it != NULL, "Invalid block found in the layout" );

      (*it)->saveState();
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphLayout::restoreState() 
{
   for ( std::vector< GraphBlock* >::const_iterator it = m_blocks.begin(); it != m_blocks.end(); ++it )
   {
      ASSERT_MSG( *it != NULL, "Invalid block found in the layout" );

      (*it)->restoreState();
      addItem( *it );
   }
}

///////////////////////////////////////////////////////////////////////////////
