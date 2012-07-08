/// @file   TamyEditor\FSLeafNode.h
/// @brief  A filesystem tree leaf node.
#pragma once

#include "FSTreeNode.h"


///////////////////////////////////////////////////////////////////////////////

class TreeWidgetDescFactory;

///////////////////////////////////////////////////////////////////////////////

/**
 * A filesystem tree leaf node.
 */
class FSLeafNode : public FSTreeNode
{
protected:
   std::string          m_fsNodeName;

public:
   /**
    * Constructor.
    *
    * @param parent
    * @param nodeName
    * @param itemsFactory
    */
   FSLeafNode( FSTreeNode* parent,  const std::string& nodeName, TreeWidgetDescFactory& itemsFactory );

   // -------------------------------------------------------------------------
   // FSTreeNode implementation
   // -------------------------------------------------------------------------
   bool compareNodeName( const std::string& name ) const;
   std::string getRelativePath() const;
   TreeWidgetDescFactory* getDescFactory( FilesystemTree& resourcesFactory );
   void addNode( unsigned int typeIdx, FilesystemTree& resourcesFactory );
   void openItem( FilesystemTree& hostTree );

private:
   void setEntryIcon( const Filesystem& fs, TreeWidgetDescFactory& itemsFactory );
   void setEntryName( const Filesystem& fs );
   void setEntrySize( const Filesystem& fs );
};

///////////////////////////////////////////////////////////////////////////////
