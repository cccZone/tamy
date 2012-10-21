/// @file   core\SingletonsManager.h
/// @brief  tool for managing unique instances of classes
#ifndef _SINGLETONS_MANAGER_H
#define _SINGLETONS_MANAGER_H


#include <vector>
#include "core\Manageable.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This class can manage singleton instances of various classes.
 */
class SingletonsManager
{
   DECLARE_ALLOCATOR( SingletonsManager, AM_DEFAULT );

private:
   std::vector<Manageable*> m_shared;

public:
   ~SingletonsManager();

   /**
    * This method allows to set a shared instance of a particular class
    * that can later on be queried using the @see shared method.
    *
    * @param shared     instance we want to manage
    * @return           'true' if the instance was correctly added, 
    *                   'false' otherwise
    */
   template<typename Type>
   bool setShared(Type* shared);

   /**
    * This method allows to set a shared instance of a particular class
    * that can later on be queried using the @see shared method
    *
    * When an instance is set through this method, it won't get deleted when
    * the ResourceManager instance is deleted
    */
   template<typename Type>
   bool setShared(Type& shared);

   /**
    * Removes a shared instance of the specified type.
    */
   template< typename Type >
   void removeShared();

   /**
    * This method tells whether a specific shared instance is registered..
    */
   template<typename Type>
   bool hasShared() const;

   /**
    * This method returns a shared instance of a certain class.
    *
    * @return     reference to the requested instance
    */
   template<typename Type>
   Type& shared();

private:
   bool add(Manageable* newManageable);
};

///////////////////////////////////////////////////////////////////////////////

#include "core\SingletonsManager.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _SINGLETONS_MANAGER_H
