/// @file   TamyEditor/DebugEntitiesManager.h
/// @brief  manager of the debug entities
#pragma once

#include "core\GenericFactory.h"
#include "core-MVC\SpatialEntity.h"
#include "core-MVC\ModelView.h"
#include "core-AppFlow\TimeDependent.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class EditorDebugRenderer;
class DebugGeometry;

///////////////////////////////////////////////////////////////////////////////

/**
 * Manager of the debug entities.
 */
class DebugEntitiesManager : public GenericFactory< SpatialEntity, DebugGeometry >, public ModelView, public TimeDependent
{
private:
   EditorDebugRenderer&                      m_debugRenderer;

   std::vector< SpatialEntity* >             m_representedEntities;
   std::vector< DebugGeometry* >             m_representations;

public:
   /**
    * Constructor.
    */
   DebugEntitiesManager( EditorDebugRenderer& debugRenderer );
   ~DebugEntitiesManager();

   // -------------------------------------------------------------------------
   // TimeDependent implementation
   // -------------------------------------------------------------------------
   void update( float timeElapsed );

   // -------------------------------------------------------------------------
   // DebugEntitiesManager implementation
   // -------------------------------------------------------------------------
   void onEntityAdded( Entity& entity );
   void onEntityRemoved( Entity& entity );
   void onEntityChanged( Entity& entity );
   void resetContents();
};

///////////////////////////////////////////////////////////////////////////////
