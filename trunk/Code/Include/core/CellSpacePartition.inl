#ifndef _CELL_SPACE_PARTITION_H
#error "This file can only be included from CellSpacePartition.h"
#else

///////////////////////////////////////////////////////////////////////////////

template <class entity>
CellSpacePartition<entity>::CellSpacePartition( float width, float height, int numCellsX, int numCellsZ)
   : m_spaceWidth( width )
   , m_spaceHeight( height ),
   , m_numCellsX( numCellsX )
   , m_numCellsZ( numCellsZ )
   , m_cellSizeX( m_spaceWidth / (float)m_numCellsX )
   , m_cellSizeZ( m_spaceHeight / (float)m_numCellsZ )
{
   for (int x = 0; x < m_numCellsX; x++)
   {
      for (int z = 0; z < m_numCellsZ; z++)
      {
         m_cells.push_back(
            Cell<entity>(  Vector(x * m_cellSizeX, 0, z * m_cellSizeZ ),
                           Vector( (x + 1) * m_cellSizeX, 0, (z + 1) * m_cellSizeZ ) ) );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template <class entity>
void CellSpacePartition<entity>::addEntity( entity& ent )
{
   Vector pos;
   ent.getPosition( pos );
   int idx = posToIndex( pos );

   m_cells[idx].m_inhabitants.push_back( &ent );
}

///////////////////////////////////////////////////////////////////////////////

template <class entity>
void CellSpacePartition<entity>::calculateNeighbors( const AABoundingBox& queryAABox, std::list<entity*>& foundEntities ) const
{
   for ( std::vector< Cell< entity > >::const_iterator curCell = m_cells.begin(); curCell != m_cells.end(); ++curCell )
   {
      if ( curCell->m_bbox.isCollidingWith( queryAABox ) && ( curCell->m_inhabitants.empty() == false ) )
      {
         for (std::list<entity*>::const_iterator it = curCell->m_inhabitants.begin(); 
            it != curCell->m_inhabitants.end(); ++it)
         {
            if (queryAABox.isCollidingWith((*it)->getPosition()))
            {
               foundEntities.push_back(*it);
            }
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template <class entity>
void CellSpacePartition<entity>::calculateNeighbors( const BoundingSphere& querySphere, std::list<entity*>& foundEntities ) const
{
   D3DXVECTOR3 radVec(querySphere.radius, querySphere.radius, querySphere.radius);
   AABoundingBox queryAABox(querySphere.origin - radVec, querySphere.origin + radVec);

   for ( std::vector<Cell<entity> >::const_iterator curCell = m_cells.begin(); curCell != m_cells.end(); ++curCell )
   {
      if ( curCell->m_bbox.isCollidingWith( queryAABox ) && ( curCell->m_inhabitants.empty() == false ) )
      {
         for ( std::list<entity*>::const_iterator it = curCell->m_inhabitants.begin(); it != curCell->m_inhabitants.end(); ++it )
         {
            if (querySphere.isCollidingWith((*it)->getPosition()))
            {
               foundEntities.push_back(*it);
            }
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template <class entity>
void CellSpacePartition<entity>::updateEntity( entity& ent, const Vector& oldPos ) 
{
   //if the index for the old pos and the new pos are not equal then
   //the entity has moved to another cell.
   int oldIdx = posToIndex( oldPos );
   int newIdx = posToIndex( ent.getPosition() );

   if (newIdx == oldIdx) return;

   //the entity has moved into another cell so delete from current cell
   //and add to new one
   m_cells[oldIdx].m_inhabitants.remove( &ent );
   m_cells[newIdx].m_inhabitants.push_back( &ent );
}

///////////////////////////////////////////////////////////////////////////////

template <class entity>
int CellSpacePartition<entity>::posToIndex( const Vector& pos ) const
{
   int idx = ( int )( m_numCellsX * pos.x / m_spaceWidth ) + 
      ( ( int )( m_numCellsZ * pos.z / m_spaceHeight ) * m_numCellsX );

   // if the entity's position is equal to vector2d(m_spaceWidth, m_spaceHeight)
   // then the index will overshoot. We need to check for this and adjust
   if ( idx > (int)m_cells.size() - 1 )
   {
      idx = (int)m_cells.size() - 1;
   }

   return idx;
}

///////////////////////////////////////////////////////////////////////////////


#endif // _CELL_SPACE_PARTITION_H
