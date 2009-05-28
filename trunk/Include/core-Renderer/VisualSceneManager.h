#pragma once

#include "core\AbstractSceneManager.h"
#include "core-Renderer\BatchingStrategy.h"
#include <list>
#include "core\Array.h"
#include "core\TNodesVisitor.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\Camera.h"
#include "core\DistanceComparator.h"
#include <set>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

typedef AbstractGraphicalNode* AbstractGraphicalNodeP;
class Node;
class SkyBox;
class Camera;
class ActiveCameraNode;
class Culler;

///////////////////////////////////////////////////////////////////////////////

class VisualSceneManager: public AbstractSceneManager, 
                          public TNodesVisitor<Light>,
                          public TNodesVisitor<AbstractGraphicalNode>,
                          public TNodesVisitor<Camera>,
                          public TNodesVisitor<SkyBox>
{
private:
   class Operation
   {
   public:
      virtual ~Operation() {}
      
      virtual void perform(Light& light) = 0;
      virtual void perform(AbstractGraphicalNode& node) = 0;
      virtual void perform(Camera& node) = 0;
      virtual void perform(SkyBox& node) = 0;
   };

private:
   Operation* m_addOperation;
   Operation* m_removeOperation;
   Operation* m_noOperation;
   Operation* m_currentOperation;

   Culler* m_culler;

   ActiveCameraNode* m_activeCameraDeploymentNode;
   Camera* m_activeCamera;

   SkyBox* m_skyBox;

   std::list<Light*> m_allLights;
   D3DXVECTOR3 m_cachedCameraPos;
   std::list<Light*> m_currentlyVisibleLights;

   //typedef std::multiset<AbstractGraphicalNode*, BatchComparator> GraphicalNodesSet;
   //GraphicalNodesSet m_regularGraphicalNodes;
   Array<AbstractGraphicalNodeP> m_regularGraphicalNodes;
   Array<AbstractGraphicalNodeP> m_regularRenderingQueue;
   bool m_regularNodesDirty;

   Array<AbstractGraphicalNodeP> m_transparentNodes;
   Array<AbstractGraphicalNodeP> m_transparentRenderingQueue;


   BatchComparator m_materialsComparator;
   DistanceComparator<AbstractGraphicalNode> m_distanceComparator;

public:
   VisualSceneManager();
   ~VisualSceneManager();

   /**
    * The method allows to switch from the default culler to a custom one
    */
   void setCuller(Culler* culler);

   /**
    * In order for the rendered scene to appear more lifelike, we can
    * add a background. A skybox is a perfect choice - and this method allows to add it.
    */
   void setSkyBox(SkyBox* skyBox);
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
    * This method adds a new node to the scene.
    */
   void addNode(Node* node);

   /**
    * This method removes an existing node from the scene
    */
   void removeNode(Node& node);

   void visit(Light& light);
   void visit(AbstractGraphicalNode& node);
   void visit(Camera& node);
   void visit(SkyBox& node);

   /**
    * The method extracts the most influential lights (up to the given limit)
    * for the area specified by the transformation of the 'cameraNode'
    */
   const std::list<Light*>& getLights(int lightLimit);

   /**
    * The method extracts all the nodes that can be rendered from the camera's 
    * standpoint and that don't have any transparency
    *
    * @param arraySize - upon return this parameter will contain the number of elements
    *                    in the returned array
    * @return the array contains all the nodes that can be renderered. The array
    *         is managed by the scene manager and should not be released from the context
    *         that calls this method
    */
   AbstractGraphicalNodeP* getRegularGraphicalNodes(DWORD& arraySize);

   /**
    * The method extracts all the transparent nodes that can be rendered from the camera's 
    * standpoint
    *
    * @param arraySize - upon return this parameter will contain the number of elements
    *                    in the returned array
    * @return the array contains all the nodes that can be renderered. The array
    *         is managed by the scene manager and should not be released from the context
    *         that calls this method
    */
   AbstractGraphicalNodeP* getTransparentGraphicalNodes(DWORD& arraySize);


private:
   void add(Light& light);
   void remove(Light& light);
   void refreshVisibleLights(int lightLimit);

   void add(Camera& node);
   void remove(Camera& node);

   void add(AbstractGraphicalNode& node);
   void remove(AbstractGraphicalNode& node);

   void addRegularNode(AbstractGraphicalNode& node);
   void removeRegularNode(AbstractGraphicalNode& node);

   void addTransparentNode(AbstractGraphicalNode& node);
   void removeTransparentNode(AbstractGraphicalNode& node);

private:
   // ---------------------------

   class AddOperation : public Operation
   {
   private:
      VisualSceneManager& m_controller;

   public:
      AddOperation(VisualSceneManager& controller) : m_controller(controller) {}

      void perform(Light& light) {m_controller.add(light);}
      void perform(AbstractGraphicalNode& node) {m_controller.add(node);}
      void perform(Camera& node) {m_controller.add(node);}
      void perform(SkyBox& node) {m_controller.setSkyBox(&node);}
   };
   friend class AddOperation;

   // ---------------------------

   class RemoveOperation : public Operation
   {
   private:
      VisualSceneManager& m_controller;

   public:
      RemoveOperation(VisualSceneManager& controller) : m_controller(controller) {}

      void perform(Light& light) {m_controller.remove(light);}
      void perform(AbstractGraphicalNode& node) {m_controller.remove(node);}
      void perform(Camera& node) {m_controller.remove(node);}
      void perform(SkyBox& node) {}
   };
   friend class RemoveOperation;

   // ---------------------------

   class NoOperation : public Operation
   {
   public:
      NoOperation() {}
      
      void perform(Light& light) {}
      void perform(AbstractGraphicalNode& node) {}
      void perform(Camera& node) {}
      void perform(SkyBox& node) {}
   };
   friend class NoOperation;

   // ---------------------------

};

///////////////////////////////////////////////////////////////////////////////
