#include "core\NodeActionsExecutor.h"
#include "core\Node.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

NodeActionsExecutor::comparator::comparator(const Node& node) 
      : m_node(&node) 
{}

///////////////////////////////////////////////////////////////////////////////

NodeActionsExecutor::comparator::result_type 
NodeActionsExecutor::comparator::operator()(argument_type actionDef)
{
   return m_node == actionDef.first;
}

///////////////////////////////////////////////////////////////////////////////

void NodeActionsExecutor::add(Node& node, const NodeActionDelegate& delegate)
{
   ActionsStorage::iterator it = std::find_if(m_actions.begin(), m_actions.end(), comparator(node));
   if (it == m_actions.end())
   {
      m_actions.push_back(std::make_pair(&node, delegate));
   }
   else
   {
      it->second = delegate;
   }
}

///////////////////////////////////////////////////////////////////////////////

void NodeActionsExecutor::execute(Node& node)
{
   std::set<NodeActionDelegate*> actions;
   findActionNodes(node, actions);
   executeActions(actions);
}

///////////////////////////////////////////////////////////////////////////////

void NodeActionsExecutor::execute(const Array<Node*>& nodes)
{
   std::set<NodeActionDelegate*> actions;

   unsigned int nodesCount = nodes.size();
   for (unsigned int i = 0; i < nodesCount; ++i)
   {
      findActionNodes(*(nodes[i]), actions);
   }

   executeActions(actions);
}

///////////////////////////////////////////////////////////////////////////////

void NodeActionsExecutor::findActionNodes(Node& inNodeToAnalyze, 
                                          std::set<NodeActionDelegate*>& outActions)
{
   ActionsStorage::iterator it;
   Node* analyzedNode = &inNodeToAnalyze;

   bool foundActionNode = false;
   while ((analyzedNode != NULL) || (foundActionNode == false))
   {
      it = std::find_if(m_actions.begin(), m_actions.end(), 
                        comparator(*analyzedNode));
      if (it != m_actions.end())
      {
         outActions.insert(&(it->second));
         foundActionNode = true;
      }

      if (analyzedNode->hasParent())
      {
         analyzedNode = &(analyzedNode->getParent());
      }
      else
      {
         analyzedNode = NULL;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void NodeActionsExecutor::executeActions(const std::set<NodeActionDelegate*>& actions)
{
   for (std::set<NodeActionDelegate*>::const_iterator it = actions.begin();
        it != actions.end(); ++it)
   {
      (*(*it))();
   }
}

///////////////////////////////////////////////////////////////////////////////
