/// @file   TamyEditor\ProjectLeafNode.h
/// @brief  A project tree leaf node.
#pragma once

#include "ProjectTreeNode.h"


///////////////////////////////////////////////////////////////////////////////

class TreeWidgetDescFactory;

///////////////////////////////////////////////////////////////////////////////

/**
 * A project tree leaf node.
 */
class ProjectLeafNode : public ProjectTreeNode
{
protected:
   std::string          m_projectNodeName;

public:
   /**
    * Constructor.
    *
    * @param parent
    * @param nodeName
    * @param itemsFactory
    */
   ProjectLeafNode( ProjectTreeNode* parent,  const std::string& nodeName, TreeWidgetDescFactory& itemsFactory );

   // -------------------------------------------------------------------------
   // ProjectTreeNode implementation
   // -------------------------------------------------------------------------
   bool compareNodeName( const std::string& name ) const;
   std::string getRelativePath() const;
   TreeWidgetDescFactory* getDescFactory( ProjectTree& tree );
   void addNode( unsigned int typeIdx, ProjectTree& tree );
   void openItem( ProjectTree& tree );

private:
   void setEntryIcon( const Filesystem& fs, TreeWidgetDescFactory& itemsFactory );
   void setEntryName( const Filesystem& fs );
   void setEntrySize( const Filesystem& fs );
};

///////////////////////////////////////////////////////////////////////////////
