#pragma once

#include "core\Node.h"
#include "core\MatrixWriter.h"
#include "core\HierarchyWriter.h"
#include <typeinfo>
#include "core-Renderer\GraphicalNode.h"
#include "core-Renderer\SkinnedGraphicalNode.h"
#include "core-Renderer\GraphicalEntity.h"
#include "core-Renderer\SkinnedGraphicalEntity.h"


///////////////////////////////////////////////////////////////////////////////

class NodeHierarchyWriter : public HierarchyWriter<Node>
{
protected:
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
