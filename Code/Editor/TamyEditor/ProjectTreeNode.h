/// @file   TamyEditor\ProjectTreeNode.h
/// @brief  Resources browser's project tree widget node
#pragma once


#include <QTreeWidgetItem>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class ProjectTree;
class Filesystem;
class TreeWidgetDescFactory;

///////////////////////////////////////////////////////////////////////////////

/**
 * An item describing a project node in the tree structure
 */
class ProjectTreeNode : public QTreeWidgetItem 
{
public:
   virtual ~ProjectTreeNode() {}

   /**
    * Searches for a child entry with the specified name.
    *
    * @param nodeName
    */
   ProjectTreeNode* find( const std::string& nodeName );

   /**
    * Opens the item in its own specific way.
    *
    * @param hostTree    host tree
    */
   virtual void openItem( ProjectTree& hostTree ) {}

   /**
    * Returns a relative path leading to this fs node.
    */
   virtual std::string getRelativePath() const = 0;

   /**
    * This is a call marshaling method - a node can marshal it or not, thus allowing
    * for new items creation or not.
    * 
    * @param tree       host tree
    */
   virtual TreeWidgetDescFactory* getDescFactory( ProjectTree& tree ) = 0;

   /**
    * This is a call marshaling method - a node can marshal it or not, thus allowing
    * for new items to be added or not.
    *
    * @param typeIdx
    * @param tree       host tree
    */
   virtual void addNode( unsigned int typeIdx, ProjectTree& tree ) = 0;

   /**
    * Compares the node name.
    *
    * @param name
    */
   virtual bool compareNodeName( const std::string& name ) const = 0;

   /**
    * Removes a child node, deleting the corresponding entry from
    * the file system.
    *
    * @param child
    */
   void removeNode( ProjectTreeNode* child );

   /**
    * Removes all node's children, physically deleting the corresponding
    * entries from the file system.
    */
   void clearNodes();

   /**
    * Removes all node's children, but doesn't touch their physical
    * file system versions.
    */
   void clear();

protected:
   /**
    * Constructor for common nodes.
    *
    * @param parent
    */
   ProjectTreeNode( ProjectTreeNode* parent );

   /**
    * Constructor for the root node.
    *
    * @param parent
    */
   ProjectTreeNode( QTreeWidget* parent );
};

///////////////////////////////////////////////////////////////////////////////
