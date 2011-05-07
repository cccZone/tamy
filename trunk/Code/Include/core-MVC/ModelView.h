#pragma once

/// @file   core-MVC\ModelView.h
/// @brief  view that observes changes of a model

#include <set>


///////////////////////////////////////////////////////////////////////////////

class Entity;
class Model;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class is a model observer.
 */
class ModelView 
{
private:
   std::set< Model* >   m_models;

public:
   virtual ~ModelView();

   /**
    * Deletes the view instance.
    */
   void release();

   /**
    * This method is called when a new entity is added to the observed model.
    *
    * @param entity  entity that's been added to the model
    */
   virtual void onEntityAdded( Entity& entity ) = 0;

   /**
    * This method is called just before an entity is removed from the observed 
    * model.
    *
    * @param entity  entity that is about to be removed the model
    */
   virtual void onEntityRemoved( Entity& entity ) = 0;

   /**
    * This method is called to inform that some aspect of an entity has changed.
    *
    * @param entity
    */
   virtual void onEntityChanged( Entity& entity ) = 0;

   /**
    * The method will should clear view contents.
    */
   virtual void resetContents() {}

   // -------------------------------------------------------------------------
   // Model-View link management
   // -------------------------------------------------------------------------
   /**
    * A Model instance calls this method to inform the view about the link
    * they now create.
    *
    * @param model   instance of Model the view is now observing
    */
   void onAttachedToModel( Model& model );

   /**
    * A Model instance calls this method to inform the view that the link
    * they created was broken and they are no longer connected.
    *
    * @param model   instance of Model the view used to be observing.
    */
   void onDetachedFromModel( Model& model );

   /**
    * The method can be used to check if the view is actively observing 
    * any model.
    *
    * @return     'true' if it's attached to a model, 'false' otherwise
    */
   bool isAttachedToModel() const;

protected:
   /**
    * Constructor.
    */
   ModelView();
};

///////////////////////////////////////////////////////////////////////////////
