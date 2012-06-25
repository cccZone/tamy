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
    * @param fs         file system with the editor icons
    */
   FSRootNode( QTreeWidget* parent, const Filesystem& fs );

   // -------------------------------------------------------------------------
   // FSTreeNode implementation
   // -------------------------------------------------------------------------
   bool compareNodeName( const std::string& name ) const;
   std::string getRelativePath() const;
   TreeWidgetDescFactory* getDescFactory( ResourcesBrowser& resourcesFactory );
   void addNode( unsigned int typeIdx, ResourcesBrowser& resourcesFactory );
};

///////////////////////////////////////////////////////////////////////////////
