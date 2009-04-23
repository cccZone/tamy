#include "core-Renderer\VisualSceneManager.h"
#include "core\Node.h"
#include "core-Renderer\SkyBox.h"
#include "core-Renderer\ActiveCameraNode.h"


///////////////////////////////////////////////////////////////////////////////

VisualSceneManager::VisualSceneManager()
      : m_addOperation(NULL),
      m_removeOperation(NULL),
      m_noOperation(new NoOperation()),
      m_currentOperation(m_noOperation),
      m_activeCameraDeploymentNode(new ActiveCameraNode()),
      m_activeCamera(NULL),
      m_skyBox(NULL)
{
   m_addOperation = new AddOperation(*this);
   m_removeOperation = new RemoveOperation(*this);
}

///////////////////////////////////////////////////////////////////////////////

VisualSceneManager::~VisualSceneManager()
{
   delete m_activeCameraDeploymentNode;
   m_activeCameraDeploymentNode = NULL;

   m_currentOperation = NULL;

   delete m_noOperation;
   m_noOperation = NULL;

   delete m_removeOperation;
   m_removeOperation = NULL;

   delete m_addOperation;
   m_addOperation = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::addNode(Node* node)
{
   m_currentOperation = m_addOperation;
   node->accept(*this);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::removeNode(Node& node)
{
   m_currentOperation = m_removeOperation;
   node.accept(*this);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::visit(Light& light)
{
   m_currentOperation->perform(light);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::visit(AbstractGraphicalNode& node)
{
   m_currentOperation->perform(node);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::visit(Camera& node)
{
   m_currentOperation->perform(node);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::visit(SkyBox& node)
{
   m_currentOperation->perform(node);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::add(Camera& node)
{
   if (m_activeCamera != NULL) {return;}
   setActiveCamera(node);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::remove(Camera& node)
{
   if (m_activeCamera != &node) {return;}
   m_activeCamera = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::setSkyBox(SkyBox* skyBox)
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

void VisualSceneManager::setActiveCamera(Camera& camera)
{
   m_activeCamera = &camera;
   m_activeCameraDeploymentNode->setCameraNode(camera);
}

///////////////////////////////////////////////////////////////////////////////
