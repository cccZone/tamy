#pragma once

#include "core\AbstractSceneManager.h"
#include "core\Array.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\AGNVolExtractor.h"
#include "core\Octree.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

typedef AbstractGraphicalNode* AbstractGraphicalNodeP;
class Node;
class SkyBox;
class Camera;
class ActiveCameraNode;
class LinearNodesStorage;

///////////////////////////////////////////////////////////////////////////////

/**
 * This manager manages the visual aspects of the scene
 */
class VisualSceneManager: public AbstractSceneManager,
                          public SceneAspectManager<Light>, 
                          public SceneAspectManager<AbstractGraphicalNode>, 
                          public SceneAspectManager<Camera>, 
                          public SceneAspectManager<SkyBox>
{
private:
   ActiveCameraNode* m_activeCameraDeploymentNode;
   Camera* m_activeCamera;

   SkyBox* m_skyBox;

   std::list<Light*> m_allLights;
   D3DXVECTOR3 m_cachedCameraPos;
   std::list<Light*> m_currentlyVisibleLights;

   LinearNodesStorage* m_dynamicNodesContainer;
   Octree<AbstractGraphicalNodeP, AGNVolExtractor, BoundingSphere>* m_staticNodesContainer;

public:
   VisualSceneManager(unsigned int maxElemsPerSector = 64, float worldSize = 1000);
   ~VisualSceneManager();

   bool isSkyBox() const {return m_skyBox != NULL;}
   SkyBox& getSkyBox() const {return *m_skyBox;}

   /**
    * Sets a camera used to view the rendered scene. Without an active camera
    * no rendering will take place
    */
   void setActiveCamera(Camera& camera);
   bool hasActiveCamera() const {return m_activeCamera != NULL;}
   Camera& getActiveCamera() const {return *m_activeCamera;}

   /**
    * The method extracts the most influential lights (up to the given limit)
    * for the area specified by the transformation of the 'cameraNode'
    */
   const std::list<Light*>& getLights(int lightLimit);

   /**
    * The method allows to query the graphical scene for nodes that overlap
    * the volume passed in the @param volume.
    *
    * The results are returned in the array passed in the @param output
    */
   template<typename QueryVolume>
   void query(const QueryVolume& volume, Array<AbstractGraphicalNode*>& output) const;

   /**
   * The method allows to query the graphical scene for nodes that overlap
   * the volume passed in the @param volume. The query is performed in broad and narrow phase,
   * meaning that not only the bounding volumes of the scene nodes, but also
   * the underlying geometry will be tested
   *
   * The results are returned in the array passed in the @param output
   */
   template<typename QueryVolume>
   void detailedQuery(const QueryVolume& volume, Array<AbstractGraphicalNode*>& output) const;

private:
   void add(Light& light);
   void remove(Light& light);
   void refreshVisibleLights(int lightLimit);

   void add(Camera& node);
   void remove(Camera& node);

   void add(AbstractGraphicalNode& node);
   void remove(AbstractGraphicalNode& node);

   void add(SkyBox& skyBox);
   void remove(SkyBox& skyBox);
};

///////////////////////////////////////////////////////////////////////////////

#ifndef VISUAL_SCENE_MANAGER_H
#define VISUAL_SCENE_MANAGER_H

#include "core-Renderer\VisualSceneManager.inl"

#endif

///////////////////////////////////////////////////////////////////////////////
