#pragma once

/// @file   core-Scene\Model.h
/// @brief  MVC model

#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Entity;
class ModelSerializer;
class ModelView;

///////////////////////////////////////////////////////////////////////////////

/**
 * Model stores entities and allows to serialize them. Model
 * can be observed and 'illustrated' using views.
 */
class Model
{
private:
   typedef std::vector<Entity*> Entities;
   Entities m_entities;

   typedef std::vector<ModelView*> Views;
   Views m_views;

public:
   ~Model();

   // -------------------------------------------------------------------------
   // Entities management
   // -------------------------------------------------------------------------

   /**
    * This method adds a new entity to the model.
    *
    * @param entity     new entity we want to add to the model
    */
   void add(Entity* entity);

   /**
    * This method removes an entity from the model.
    *
    * @param entity     entity we want to remove
    * @param release    this parameter specifies whether the entity
    *                   should be deleted when it's removed.
    */
   void remove(Entity& entity, bool release = true);

   /**
    * The method removes all entities from the model.
    */
   void clear();

   /**
    * This method allows to save model's state.
    *
    * @param modelSerializer  serializer that will take care of writing
    *                         stuff to an external storage
    */
   void save(ModelSerializer& modelSerializer);

   /**
    * This method loads a model from an external storage. 
    * Current model's state WILL NOT BE ERASED.
    *
    * @param modelSerializer  serializer that will take care of reading
    *                         stuff from an external storage
    */
   void load(ModelSerializer& modelSerializer);

   // -------------------------------------------------------------------------
   // Housekeeping
   // -------------------------------------------------------------------------

   /**
    * This housekeeping method informs about the number of entities
    * currently living in the model.
    *
    * @return  number of entities in this model
    */
   unsigned int getEntitiesCount() const;

   /**
    * This method allows to access a particular entity instance.
    *
    * @param idx  index of an entity we want to access.
    * @return     reference to the specified entity
    */
   Entity& getEntity(unsigned int idx);

   // -------------------------------------------------------------------------
   // Views management
   // -------------------------------------------------------------------------
   /**
    * Attaches a new view to the model.
    *
    * @param view    new model observer
    */
   void attach(ModelView& view);

   /**
    * Detaches a view from the model.
    *
    * @param view    observer we want to detach
    */
   void detach(ModelView& view);

   /**
    * The method returns a number of views that currently are
    * observing this model instance.
    *
    * @return        active views count
    */
   unsigned int getViewsCount() const;

private:
   void notifyEntityAdded(Entity& entity);
   void notifyEntityRemoved(Entity& entity);
};

///////////////////////////////////////////////////////////////////////////////
