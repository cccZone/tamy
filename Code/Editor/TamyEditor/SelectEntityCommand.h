#pragma once

/// @file   TamyEditor\SelectEntityCommand.h
/// @brief  command for selecting entity from a scene

#include "QueryableScene.h"
#include "core\Delegate.h"
#include "CameraCommand.h"
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class Entity;
class Camera;
struct Ray;
class PropertiesEditor;

///////////////////////////////////////////////////////////////////////////////

/**
 * Command for selecting entity from a scene.
 */
class SelectEntityCommand : public CameraCommand
{

private:
   Camera& m_camera;
   QueryableScene& m_scene;
   PropertiesEditor& m_editor;

public:
   /**
    * Constructor.
    *
    * @param camera        camera that shows the scene
    * @param scene         scene we want to query
    * @param editor        properties editor that will edit the selected 
    *                      properties
    */
   SelectEntityCommand(Camera& camera,
                       QueryableScene& scene, 
                       PropertiesEditor& editor);

   // -------------------------------------------------------------------------
   // MousePointerCommand implementation
   // -------------------------------------------------------------------------
   void execute(const D3DXVECTOR2& mousePos);

private:
   SpatiallyQueryable* findClosest(const Ray& ray,
                                   const Array<SpatiallyQueryable*>& queryables);
};

///////////////////////////////////////////////////////////////////////////////
