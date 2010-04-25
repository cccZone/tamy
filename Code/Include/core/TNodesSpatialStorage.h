#ifndef _T_NODES_SPATIAL_STORAGE_H
#define _T_NODES_SPATIAL_STORAGE_H

/// @file   core\TNodesSpatialStorage.h
/// @file   a spatial storage for keeping nodes

#include "core\SpatialStorage.h"
#include "core\Node.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "core\NodeObserver.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This storage is designed for storing hierarchy of nodes.
 * It provides a mechanism for extracting nodes of specific types
 * and querying them. That way we can store arbitrary hierarchies of nodes
 * here, and we can query only the nodes the storage was designed 
 * for querying.
 *
 * However it can't function alone - it requires an underlying
 * storage implementation, which will be responsible for storing
 * the extracted nodes of the specified type.
 */
template<typename NodeType, template<class> class ConcreteSpatialStorage>
class TNodesSpatialStorage : public SpatialStorage<NodeType>,
                             public NodeVisitor, 
                             public TNodesVisitor<NodeType>,
                             public NodeObserver
{
private:
   Node* m_root;
   ConcreteSpatialStorage<NodeType>* m_storage;

   // the flag is set temporarily when a new node is added
   // or removed to inform the visitor mechanism about an action
   // it should take when the specialized node is encountered
   bool m_isAdding;
   
public:
   // -------------------------------------------------------------------------
   // public interface
   // -------------------------------------------------------------------------

   /**
    * Constructor.
    *
    * @param storage    underlying storage that will store the specified
    *                   type of nodes.
    */
   TNodesSpatialStorage(ConcreteSpatialStorage<NodeType>* storage);
   ~TNodesSpatialStorage();

   /**
    * The method adds a new node to the storage, making
    * sure all the nodes this instance is specialized for
    * are collected in the spatial storage.
    *
    * @param node    a new node to add
    */
   void addNode(Node* node);

   /**
    * The method removes an existing node from the storage,
    * making sure its specialized children are also removed
    * from the spatial storage
    *
    * @param node    node to remove
    */
   void removeNode(Node& node);

   /**
    * This visitor will check the nodes it's specialized for
    * and try adding them to the global storage.
    *
    * @param node    currently visited node
    */
   void visit(NodeType& node);

   /**
   * The method allows to query the scene for nodes that overlap
   * the specified volume.
   *
   * @param volume   volume used for the query
   * @param output   upon method return this array will be filled with
   *                 nodes that fit inside or overlap the query volume
   */
   void query(const BoundingVolume& volume, Array<NodeType*>& output) const;

   /**
    * The method allows to access the root node all the nodes added
    * to this storage are attached to
    *
    * @return     composite root node
    */
   inline Node& root();


   // -------------------------------------------------------------------------
   // NodeObserver implementation
   // -------------------------------------------------------------------------

   void childAdded(Node& parent, Node& child);

   void childRemoved(Node& parent, Node& child);
};

///////////////////////////////////////////////////////////////////////////////

#include "core\TNodesSpatialStorage.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _T_NODES_SPATIAL_STORAGE_H
