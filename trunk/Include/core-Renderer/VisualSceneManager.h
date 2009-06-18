#pragma once

#include "core\AbstractSceneManager.h"
#include "core\Array.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\SkyBox.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

typedef AbstractGraphicalNode* AbstractGraphicalNodeP;
class Node;
class SkyBox;
class Camera;
class ActiveCameraNode;

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

private:
   void onAdd(Light& light);
   void onRemove(Light& light);
   void refreshVisibleLights(int lightLimit);

   void onAdd(Camera& node);
   void onRemove(Camera& node);

   void onAdd(AbstractGraphicalNode& node);
   void onRemove(AbstractGraphicalNode& node);

   void onAdd(SkyBox& skyBox);
   void onRemove(SkyBox& skyBox);
};

///////////////////////////////////////////////////////////////////////////////
