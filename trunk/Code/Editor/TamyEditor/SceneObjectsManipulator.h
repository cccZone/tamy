/// @file   TamyEditor/SceneObjectsManipulator.h
/// @brief  tool for manual manipulation of the scene objects
#pragma once

#include "SceneRendererInputController.h"
#include "core-AI/FSMController.h"
#include "SelectionManagerListener.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

enum NodeTransformControlMode;
class SpatialEntity;
class SceneEditor;

///////////////////////////////////////////////////////////////////////////////

class SceneObjectsManipulator : public SceneRendererInputController, public FSMController< SceneObjectsManipulator >, public SelectionManagerListener
{
   DECLARE_CLASS()

private:
   SceneEditor&                        m_sceneEditor;
   TamySceneWidget*                    m_widget;

   SceneRendererInputController*       m_activeController;
   std::vector< SpatialEntity* >       m_selectedNodes;

public:
   /**
    * Constructor.
    *
    * @param editor     host scene editor instance
    */
   SceneObjectsManipulator( SceneEditor& editor );
   ~SceneObjectsManipulator();

   // -------------------------------------------------------------------------
   // States API
   // -------------------------------------------------------------------------

   /**
    * States changing functionality.
    *
    * @param controller
    */
   void setController( SceneRendererInputController* controller );

   /**
    * Returns the scene display widget.
    */
   inline TamySceneWidget& getSceneWidget() const { return *m_widget; }

   /**
    * Returns the host instance of SceneEditor.
    */
   inline SceneEditor& getSceneEditor() const { return m_sceneEditor; }

   /**
    * Tells if there are currently any entities selected on the scene.
    */
   inline bool areNodesSelected() const { return !m_selectedNodes.empty(); }

   /**
    * Returns an array of currently selected nodes.
    */
   inline const std::vector< SpatialEntity* >& getSelectedNodes() const { return m_selectedNodes; }

   // -------------------------------------------------------------------------
   // SceneRendererInputController implementation
   // -------------------------------------------------------------------------
   void initialize( TamySceneWidget& widget );

   // -------------------------------------------------------------------------
   // TimeDependent implementation
   // -------------------------------------------------------------------------
   void update( float timeElapsed );

   // -------------------------------------------------------------------------
   // SelectionManagerListener implementation
   // -------------------------------------------------------------------------
   void onEntitySelected( Entity& entity );
   void onEntityDeselected( Entity& entity );

private:
   /**
    * This method deletes all currently selected entities.
    */
   void deleteSelectedEntities();
};

///////////////////////////////////////////////////////////////////////////////
