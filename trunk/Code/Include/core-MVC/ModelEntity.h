/// @file   core-MVC/ModelEntity.h
/// @brief  a spatial entity you can embed a whole model in
#pragma once

#include "core-MVC/SpatialEntity.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class ModelEntity : public SpatialEntity
{
   DECLARE_CLASS( ModelEntity )

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
   void onPropertyChanged( Property& property );

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   Entity* cloneSelf() const;

private:
   void onModelChanged();
};

///////////////////////////////////////////////////////////////////////////////
