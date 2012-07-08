/// @file   TamyEditor\ProjectDirNode.h
/// @brief  A node describing a directory in a project tree structure.
#pragma 

#include "ProjectTreeNode.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A node describing a directory in a project tree structure.
 */
class ProjectDirNode : public ProjectTreeNode
{
private:
   std::string          m_projectNodeName;

public:
   /**
    * Constructor.
    *
    * @param parent
    * @param nodeName
    */
   ProjectDirNode( ProjectTreeNode* parent, const std::string& nodeName );

   // -------------------------------------------------------------------------
   // ProjectTreeNode implementation
   // -------------------------------------------------------------------------
   bool compareNodeName( const std::string& name ) const;
   std::string getRelativePath() const;
   TreeWidgetDescFactory* getDescFactory( ProjectTree& tree );
   void addNode( unsigned int typeIdx, ProjectTree& tree );
   void openItem( ProjectTree& tree );

};

///////////////////////////////////////////////////////////////////////////////
