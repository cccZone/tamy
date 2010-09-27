#ifndef _MODEL_H
#define _MODEL_H

/// @file   core-MVC\Model.h
/// @brief  MVC model

#include <vector>
#include "core\ComponentsManager.h"
#include "core\Delegate.h"
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
class Model : public Resource, public ComponentsManager< Model >
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
    *
    * @param name    scene name
    */
   Model( const std::string& name = "" );
   ~Model();

   // -------------------------------------------------------------------------
   // Entities management
   // -------------------------------------------------------------------------

   /**
    * This method adds a new entity to the model.
    *
    * @param entity     new entity we want to add to the model
    * @param manage     should the model manage the entity
    */
   void add(Entity* entity, bool manage = true);

   /**
    * This method removes an entity from the model.
    *
    * @param entity     entity we want to remove
    */
   void remove( Entity& entity );

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
   // Serializable implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

protected:
   // -------------------------------------------------------------------------
   // ComponentsManager implementation
   // -------------------------------------------------------------------------
   void onComponentAdded( Component< Model >& component );

   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onResourceLoaded( ResourcesManager& mgr );

private:
   void notifyEntityAdded( Entity& entity );
   void notifyEntityRemoved( Entity& entity );
   void notifyEntityChanged( Entity& entity );
   void notifyComponentAdded( Entity& entity, Component< Model >& component );
   void processViewsOperations();
   void entityDFS( Entity& entity, const Functor& operation );

   // -------------------------------------------------------------------------
   // Befriended operations
   // -------------------------------------------------------------------------
   friend class ComponentAddOperation;
   friend class Entity;
};

///////////////////////////////////////////////////////////////////////////////

#include "core-MVC\Model.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _MODEL_H
