#pragma once

/// @file   core\Node.h
/// @brief  a hierarchical tree node


#include <list>
#include <d3dx9.h>
#include <string>
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class NodeVisitor;
class NodeObserver;
class BoundingVolume;
struct Triangle;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a node representation. Nodes can form tree hierarchies.
 */
class Node
{
private:
   std::string m_name;
   
   // local coordinate system
   D3DXMATRIX m_localMtx;
   BoundingVolume* m_volume;

   // global coordinate system
   D3DXMATRIX m_globalMtx;
   D3DXMATRIX m_parentGlobalMtxCache;
   D3DXMATRIX m_localMtxCache;
   BoundingVolume* m_globalVolume;

   Node* m_parent;
   std::list<Node*> m_children;
   std::list<NodeObserver*> m_observers;

   Array<Triangle*> m_noGeometry;

public:
   Node(const std::string& name);
   virtual ~Node();

   /**
    * Each node has a name(meaningful or otherwise) which serves
    * the node identification purpose. 
    */
   const std::string& getName() const {return m_name;}

   /**
    * This is the matrix that describes the node's absolute world position
    * (unlike the local matrix which describes the position relative to node's
    * parent).
    */
   virtual const D3DXMATRIX& getGlobalMtx();

   /**
    * This is the matrix that describes the node's position in relation
    * to the position of its parent.
    * It the node doesn't have a parent, this one will be equal
    * to the global matrix
    */
   virtual const D3DXMATRIX& getLocalMtx() const {return m_localMtx;}

   virtual void setLocalMtx(const D3DXMATRIX& localMtx);

   /**
    * The method allows to access the matrix of the node directly,
    * skipping the setters.
    * Manipulating the matrix in this way is sometimes necessary
    * as various libs manipulate pointers to matrices. 
    * Not to worry - the global matrix will always remain in synch
    */
   virtual D3DXMATRIX& accessLocalMtx() {return m_localMtx;}

   /*
    * A group of accessors to the local coordinate system vectors
    */
   virtual void setRightVec(const D3DXVECTOR3& vec);
   virtual void setUpVec(const D3DXVECTOR3& vec);
   virtual void setLookVec(const D3DXVECTOR3& vec);
   virtual void setPosition(const D3DXVECTOR3& vec);
   virtual D3DXVECTOR3 getRightVec() const;
   virtual D3DXVECTOR3 getUpVec() const;
   virtual D3DXVECTOR3 getLookVec() const;
   virtual D3DXVECTOR3 getPosition() const;

   /**
    * This method returns the global bounding volume that bounds the node's contents
    */
   const BoundingVolume& getBoundingVolume();

   /**
    * This method allows to set a bounding volume for the node
    */
   void setBoundingVolume(BoundingVolume* volume);

   /**
    * This method returns the detailed geometry that bounds the
    * contents of a node. 
    *
    * This geometry will be used in detailed scene queries, 
    * and doesn't necessarily have to be the same as for instance 
    * the geometry of a graphical node.
    * It simply is something more detailed than the BoundingVolume, which
    * is used during the broad phase scene queries
    */
   virtual const Array<Triangle*>& getBoundingGeometry() const {return m_noGeometry;}

   /**
    * A node can have a single parent node. This method will return true
    * if this is the case.
    */
   bool hasParent() const {return m_parent != NULL;}

   /**
    * A node can have a single parent node. This method will return 
    * a reference to it if this is the case.
    *
    * @throws std::logic_error if the parent doesn't have a node set
    */
   Node& getParent();

   /**
    * A node can have any number of children. Their positions
    * will be kept relative to the parent's position.
    *
    * Since a node can't have more than one parent,
    * attaching a child node to two different parent nodes
    * will result in the node being reattached from the one
    * to which it was attached formerly.
    */
   void addChild(Node* childNode);

   unsigned int getChildrenCount() const {return m_children.size();}

   const std::list<Node*>& getChildren() const {return m_children;}
   
   /**
    * Removes a child from the list of children and resets its
    * parental information.
    * The instance of the child node we want to remove needs to be the one
    * the instance of the parent node has an information about - no name based
    * lookups are provided.
    */
   void removeChild(Node& childNode);

   /**
    * The hierarchy of nodes can be penetrated from outside using
    * the Visitor pattern. 
    *
    * This method calls onAccept on this node's instance to 
    * make the node react to the visitor if applicable,
    * then in relays the visitor to all of its children.
    */
   void accept(NodeVisitor& visitor);

   /**
    * The method allows to attach an observer to the node instance
    */
   void attachObserver(NodeObserver& observer);

   /**
    * Counterpart of the @see attachObserver method
    */
   void detachObserver(NodeObserver& observer);

protected:
   /**
    * The hierarchy of nodes can be penetrated from outside using
    * the Visitor pattern. Simply overload this method to accept a visitor.
    *
    * This particular type of node doesn't react to visitors - if it's
    * used as this class only (and it's not something deived from it),
    * then it's used as a Composite for other nodes.
    */
   virtual void onAccept(NodeVisitor& visitor) {}

   void setParent(Node& parent) {m_parent = &parent;}

   void resetParent() {m_parent = NULL;}
};

///////////////////////////////////////////////////////////////////////////////