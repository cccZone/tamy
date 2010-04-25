#pragma once

#include <list>
#include <vector>
#include "BoundingVolume.h"


template <class entity>
struct Cell
{
  std::list<entity*> m_inhabitants;

  AABoundingBox m_bbox;
  Cell(const D3DXVECTOR3& min,
       const D3DXVECTOR3& max)
  { 
      m_bbox.min = min;
      m_bbox.max = max;
  }
};

template <class entity>
class CellSpacePartition
{
private:
   std::vector<Cell<entity> >      m_cells;

   //the width and height of the world space the entities inhabit
   float  m_spaceWidth;
   float  m_spaceHeight;

   //the number of cells the space is going to be divided into
   int     m_numCellsX;
   int     m_numCellsZ;
   float   m_cellSizeX;
   float   m_cellSizeZ;

public:
   CellSpacePartition(float width, float height, int numCellsX, int numCellsY);

   inline void addEntity(entity& ent);

   // update an entity's cell by calling this from your entity's Update method
   inline void updateEntity(entity& ent, const D3DXVECTOR3& oldPos);

   void calculateNeighbors(const AABoundingBox& queryAABox, std::list<entity*>& foundEntities) const;
   void calculateNeighbors(const BoundingSphere& querySphere, std::list<entity*>& foundEntities) const;

   inline void emptyCells() {m_cells.clear();}

private:
   inline int posToIndex(const D3DXVECTOR3& pos) const;
};


///////////////////////////////////////////////////////////////////////////////

template <class entity>
CellSpacePartition<entity>::CellSpacePartition(float width,
                                               float height,
                                               int   numCellsX,
                                               int   numCellsZ)
      : m_spaceWidth(width),
      m_spaceHeight(height),
      m_numCellsX(numCellsX),
      m_numCellsZ(numCellsZ),
      m_cellSizeX(m_spaceWidth / (float)m_numCellsX),
      m_cellSizeZ(m_spaceHeight / (float)m_numCellsZ)
{
   for (int x = 0; x < m_numCellsX; x++)
   {
      for (int z = 0; z < m_numCellsZ; z++)
      {
         m_cells.push_back(
            Cell<entity>(D3DXVECTOR3(x * m_cellSizeX,       0, z * m_cellSizeZ      ),
                         D3DXVECTOR3((x + 1) * m_cellSizeX, 0, (z + 1) * m_cellSizeZ))
                         );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template <class entity>
void CellSpacePartition<entity>::addEntity(entity& ent)
{
   D3DXVECTOR3 pos = ent.getPosition();
   int idx = posToIndex(pos);

   m_cells[idx].m_inhabitants.push_back(&ent);
}

///////////////////////////////////////////////////////////////////////////////

template <class entity>
void CellSpacePartition<entity>::calculateNeighbors(const AABoundingBox& queryAABox,
                                                    std::list<entity*>& foundEntities) const
{
   for (std::vector<Cell<entity> >::const_iterator curCell = m_cells.begin(); 
         curCell != m_cells.end(); ++curCell)
   {
      if (curCell->m_bbox.isCollidingWith(queryAABox) &&
         (curCell->m_inhabitants.empty() == false))
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
void CellSpacePartition<entity>::calculateNeighbors(const BoundingSphere& querySphere,
                                                    std::list<entity*>& foundEntities) const
{
   D3DXVECTOR3 radVec(querySphere.radius, querySphere.radius, querySphere.radius);
   AABoundingBox queryAABox(querySphere.origin - radVec, querySphere.origin + radVec);

   for (std::vector<Cell<entity> >::const_iterator curCell = m_cells.begin(); 
         curCell != m_cells.end(); ++curCell)
   {
      if (curCell->m_bbox.isCollidingWith(queryAABox) &&
         (curCell->m_inhabitants.empty() == false))
      {
         for (std::list<entity*>::const_iterator it = curCell->m_inhabitants.begin(); 
               it != curCell->m_inhabitants.end(); ++it)
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
void CellSpacePartition<entity>::updateEntity(entity& ent, const D3DXVECTOR3& oldPos) 
{
   //if the index for the old pos and the new pos are not equal then
   //the entity has moved to another cell.
   int oldIdx = posToIndex(oldPos);
   int newIdx = posToIndex(ent.getPosition());

   if (newIdx == oldIdx) return;

   //the entity has moved into another cell so delete from current cell
   //and add to new one
   m_cells[oldIdx].m_inhabitants.remove(&ent);
   m_cells[newIdx].m_inhabitants.push_back(&ent);
}

///////////////////////////////////////////////////////////////////////////////

template <class entity>
int CellSpacePartition<entity>::posToIndex(const D3DXVECTOR3& pos) const
{
   int idx = (int)(m_numCellsX * pos.x / m_spaceWidth) + 
      ((int)((m_numCellsZ) * pos.z / m_spaceHeight) * m_numCellsX);

   // if the entity's position is equal to vector2d(m_spaceWidth, m_spaceHeight)
   // then the index will overshoot. We need to check for this and adjust
   if (idx > (int)m_cells.size() - 1) idx = (int)m_cells.size() - 1;

   return idx;
}

///////////////////////////////////////////////////////////////////////////////
