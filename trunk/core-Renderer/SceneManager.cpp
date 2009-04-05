#include "core-Renderer\SceneManager.h"
#include "core\Node.h"
#include "core-Renderer\SkyBox.h"
#include "core-Renderer\ActiveCameraNode.h"


///////////////////////////////////////////////////////////////////////////////

SceneManager::SceneManager()
      : m_addOperation(NULL),
      m_removeOperation(NULL),
      m_noOperation(new NoOperation()),
      m_currentOperation(m_noOperation),
      m_activeCameraDeploymentNode(new ActiveCameraNode()),
      m_activeCamera(NULL),
      m_rootNode(new Node("SceneRootNode")),
      m_skyBox(NULL)
{
   m_addOperation = new AddOperation(*this);
   m_removeOperation = new RemoveOperation(*this);

   m_rootNode->addChild(m_activeCameraDeploymentNode);
}

///////////////////////////////////////////////////////////////////////////////

SceneManager::~SceneManager()
{
   delete m_rootNode;
   m_rootNode = NULL;

   m_currentOperation = NULL;

   delete m_noOperation;
   m_noOperation = NULL;

   delete m_removeOperation;
   m_removeOperation = NULL;

   delete m_addOperation;
   m_addOperation = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SceneManager::addNode(Node* node)
{
   m_currentOperation = m_addOperation;
   m_currentOperation->toHierarchy(node);
   node->accept(*this);
}

///////////////////////////////////////////////////////////////////////////////

void SceneManager::removeNode(Node& node)
{
   m_currentOperation = m_removeOperation;
   m_currentOperation->toHierarchy(&node);
   node.accept(*this);
}

///////////////////////////////////////////////////////////////////////////////

void SceneManager::visit(Light& light)
{
   m_currentOperation->perform(light);
}

///////////////////////////////////////////////////////////////////////////////

void SceneManager::visit(AbstractGraphicalNode& node)
{
   m_currentOperation->perform(node);
}

///////////////////////////////////////////////////////////////////////////////

void SceneManager::visit(Camera& node)
{
   m_currentOperation->perform(node);
}

///////////////////////////////////////////////////////////////////////////////

void SceneManager::add(Camera& node)
{
   if (m_activeCamera != NULL) {return;}
   setActiveCamera(node);
}

///////////////////////////////////////////////////////////////////////////////

void SceneManager::remove(Camera& node)
{
   if (m_activeCamera != &node) {return;}
   m_activeCamera = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SceneManager::setSkyBox(SkyBox* skyBox)
{
   if (m_skyBox != NULL)
   {
      m_activeCameraDeploymentNode->removeChild(*m_skyBox);
      delete m_skyBox;
   }
   m_activeCameraDeploymentNode->addChild(skyBox);
   m_skyBox = skyBox;
}

///////////////////////////////////////////////////////////////////////////////

void SceneManager::setActiveCamera(Camera& camera)
{
   m_activeCamera = &camera;
   m_activeCameraDeploymentNode->setCameraNode(camera);
}

///////////////////////////////////////////////////////////////////////////////
