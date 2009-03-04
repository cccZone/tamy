#pragma once

#include "TreeSegment.h"
#include "HierarchyWriter.h"
#include "MatrixWriter.h"


///////////////////////////////////////////////////////////////////////////////

class TreeStructureWriter  : public HierarchyWriter<TreeSegment>
{
protected:
   void writeSingle(std::ostream& stream, TreeSegment& node)
   {
      stream << "index : " << node.segmentIdx << 
                "; type : ";

      switch(node.type)
      {
      case BRANCH_START:   stream << "BRANCH_START";     break;
      case BRANCH_MIDDLE:  stream << "BRANCH_MIDDLE";    break;
      case BRANCH_END:     stream << "BRANCH_END";       break;
      }

      stream << "; width : " << node.width <<
                "; position: " << node.position <<
                "; direction: " << node.direction <<
                "; child count : " << node.children.size();
   }
};

///////////////////////////////////////////////////////////////////////////////
