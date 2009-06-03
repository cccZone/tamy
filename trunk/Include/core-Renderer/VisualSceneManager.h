#pragma once

#include "core\AbstractSceneManager.h"
#include "core-Renderer\BatchingStrategy.h"
#include "core\Array.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\Camera.h"
#include "core\DistanceComparator.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

typedef AbstractGraphicalNode* AbstractGraphicalNodeP;
class Node;
class SkyBox;
class Camera;
class ActiveCameraNode;
class Culler;
class SpatialContainer;

///////////////////////////////////////////////////////////////////////////////

class VisualSceneManager: public AbstractSceneManager,
                          public SceneAspectManager<Light>, 
                          public SceneAspectManager<AbstractGraphicalNode>, 
                          public SceneAspectManager<Camera>, 
                          public SceneAspectManager<SkyBox>
{
private:
   Culler* m_culler;

   ActiveCameraNode* m_activeCameraDeploymentNode;
   Camera* m_activeCamera;

   SkyBox* m_skyBox;

   std::list<Light*> m_allLights;
   D3DXVECTOR3 m_cachedCameraPos;
   std::list<Light*> m_currentlyVisibleLights;

   SpatialContainer* m_nodesContainer;
   Array<AbstractGraphicalNodeP> m_potentiallyVisibleNodes;
   Array<AbstractGraphicalNodeP> m_visibleNodes;
   Array<AbstractGraphicalNodeP> m_nodesForSorting;

   BatchComparator m_materialsComparator;
   DistanceComparator<AbstractGraphicalNode> m_distanceComparator;

public:
   VisualSceneManager(SpatialContainer* nodesContainer = NULL);
   ~VisualSceneManager();

   /**
    * The method allows to switch from the default culler to a custom one
    */
   void setCuller(Culler* culler);

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
    * The method extracts all the nodes that can be rendered from the camera's 
    * standpoint
    *
    * @param arraySize - upon return this parameter will contain the number of elements
    *                    in the returned array
    * @return the array contains all the nodes that can be rendered. The array
    *         is managed by the scene manager and should not be released from the context
    *         that calls this method
    */
   AbstractGraphicalNodeP* getNodes(DWORD& arraySize);



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
