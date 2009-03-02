#include "GraphicalEntityInstantiator.h"
#include "CompositeGraphicalEntity.h"
#include "GraphicalEntity.h"
#include "SkinnedGraphicalEntity.h"


///////////////////////////////////////////////////////////////////////////////

GraphicalEntityInstantiator::GraphicalEntityInstantiator(const std::string& name)
      : Node(name),
      m_localParent(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

void GraphicalEntityInstantiator::attachEntity(AbstractGraphicalEntity& entity)
{
   // delete the previously set nodes hierarchy
   const std::list<Node*>& children = getChildren();
   while(getChildrenCount() > 0)
   {
      Node* childNode = getChildren().back();
      removeChild(*childNode);
      delete childNode;
   }

   // create a new instance of nodes based on the graphical entity template
   m_localParent = this;
   entity.accept(*this);

   for (std::list<std::pair<Node*, SkinnedGraphicalEntity*> >::iterator it = m_skinsToPostprocess.begin();
      it != m_skinsToPostprocess.end(); ++it)
   {
      Node* parentNode = it->first;
      SkinnedGraphicalEntity* skinnedEntity = it->second;
      parentNode->addChild(skinnedEntity->instantiate(*this));
   }
   m_skinsToPostprocess.clear();
   m_localParent = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GraphicalEntityInstantiator::visit(CompositeGraphicalEntity& entity)
{
   Node* prevParent = m_localParent;

   Node* compositeNode = entity.instantiate();
   m_localParent->addChild(compositeNode);
   m_localParent = compositeNode;

   for (std::list<AbstractGraphicalEntity*>::const_iterator it = entity.getChildren().begin();
        it != entity.getChildren().end(); ++it)
   {
      (*it)->accept(*this);
   }

   m_localParent = prevParent;
}

///////////////////////////////////////////////////////////////////////////////

void GraphicalEntityInstantiator::visit(GraphicalEntity& entity)
{
   m_localParent->addChild(entity.instantiate());
}

///////////////////////////////////////////////////////////////////////////////

void GraphicalEntityInstantiator::visit(SkinnedGraphicalEntity& entity)
{
   m_skinsToPostprocess.push_back(std::make_pair(m_localParent, &entity));
}

///////////////////////////////////////////////////////////////////////////////

void GraphicalEntityInstantiator::onAccept(NodeVisitor& visitor)
{
   // do nothing
}

///////////////////////////////////////////////////////////////////////////////
