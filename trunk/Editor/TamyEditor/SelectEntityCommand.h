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

///////////////////////////////////////////////////////////////////////////////

/**
 * Command for selecting entity from a scene.
 */
class SelectEntityCommand : public CameraCommand
{
public:
   typedef Delegate<void (Entity&)> Output;

private:
   Camera& m_camera;
   QueryableScene& m_scene;
   Output m_selectEntity;

public:
   /**
    * Constructor.
    *
    * @param camera        camera that shows the scene
    * @param scene         scene we want to query
    * @param selectEntity  output interface that will be informed about
    *                      the selected entity
    */
   SelectEntityCommand(Camera& camera,
                       QueryableScene& scene, 
                       const Output& selectEntity);

   // -------------------------------------------------------------------------
   // MousePointerCommand implementation
   // -------------------------------------------------------------------------
   void execute(const D3DXVECTOR2& mousePos);

private:
   SpatiallyQueryable* findClosest(const Ray& ray,
                                   const Array<SpatiallyQueryable*>& queryables);
};

///////////////////////////////////////////////////////////////////////////////
