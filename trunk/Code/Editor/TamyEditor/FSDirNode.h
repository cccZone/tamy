#pragma 

/// @file   TamyEditor\FSDirNode.h
/// @brief  A node describing a directory in a file system tree structure.

#include "FSTreeNode.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A node describing a directory in a file system tree structure.
 */
class FSDirNode : public FSTreeNode
{
private:
   std::string          m_fsNodeName;

public:
   /**
    * Constructor.
    *
    * @param parent
    * @param nodeName
    */
   FSDirNode( FSTreeNode* parent, const std::string& nodeName );

   // -------------------------------------------------------------------------
   // FSTreeNode implementation
   // -------------------------------------------------------------------------
   bool compareNodeName( const std::string& name ) const;
   std::string getRelativePath() const;
   TreeWidgetDescFactory* getDescFactory( FilesystemTree& resourcesFactory );
   void addNode( unsigned int typeIdx, FilesystemTree& resourcesFactory );
   void openItem( FilesystemTree& hostTree );

};

///////////////////////////////////////////////////////////////////////////////
