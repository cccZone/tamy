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

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a node representation. Nodes can form tree hierarchies.
 */
class Node
{
protected:
   // local coordinate system
   D3DXMATRIX m_localMtx;

private:
   std::string m_name;
   
   // local coordinate system
   BoundingVolume* m_volume;

   // global coordinate system
   mutable D3DXMATRIX m_globalMtx;
   mutable D3DXMATRIX m_parentGlobalMtxCache;
   mutable D3DXMATRIX m_localMtxCache;
   BoundingVolume* m_globalVolume;

   Node* m_parent;
   std::list<Node*> m_childrenNodes;
   std::list<NodeObserver*> m_observers;

public:
   Node(const std::string& name);
   virtual ~Node();

   /**
    * Changes the node's name.
    *
    * @param name
    */
   inline void setName( const std::string& name ) { m_name = name; }

   /**
    * Each node has a name(meaningful or otherwise) which serves
    * the node identification purpose. 
    */
   inline const std::string& getName() const {return m_name;}

   /**
    * This is the matrix that describes the node's absolute world position
    * (unlike the local matrix which describes the position relative to node's
    * parent).
    */
   virtual const D3DXMATRIX& getGlobalMtx() const;

   /**
    * This is the matrix that describes the node's position in relation
    * to the position of its parent.
    * It the node doesn't have a parent, this one will be equal
    * to the global matrix
    */
   virtual const D3DXMATRIX& getLocalMtx() const {return m_localMtx;}

   /**
    * Assings the node a new local matrix.
    */
   virtual void setLocalMtx(const D3DXMATRIX& localMtx);

   /**
    * The method allows to access the matrix of the node directly,
    * skipping the setters.
    * Manipulating the matrix in this way is sometimes necessary
    * as various libs manipulate pointers to matrices. 
    * Not to worry - the global matrix will always remain in sync
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

   /*
    * Returns the global coordinate system vectors
    */
   void getGlobalVectors( D3DXVECTOR3& right, D3DXVECTOR3& up, D3DXVECTOR3& look, D3DXVECTOR3& pos ) const;

   /**
    * This method allows to set a bounding volume for the node
    */
   void setBoundingVolume(BoundingVolume* volume);

   /**
    * Returns the bounding volume that bounds the node's contents. 
    * The bounding volume is located in the world space.
    *
    * @return  node's contents bounding volume
    */
   const BoundingVolume& getBoundingVolume();

   /**
    * A node can have a single parent node. This method will return true
    * if this is the case.
    */
   bool hasParentNode() const {return m_parent != NULL;}

   /**
    * A node can have a single parent node. This method will return 
    * a reference to it if this is the case.
    *
    * @throws std::logic_error if the parent doesn't have a node set
    */
   Node& getParentNode();

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

   /**
    * Removes a child from the list of children and resets its
    * parental information.
    * The instance of the child node we want to remove needs to be the one
    * the instance of the parent node has an information about - no name based
    * lookups are provided.
    */
   void removeChild(Node& childNode);

   /**
    * Returns the number of attached children.
    */
   inline unsigned int getChildrenCount() const { return m_childrenNodes.size(); }

   /**
    * Returns an array of attached children.
    */
   inline const std::list<Node*>& getChildren() const {return m_childrenNodes;}

   /**
    * Looks for a node with the specified name in the attached hierarchy.
    *
    * @param name    searched node name
    * @return        pointer to the node, or NULL if none was found
    */
   Node* findNode( const std::string& name );

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
    * used as this class only (and it's not something derived from it),
    * then it's used as a Composite for other nodes.
    */
   virtual void onAccept(NodeVisitor& visitor) {}

   void setParent(Node& parent) {m_parent = &parent;}

   void resetParent() {m_parent = NULL;}
};

///////////////////////////////////////////////////////////////////////////////