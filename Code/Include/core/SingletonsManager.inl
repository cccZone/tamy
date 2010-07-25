#ifndef _SINGLETONS_MANAGER_H
#error "This file can only be included from SingletonsManager.h"
#else

#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

template<typename Type>
bool SingletonsManager::setShared(Type* shared)
{
   if (shared == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a valid instance");
   }
   return add(new TManageable<Type> (shared));
}

///////////////////////////////////////////////////////////////////////////////

template<typename Type>
bool SingletonsManager::setShared(Type& shared)
{
   return add(new TManageable<Type> (shared));
}

///////////////////////////////////////////////////////////////////////////////

template< typename Type >
void SingletonsManager::removeShared()
{
   for ( std::vector< Manageable* >::iterator it = m_shared.begin();
         it != m_shared.end(); ++it )
   {
      TManageable< Type >* f = dynamic_cast< TManageable< Type >* >( *it );
      if (f != NULL) 
      {
         delete f;
         m_shared.erase( it );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Type>
bool SingletonsManager::hasShared() const
{
   unsigned int count = m_shared.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      TManageable<Type>* f = dynamic_cast<TManageable<Type>*> (m_shared[i]);
      if (f != NULL) {return true;}
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

template<typename Type>
Type& SingletonsManager::shared() 
{
   unsigned int count = m_shared.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      TManageable<Type>* f = dynamic_cast<TManageable<Type>*> (m_shared[i]);
      if (f != NULL) {return **f;}
   }
   throw std::out_of_range("Unknown class type");
}

///////////////////////////////////////////////////////////////////////////////

#endif // _SINGLETONS_MANAGER_H