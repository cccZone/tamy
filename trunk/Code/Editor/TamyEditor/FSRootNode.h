#pragma 

/// @file   TamyEditor\FSRootNode.h
/// @brief  A node describing the root of the file system.

#include "FSTreeNode.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A node describing the root of the file system.
 */
class FSRootNode : public FSTreeNode
{
private:
   std::string          m_fsNodeName;

public:
   /**
    * Constructor.
    *
    * @param parent
    */
   FSRootNode( QTreeWidget* parent );

   // -------------------------------------------------------------------------
   // FSTreeNode implementation
   // -------------------------------------------------------------------------
   bool compareNodeName( const std::string& name ) const;
   std::string getRelativePath() const;
   TreeWidgetDescFactory* getDescFactory( FilesystemTree& resourcesFactory );
   void addNode( unsigned int typeIdx, FilesystemTree& resourcesFactory );
};

///////////////////////////////////////////////////////////////////////////////
