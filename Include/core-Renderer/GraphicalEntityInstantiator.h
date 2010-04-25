#pragma once

/// @file      core-Renderer\GraphicalEntityInstantiator.h
/// @brief     class that creates instances of complex graphical entities

#include "core\Node.h"


///////////////////////////////////////////////////////////////////////////////

class AbstractGraphicalEntity;
class CompositeGraphicalEntity;
class GraphicalEntity;
class SkinnedGraphicalEntity;

///////////////////////////////////////////////////////////////////////////////

/**
 * This type of node allows to bring graphical entities into the scene.
 */
class GraphicalEntityInstantiator : public Node
{
private:
   Node* m_localParent;
   std::list<std::pair<Node*, SkinnedGraphicalEntity*> > m_skinsToPostprocess;

public:
   GraphicalEntityInstantiator(const std::string& name);

   void attachEntity(AbstractGraphicalEntity& entity);

protected:
   void onAccept(NodeVisitor& visitor);

private:
   friend class CompositeGraphicalEntity;
   friend class GraphicalEntity;
   friend class SkinnedGraphicalEntity;

   void visit(CompositeGraphicalEntity& entity);
   void visit(GraphicalEntity& entity);
   void visit(SkinnedGraphicalEntity& entity);
};

///////////////////////////////////////////////////////////////////////////////
