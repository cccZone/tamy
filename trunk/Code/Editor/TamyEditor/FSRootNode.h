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
   std::string getRelativePath() const;
   TreeWidgetDescFactory* getDescFactory( ResourcesBrowser& resourcesFactory );
   void addNode( unsigned int typeIdx, ResourcesBrowser& resourcesFactory );
};

///////////////////////////////////////////////////////////////////////////////
