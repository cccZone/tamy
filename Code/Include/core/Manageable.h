/// @file   core\Managable.h
/// @brief  instance that can be managed by the SingletonsManager
#pragma once

#include "core\MemoryRouter.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A common interface for all managables that gives possibility to store them
 * in collections.
 */
class Manageable
{
public:
   virtual ~Manageable() {}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A manageable of a specific type.
 */
template<typename T>
class TManageable : public Manageable
{
   DECLARE_ALLOCATOR( TManageable, AM_DEFAULT );

private:
   T* m_object;
   bool m_destroy;

public:
   TManageable(T* object) : m_object(object), m_destroy(true) {}
   TManageable(T& object) : m_object(&object), m_destroy(false) {}

   ~TManageable() 
   {
      if (m_destroy)
      {
         delete m_object; 
      }
      m_object = NULL;
   }

   operator T*()
   {
      return m_object;
   }

   T& operator*()
   {
      return *m_object;
   }

   const T& operator*() const
   {
      return *m_object;
   }
};

///////////////////////////////////////////////////////////////////////////////
