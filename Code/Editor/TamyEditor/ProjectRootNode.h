/// @file   TamyEditor\ProjectRootNode.h
/// @brief  A node describing the root of the project.
#pragma 

#include "ProjectTreeNode.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A node describing the root of the file system.
 */
class ProjectRootNode : public ProjectTreeNode
{
public:
   /**
    * Constructor.
    *
    * @param parent
    */
   ProjectRootNode( QTreeWidget* parent );

   // -------------------------------------------------------------------------
   // ProjectTreeNode implementation
   // -------------------------------------------------------------------------
   bool compareNodeName( const std::string& name ) const;
   std::string getRelativePath() const;
   TreeWidgetDescFactory* getDescFactory( ProjectTree& tree );
   void addNode( unsigned int typeIdx, ProjectTree& tree );
};

///////////////////////////////////////////////////////////////////////////////
