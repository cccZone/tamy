#ifndef _LINEAR_STORAGE_H
#define _LINEAR_STORAGE_H

/// @file   core\LinearStorage.h
/// @brief  spatial storage that keeps its elements in an array

#include "core\SpatialStorage.h"
#include "core\Array.h"
#include "core\CollisionTests.h"
#include "core\BoundingVolume.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This is the simplest spatial storage - one that does not have
 * any spatial hierarchy - it simply uses a linear array as an underlying
 * storage.
 * Use when you're dealing with a small set of objects, in which case
 * using any other spatial container wouild be an overkill.
 */
template<typename T>
class LinearStorage : public SpatialStorage<T>
{
private:
   Array<T*>* m_container;

public:
   LinearStorage();

   ~LinearStorage();

   /**
    * Adds a new element to the storage.
    *
    * @param elem    new element
    */
   void insert(T& elem);

   /**
    * Removes an element from the storage.
    *
    * @param elem    element to be removed
    */
   void remove(T& elem);

   /**
    * Returns a list of elements that collide with the specified
    * bounding volume.
    *
    * @param boundingVol   volume that contains the elements we want
    * @param output        upon method return this array will contain
    *                      the elements we found
    */
   void query(const BoundingVolume& boundingVol, Array<T*>& output) const;

   /**
    * Returns the number of elements in the storage;
    */
   unsigned int size() const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core\LinearStorage.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _LINEAR_STORAGE_H
