#ifndef _MODEL_H
#define _MODEL_H

/// @file   core-MVC\Model.h
/// @brief  MVC model

#include <vector>
#include "core\Delegate.h"
#include "core\ComponentsManager.h"
#include "core\Resource.h"


///////////////////////////////////////////////////////////////////////////////

class Entity;
class ModelSerializer;
class ModelView;
class Saver;
class Loader;

///////////////////////////////////////////////////////////////////////////////

/**
 * Model stores entities and allows to serialize them. Model
 * can be observed and 'illustrated' using views.
 */
class Model : public ComponentsManager<Model>, public Resource
{
   DECLARE_RESOURCE( Model )

private:
   typedef Delegate<void (Entity&)> Functor;

   typedef std::vector<Entity*> Entities;
   Entities m_entities;

   typedef std::vector<ModelView*> Views;
   Views m_views;

   Views m_viewsToAdd;
   unsigned int m_viewsToRemoveCount;

public:
   /**
    * Constructor.
    */
   Model();
   ~Model();

   // -------------------------------------------------------------------------
   // Entities management
   // -------------------------------------------------------------------------

   /**
    * This method adds a new entity to the model.
    *
    * @param entity     new entity we want to add to the model
    * @param manage        should the model manage the entity
    */
   void add(Entity* entity, bool manage = true);

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

   // -------------------------------------------------------------------------
   // Components management
   // -------------------------------------------------------------------------

   /**
    * This helper method allows to quickly retrieve a component of the specified
    * type, if one was added to the model.
    *
    * @param COMPONENT_TYPE   type of the component we wish to retrive
    */
   template<typename COMPONENT_TYPE>
   COMPONENT_TYPE* getComponent();

private:
   void notifyEntityAdded(Entity& entity);
   void notifyEntityRemoved(Entity& entity);
   void processViewsOperations();
   void entityDFS(Entity& entity, const Functor& operation);
};

///////////////////////////////////////////////////////////////////////////////

#include "core-MVC\Model.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _MODEL_H
