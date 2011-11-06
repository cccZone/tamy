#pragma 

/// @file   TamyEditor\FSLeafNode.h
/// @brief  A node describing a file in a file system tree structure.

#include "FSTreeNode.h"
#include "TypeDescFactory.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A node describing a file in a file system tree structure.
 */
class FSLeafNode : public FSTreeNode
{
public:
   /**
    * Constructor.
    *
    * @param parent
    * @param nodeName
    * @param fs         file system from which we can access entry files
    * @param itemsFactory
    */
   FSLeafNode( FSTreeNode* parent,  const std::string& nodeName, const Filesystem& fs, TypeDescFactory< Resource >& itemsFactory );

   // -------------------------------------------------------------------------
   // FSTreeNode implementation
   // -------------------------------------------------------------------------
   std::string getRelativePath() const;
   TreeWidgetDescFactory* getDescFactory( ResourcesBrowser& resourcesFactory );
   void addNode( unsigned int typeIdx, ResourcesBrowser& resourcesFactory );
   void editResource( ResourcesBrowser& editorsFactory );

private:
   void setEntryIcon( const Filesystem& fs, TypeDescFactory< Resource >& itemsFactory );
   void setEntryName( const Filesystem& fs );
   void setEntrySize( const Filesystem& fs );
};

///////////////////////////////////////////////////////////////////////////////
