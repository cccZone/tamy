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

enum DebugFeature
{
   BoundingBoxes,
   DebugShapes,

   MAX_DEBUG_FEATURES
};

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

   bool*                                     m_debugFeatureEnabled;

public:
   /**
    * Constructor.
    */
   DebugEntitiesManager( EditorDebugRenderer& debugRenderer );
   ~DebugEntitiesManager();

   /**
    * Returns the state of the specified debug feature.
    */
   bool getDebugDisplayState( DebugFeature flag ) const { return m_debugFeatureEnabled[flag]; }

   /**
    * Toggles the display of a particular type of debug entities.
    *
    * @return     new state of the feature
    */
   bool toggleDebugDisplay( DebugFeature flag );

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

private:
   /**
    * Applies the selected debug feature ( toggling it on or off, depending on its state )
    * to the selected debug representation.
    *
    * @param representation
    * @param feature
    */
   void applyFeature( DebugGeometry* representation, DebugFeature feature );
};

///////////////////////////////////////////////////////////////////////////////
