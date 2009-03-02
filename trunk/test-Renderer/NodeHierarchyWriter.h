#pragma once

#include "Node.h"
#include "MatrixWriter.h"
#include <sstream>
#include <typeinfo>
#include <list>
#include <deque>
#include "GraphicalNode.h"
#include "SkinnedGraphicalNode.h"
#include "GraphicalEntity.h"
#include "SkinnedGraphicalEntity.h"


///////////////////////////////////////////////////////////////////////////////

class NodeHierarchyWriter
{
public:
   std::string operator()(Node& node)
   {
      std::list<Node*> hierarchy = linearizeHierarchy(node);

      std::stringstream result;

      for (std::list<Node*>::iterator it = hierarchy.begin(); 
           it != hierarchy.end(); ++it)
      {
         writeSingle(result, **it);
         result << std::endl;
      }

      return result.str();
   }

private:
   std::list<Node*> linearizeHierarchy(Node& node)
   {
      std::list<Node*> result;
      std::deque<Node*> nodesQueue;
      nodesQueue.push_back(&node);

      while(nodesQueue.size() > 0)
      {
         Node* currNode = nodesQueue.back();
         nodesQueue.pop_back();

         result.push_back(currNode);

         for (std::list<Node*>::const_iterator it = currNode->getChildren().begin();
              it != currNode->getChildren().end(); ++it)
         {
            nodesQueue.push_back(*it);
         }
      }

      return result;
   }

   void writeSingle(std::ostream& stream, Node& node)
   {
      stream << "parent name : " << (node.hasParent() ? node.getParent().getName() : "NULL") << 
                "; type : " << typeid(node).name() << 
                "; name : " << node.getName() <<
                "; children count : " << node.getChildrenCount() <<
                "; local mtx : " << node.getLocalMtx();

      GraphicalNode* gn = dynamic_cast<GraphicalNode*> (&node);
      if (gn != NULL)
      {
         stream << "; entity name : " << gn->getEntity().getName() << 
                   "; subset : " << gn->getSubsetIdx();
      }

      SkinnedGraphicalNode* sgn = dynamic_cast<SkinnedGraphicalNode*> (&node);
      if (sgn != NULL)
      {
         stream << "; entity name : " << sgn->getEntity().getName() << 
                   "; subset : " << sgn->getSubsetIdx() << "; bones: [";

         const SkinnedGraphicalNode::BonesDef& bd = sgn->getBonesDefinition();
         for (SkinnedGraphicalNode::BonesDef::const_iterator it = bd.begin();
              it != bd.end(); ++it)
         {
            Node* boneNode = it->first;
            stream << boneNode->getName() << ", ";
         }
         stream << "]";
      }
   }
};

///////////////////////////////////////////////////////////////////////////////
