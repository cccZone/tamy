#pragma once

#include <vector>
#include "core-ResourceManagement\Managable.h"
#include <typeinfo>
#include <stdexcept>



///////////////////////////////////////////////////////////////////////////////

/**
 * This class can manage singleton instances of various classes
 */
class SingletonsManager
{
private:
   std::vector<Managable*> m_shared;

public:
   ~SingletonsManager();

   /**
    * This method allows to set a shared instance of a particular class
    * that can later on be queried using the @see shared method
    */
   template<typename Type>
   bool setShared(Type* shared)
   {
      if (shared == NULL)
      {
         throw std::invalid_argument("NULL pointer instead a valid instance");
      }
      
      // we can store only the instances of unique classes
      unsigned int count = m_shared.size();
      std::string typeName = typeid(Type).name();
      std::string tmpTypeName;
      for (unsigned int i = 0; i < count; ++i)
      {
         tmpTypeName = typeid(*(m_shared[i])).name();
         if (typeName == tmpTypeName)
         {
            delete shared;
            return false;
         }
      }

      // object of this type is not yet registered - we can safely add it to our collections
      m_shared.push_back(new TManagable<Type> (shared));
      return true;
   }

   /**
    * This method allows to set a shared instance of a particular class
    * that can later on be queried using the @see shared method
    *
    * When an instance is set through this method, it won't get deleted when
    * the ResourceManager instance is deleted
    */
   template<typename Type>
   bool setShared(Type& shared)
   {
      // we can store only the instances of unique classes
      unsigned int count = m_shared.size();
      std::string typeName = typeid(Type).name();
      std::string tmpTypeName;
      for (unsigned int i = 0; i < count; ++i)
      {
         tmpTypeName = typeid(*(m_shared[i])).name();
         if (typeName == tmpTypeName)
         {
            return false;
         }
      }

      // object of this type is not yet registered - we can safely add it to our collections
      m_shared.push_back(new TManagable<Type> (shared));
      return true;
   }

   /**
    * This method returns a shared instance to a certain class.
    *
    * @throw out_of_range if the instance of this class wasn't registered
    */
   template<typename Type>
   Type& shared() 
   {
      unsigned int count = m_shared.size();
      for (unsigned int i = 0; i < count; ++i)
      {
         TManagable<Type>* f = dynamic_cast<TManagable<Type>*> (m_shared[i]);
         if (f != NULL) {return **f;}
      }
      throw std::out_of_range("Unknown class type");
   }
};

///////////////////////////////////////////////////////////////////////////////
