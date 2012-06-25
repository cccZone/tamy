#pragma once

/// @file   TamyEditor\FSTreeNode.h
/// @brief  Resource browser tree widget node

#include <QTreeWidgetItem>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class ResourcesBrowser;
class Filesystem;
class TreeWidgetDescFactory;

///////////////////////////////////////////////////////////////////////////////

/**
 * An item describing an fs node in the tree structure
 */
class FSTreeNode : public QTreeWidgetItem 
{
protected:
   const Filesystem&    m_fs;

public:
   /**
    * Constructor creating the root node.
    *
    * @param hostTree
    * @param fs         file system from which we can access entry icons
    */
   virtual ~FSTreeNode() {}

   /**
    * Searches for a child entry with the specified name.
    *
    * @param nodeName
    */
   FSTreeNode* find( const std::string& nodeName );

   /**
    * Opens the resource encapsulated in the node for edition.
    *
    * @param editorsFactory    factory capable of creating proper editors
    */
   virtual void editResource( ResourcesBrowser& editorsFactory ) {}

   /**
    * Returns a relative path leading to this fs node.
    */
   virtual std::string getRelativePath() const = 0;

   /**
    * Returns an item descriptions factory for the specific node.
    * The factory will tell what items can be attached
    * to this node.
    * 
    * @param resourcesFactory    factory capable of creating the actual resources
    */
   virtual TreeWidgetDescFactory* getDescFactory( ResourcesBrowser& resourcesFactory ) = 0;

   /**
    * Creates a new child of the type specified by an index
    * expressed in terms of the node's TreeWidgetDescFactory.
    *
    * @param typeIdx
    * @param resourcesFactory    factory capable of creating the actual resources
    */
   virtual void addNode( unsigned int typeIdx, ResourcesBrowser& resourcesFactory ) = 0;

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
   void removeNode( FSTreeNode* child );

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
    * @param fs         file system which contains physical files & dirs 
    *                   corresponding to the nodes
    */
   FSTreeNode( FSTreeNode* parent, const Filesystem& fs );

   /**
    * Constructor for the root node.
    *
    * @param parent
    * @param fs         file system which contains physical files & dirs 
    *                   corresponding to the nodes
    */
   FSTreeNode( QTreeWidget* parent, const Filesystem& fs );
};

///////////////////////////////////////////////////////////////////////////////
