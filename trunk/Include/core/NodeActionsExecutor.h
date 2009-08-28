#pragma once

/// @file   core\NodeActionsExecutor.h
/// @brief  facility for associating actions with scene nodes


#include <vector>
#include <set>
#include <functional>
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class Node;
class NodeAction;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class allows to bind actions to nodes (and node trees) and execute them.
 */
class NodeActionsExecutor
{
private:
   typedef std::pair<Node*, NodeAction*> ActionDef;
   typedef std::vector<ActionDef> ActionsStorage;
   ActionsStorage m_actions;

public:
   ~NodeActionsExecutor();

   void add(Node& node, NodeAction* action);

   /**
    * This method gives you an ability to execute an action
    * for a single node or the first of its parents
    * that has an action defined
    */
   void execute(Node& node);

   /**
    * This method does exactly the same as the one that accepts a single node,
    * with a small difference that it allows to do the same for multiple nodes at once.
    *
    * If some of these nodes share the hierarchy (end up having the same action executed),
    * that common action will be executed only once
    */
   void execute(const Array<Node*>& nodes);

private:
   void findActionNodes(Node& inNodeToAnalyze, 
                        std::set<NodeAction*>& outActions);

   void executeActions(const std::set<NodeAction*>& actions);

private:
   class comparator : public std::unary_function<ActionDef, bool>
   {
   private:
      const Node* m_node;

   public:
      comparator(const Node& node);
      result_type operator()(argument_type actionDef);
   };
};

///////////////////////////////////////////////////////////////////////////////
