/// @file   core-MVC/ModelEntity.h
/// @brief  a spatial entity you can embed a whole model in
#pragma once

#include "core\MemoryRouter.h"
#include "core-MVC\SpatialEntity.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class ModelEntity : public SpatialEntity
{
   DECLARE_ALLOCATOR( ModelEntity, AM_ALIGNED_16 );
   DECLARE_CLASS();

private:
   // static data
   Model*                     m_model;

   // runtime data
   std::vector< Entity* >     m_embeddedEntities;

public:
   /**
    * Constructor.
    *
    * @param name
    */
   ModelEntity( const std::string& name = "" );

   /**
    * Copy constructor.
    *
    * @param rhs
    */
   ModelEntity( const ModelEntity& rhs );

   /**
    * Constructor.
    *
    * @param model         model we want to embed
    * @param name          entity name
    */
   ModelEntity( Model& model, const std::string& name = "" );
   ~ModelEntity();

protected:
   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
   void onPropertyChanged( ReflectionProperty& property );

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   Entity* cloneSelf() const;

private:
   void onModelChanged();
};

///////////////////////////////////////////////////////////////////////////////
