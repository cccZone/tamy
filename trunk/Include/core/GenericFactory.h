#pragma once

/// @file   core\GenericFactory.h
/// @file   abstract factory pattern


#include <map>


///////////////////////////////////////////////////////////////////////////////

/**
 * An interface of a helper class that will create specialized instances
 * of REPR_BASE (representation base interface).
 *
 * We need this interface so that we can store many creators in a single
 * map and look them up.
 */
template<class REPR_BASE>
class Creator
{
public:
   virtual ~Creator() {}

   virtual REPR_BASE* operator()(void* obj) = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Specialized creator that creates an implementation REPR_IMPL of the REPR_BASE
 * class.
 */
template<class REPR_BASE, class REPR_IMPL, class ENTITY_IMPL>
class TCreator : public Creator<REPR_BASE>
{
public:
   REPR_BASE* operator()(void* obj)
   {
      return new REPR_IMPL(*((ENTITY_IMPL*)obj));
   }
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This factory will create custom objects fom one class family 
 * as a representation of custom classes from another class family.
 */
template<class ENTITY, class REPR_BASE>
class GenericFactory
{
private:
   typedef std::map<std::string, Creator<REPR_BASE>*> CreatorsMap;
   CreatorsMap m_creators;

public:
   virtual ~GenericFactory()
   {
      for (CreatorsMap::iterator it = m_creators.begin();
         it != m_creators.end(); ++it)
      {
         delete it->second;
      }
      m_creators.clear();
   }

   /**
    * This method associates an entity with its representation.
    * From now on whenever a create method on such entity is 
    * called, this very representation will be created for it.
    *
    * The call to this method can be chained, allowing to set up
    * the whole factory in a single code block.
    *
    * @param ENTITY_IMPL   entity we want to create an implementation for
    * @param REPR_IMPL     specialized representation we'll create for
    *                      this entity
    * @return              instance of the factory - this allows to chain
    *                      the calls
    */
   template<class ENTITY_IMPL, class REPR_IMPL>
   GenericFactory& associate()
   {
      std::string typeName = typeid(ENTITY_IMPL).name();
      m_creators.insert(std::make_pair(typeName,
         new TCreator<REPR_BASE, REPR_IMPL, ENTITY_IMPL>()));

      return *this;
   }

   /**
    * The method creates a representation for an entity.
    * Such an entity needs to be registered with the factory
    * prior to this call.
    *
    * @param entity     entity for which we want to create a representation
    * @return           representation
    */ 
   REPR_BASE* create(ENTITY& entity)
   {
      std::string typeName = typeid(entity).name();
      CreatorsMap::iterator it = m_creators.find(typeName);

      if (it == m_creators.end())
      {
         return NULL;
      }
      else
      {
         REPR_BASE* rep = (*(it->second))((void*)(&entity));

         return rep;
      }
   }
};

///////////////////////////////////////////////////////////////////////////////
