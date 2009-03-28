#pragma once

#include "core-Renderer\BatchingStrategy.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

class AbstractGraphicalNode;
typedef AbstractGraphicalNode* AbstractGraphicalNodeP;
class Node;
class Light;
class SkyBox;

///////////////////////////////////////////////////////////////////////////////

class SceneManager
{
private:
   Node* m_rootNode;
   SkyBox* m_skyBox;

public:
   SceneManager();
   ~SceneManager();

   inline Node& getRootNode() {return *m_rootNode;}

   /**
    * In order for the rendered scene to appear more lifelike, we can
    * add a background. A skybox is a perfect choice - and this method allows to add it.
    */
   void setSkyBox(SkyBox* skyBox);
   bool isSkyBox() const {return m_skyBox != NULL;}
   SkyBox& getSkyBox() const {return *m_skyBox;}

   /**
    * This method adds a new node to the scene.
    * Depending on the implementation, different strategies
    * for doing that will be implemented
    */
   virtual void addNode(Node* node) = 0;

   /**
    * The method extracts the most influential lights (up to the given limit)
    * for the area specified by the transformation of the 'cameraNode'
    */
   virtual const std::list<Light*>& getLights(const Node& cameraNode, int lightLimit) = 0;

   /**
    * The method extracts all the nodes that can be rendered from the camera's 
    * standpoint and that don't have any transparency
    *
    * @param arraySize - upon return this parameter will contain the number of elements
    *                    in the returned array
    * @return the array contaions all the nodes that can be renderered. The array
    *         is managed by the scene manager and should not be released from the context
    *         that calls this method
    */
   virtual AbstractGraphicalNodeP* getRegularGraphicalNodes(const Node& cameraNode,
                                                     DWORD& arraySize) = 0;

   /**
    * The method extracts all the transparent nodes that can be rendered from the camera's 
    * standpoint
    *
    * @param arraySize - upon return this parameter will contain the number of elements
    *                    in the returned array
    * @return the array contaions all the nodes that can be renderered. The array
    *         is managed by the scene manager and should not be released from the context
    *         that calls this method
    */
   virtual AbstractGraphicalNodeP* getTransparentGraphicalNodes(const Node& cameraNode,
                                                     DWORD& arraySize) = 0;

};

///////////////////////////////////////////////////////////////////////////////
