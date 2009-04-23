#pragma once

#include "core\AbstractSceneManager.h"
#include "core-Renderer\BatchingStrategy.h"
#include <list>
#include "core\TNodesVisitor.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\Camera.h"


///////////////////////////////////////////////////////////////////////////////

typedef AbstractGraphicalNode* AbstractGraphicalNodeP;
class Node;
class SkyBox;
class Camera;
class ActiveCameraNode;

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

   ActiveCameraNode* m_activeCameraDeploymentNode;
   Camera* m_activeCamera;

   SkyBox* m_skyBox;

public:
   VisualSceneManager();
   ~VisualSceneManager();

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
   virtual const std::list<Light*>& getLights(int lightLimit) = 0;

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
   virtual AbstractGraphicalNodeP* getRegularGraphicalNodes(DWORD& arraySize) = 0;

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
   virtual AbstractGraphicalNodeP* getTransparentGraphicalNodes(DWORD& arraySize) = 0;


protected:
   /**
    * This method is called when we're adding a light to the scene
    */
   virtual void add(Light& light) = 0;

   /**
    * This method is called when we're removing a light from the scene
    */
   virtual void remove(Light& light) = 0;

   /**
    * This method is called when we're adding a graphical node to the scene
    */
   virtual void add(AbstractGraphicalNode& node) = 0;

   /**
    * This method is called when we're removing a graphical node from the scene
    */
   virtual void remove(AbstractGraphicalNode& node) = 0;

   /**
    * This method is called when we're adding a camera to the scene
    */
   void add(Camera& node);

   /**
    * This method is called when we're removing a camera from the scene
    */
   void remove(Camera& node);

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
