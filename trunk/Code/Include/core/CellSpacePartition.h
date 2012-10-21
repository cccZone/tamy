/// @file   core/CellSpacePartition.h
/// @brief  cell space partitioning utility
#ifndef _CELL_SPACE_PARTITION_H
#define _CELL_SPACE_PARTITION_H

#include <list>
#include <vector>
#include "core\BoundingVolume.h"
#include "core\MemoryRouter.h"


///////////////////////////////////////////////////////////////////////////////

struct Vector;

///////////////////////////////////////////////////////////////////////////////

/**
 * A generic cell definition.
 */
template< class entity >
struct Cell
{
   DECLARE_ALLOCATOR( Cell, AM_ALIGNED_16 );

   std::list< entity* > m_inhabitants;

   AABoundingBox m_bbox;
   Cell( const Vector& min, const Vector& max )
   { 
      m_bbox.min = min;
      m_bbox.max = max;
   }
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Cell space partitioning utility.
 */
template< class entity >
class CellSpacePartition
{
   DECLARE_ALLOCATOR( CellSpacePartition, AM_ALIGNED_16 );

private:
   std::vector< Cell< entity > >      m_cells;

   //the width and height of the world space the entities inhabit
   float  m_spaceWidth;
   float  m_spaceHeight;

   //the number of cells the space is going to be divided into
   int     m_numCellsX;
   int     m_numCellsZ;
   float   m_cellSizeX;
   float   m_cellSizeZ;

public:
   CellSpacePartition( float width, float height, int numCellsX, int numCellsY );

   inline void addEntity( entity& ent );

   // update an entity's cell by calling this from your entity's Update method
   inline void updateEntity( entity& ent, const Vector& oldPos );

   void calculateNeighbors( const AABoundingBox& queryAABox, std::list< entity* >& foundEntities ) const;
   void calculateNeighbors( const BoundingSphere& querySphere, std::list< entity* >& foundEntities ) const;

   inline void emptyCells() { m_cells.clear(); }

private:
   inline int posToIndex( const Vector& pos ) const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core/CellSpacePartition.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _CELL_SPACE_PARTITION_H

