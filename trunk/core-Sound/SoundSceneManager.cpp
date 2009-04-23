#include "core-Sound\SoundSceneManager.h"
#include "core\Node.h"


///////////////////////////////////////////////////////////////////////////////

SoundSceneManager::SoundSceneManager()
{
   m_addOperation = new AddOperation(*this);
   m_removeOperation = new RemoveOperation(*this);
   m_noOperation = new NoOperation();
   m_currentOperation = m_noOperation;
}

///////////////////////////////////////////////////////////////////////////////

SoundSceneManager::~SoundSceneManager()
{
   m_currentOperation = NULL;

   delete m_noOperation;
   m_noOperation = NULL;

   delete m_removeOperation;
   m_removeOperation = NULL;

   delete m_addOperation;
   m_addOperation = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SoundSceneManager::addNode(Node* node)
{
   m_currentOperation = m_addOperation;
   node->accept(*this);
}

///////////////////////////////////////////////////////////////////////////////

void SoundSceneManager::removeNode(Node& node)
{
   m_currentOperation = m_removeOperation;
   node.accept(*this);
}

///////////////////////////////////////////////////////////////////////////////

void SoundSceneManager::visit(SoundListener& listener)
{
   m_currentOperation->perform(listener);
}

///////////////////////////////////////////////////////////////////////////////

void SoundSceneManager::visit(Sound3D& source)
{
   m_currentOperation->perform(source);
}

///////////////////////////////////////////////////////////////////////////////
